#include "BattleState.h"
#include "GameEngine.h"
#include "ShareState.h"
#include "BattleObject.h"
#include "Fizzle\Fizzle.h"
#include "Input\InputMapper.h"

#include "Player.h"
#include "Stage.h"
#include "Board.h"

#include <iostream>
#include <fstream>
using std::string;

namespace Rules
{
	bool const REMOVING_TOKENS = false;
}

namespace BattleState
{
	BattleState BattleState::m_BattleState;

	void BattleState::Init()
	{
		// Initialize the mapper context
		// Tells the mapper to call the given function after the contexts have been mapped.
		mapper = new InputMapping::InputMapper();

		mapper->AddCallback(mainCallback, 0);


		// Tells the mapper to map a specific set of keys to a specific set of actions
		mapper->PushContext("globalContext");
		mapper->PushContext("player1ChoosePos");

		// Hold valid keys
		validKeys.clear();
		previouslyPressed.clear();
		shared::initValidKeys(validKeys);

		// Hold pressed keys
		shared::initPreviouslyPressed(previouslyPressed, validKeys);

		board1 = new Board::Board(Side::LEFT);
		board2 = new Board::Board(Side::RIGHT);
		loadData();

		// Set the list of battle objects
		battleObjects.push_back(board1);
		battleObjects.push_back(board2);
		battleObjects.push_back(player1);
		battleObjects.push_back(player2);

		states.clear();
		pushState(State::POS_CHOOSE);
		playerTurn = Side::LEFT;
		stage->setActiveBoard(playerTurn);
		getCurrentPlayer()->startTurn();
		getCurrentPlayer()->firstTurn();

		isEnd = false;
		choosingPos = 0;
	}

	void BattleState::Cleanup()
	{
		// Cleanup
		for (auto it = battleObjects.begin(); it != battleObjects.end(); it++)
		{
			delete *it;
		}
		battleObjects.clear();
		states.clear();

		delete stage;
		delete mapper;

		player1 = nullptr;
		player2 = nullptr;
		board1 = nullptr;
		board2 = nullptr;
		stage = nullptr;
	}

	void BattleState::Pause()
	{
		// Suspend sounds and potentially pop contexts
		mapper->PopContext();
		mapper->PopContext();
	}

	void BattleState::Resume()
	{
		// Resume sounds and push contexts
		mapper->PushContext("globalContext");
		
		// Find which context to push
		if (getCurrentSide() == Side::RIGHT)
		{
			switch (getCurrentState())
			{
			case State::EMPTY:
				break;
			case State::POS_CHOOSE:
				mapper->PushContext("player2ChoosePos");
				break;
			case State::ATTACK_CHOOSE:
				mapper->PushContext("p2AttackMenu");
				break;
			case State::MOVE:
				mapper->PushContext("player2Action");
				break;
			case State::ACTING:
				mapper->PushContext("player2Action");
				break;
			default:
				break;
			}
		}
		else
		{
			switch (getCurrentState())
			{
			case State::EMPTY:
				break;
			case State::POS_CHOOSE:
				mapper->PushContext("player1ChoosePos");
				break;
			case State::ATTACK_CHOOSE:
				mapper->PushContext("p1AttackMenu");
				break;
			case State::MOVE:
				mapper->PushContext("player1Action");
				break;
			case State::ACTING:
				mapper->PushContext("player1Action");
				break;
			default:
				break;
			}
		}
	}

	void BattleState::HandleEvents(GameEngine* game)
	{
		// Go through all valid keys and set if they are down or not, and if they have been previously down.
		for (FzlKey key : validKeys)
		{
			if (FzlGetKey(key))
			{
				bool previouslyDown = previouslyPressed[key];
				previouslyPressed[key] = true;
				mapper->SetRawButtonState(key, true, previouslyDown);
			}
			else
			{
				previouslyPressed[key] = false;
				mapper->SetRawButtonState(key, false, true);
			}
		}


		// push the handled inputs to the mapper
		mapper->Dispatch();
		mapper->Clear();
		// Mapper dispatches automatically at end
	}

	void mainCallback(InputMapping::MappedInput& inputs)
	{
		BattleState *self = BattleState::Instance();

		if (inputs.Actions.find(InputMapping::Action::EXIT_GAME) != inputs.Actions.end())
		{
			self->isEnd = true;
		}

		// Do not let any commands through if attacking/moving
		if (self->getCurrentState() == State::ACTING)
		{
			return;
		}

		if (inputs.Actions.find(InputMapping::Action::MENU_SELECT) != inputs.Actions.end())
		{
			if (self->getCurrentState() == State::GAME_OVER)
			{
				self->stage->setFinished();
			}
			else if (self->getCurrentState() != State::ATTACK_CHOOSE)
			{
				self->pushState(State::ATTACK_CHOOSE);
				self->getCurrentPlayer()->initAttackMenu(*self->getOtherPlayer());
				if (self->getCurrentSide() == Side::LEFT)
				{
					self->mapper->PushContext("p1AttackMenu");
				}
				else
				{
					self->mapper->PushContext("p2AttackMenu");
				}
			}
			else
			{
				bool success = self->getCurrentPlayer()->attack(*self->getOtherPlayer());
				if (success)
				{
					self->getCurrentPlayer()->clearAttackMenu(*self->getOtherPlayer());
					self->popState();
					self->mapper->PopContext();
					self->stage->updateHPColour();
				}
			}
		}

		// If the game is over, don't allow other inputs
		if (self->getCurrentState() == State::GAME_OVER)
		{
			return;
		}

		if (inputs.Actions.find(InputMapping::Action::MENU_BACK) != inputs.Actions.end())
		{
			self->getCurrentPlayer()->clearAttackMenu(*self->getOtherPlayer());
			self->popState();
			self->mapper->PopContext();
			inputs.EatAction(InputMapping::Action::MENU_BACK);
		}

		if (inputs.Actions.find(InputMapping::Action::MENU_UP) != inputs.Actions.end())
		{
			self->getCurrentPlayer()->moveSelectedAttack(1, *self->getOtherPlayer());
			inputs.EatAction(InputMapping::Action::MENU_UP);
		}

		if (inputs.Actions.find(InputMapping::Action::MENU_DOWN) != inputs.Actions.end())
		{
			self->getCurrentPlayer()->moveSelectedAttack(-1, *self->getOtherPlayer());
			inputs.EatAction(InputMapping::Action::MENU_DOWN);
		}

		// If choosing menu options, don't allow anything else
		if (self->getCurrentState() == State::ATTACK_CHOOSE) return;

		if (inputs.Actions.find(InputMapping::Action::BOARD_UP) != inputs.Actions.end())
		{
			self->getCurrentPlayer()->move(Direction::UP);
			inputs.EatAction(InputMapping::Action::BOARD_UP);
		}

		if (inputs.Actions.find(InputMapping::Action::BOARD_DOWN) != inputs.Actions.end())
		{
			self->getCurrentPlayer()->move(Direction::DOWN);
			inputs.EatAction(InputMapping::Action::BOARD_DOWN);
		}

		if (inputs.Actions.find(InputMapping::Action::BOARD_FORWARD) != inputs.Actions.end())
		{
			self->getCurrentPlayer()->move(Direction::FORWARD);
			inputs.EatAction(InputMapping::Action::BOARD_FORWARD);
		}

		if (inputs.Actions.find(InputMapping::Action::BOARD_BACKWARD) != inputs.Actions.end())
		{
			self->getCurrentPlayer()->move(Direction::BACKWARD);
			inputs.EatAction(InputMapping::Action::BOARD_BACKWARD);
		}

		if (inputs.Actions.find(InputMapping::Action::SELECT_POS) != inputs.Actions.end())
		{
			self->swapCurrentPlayer();
			inputs.EatAction(InputMapping::Action::SELECT_POS);
		}
	}

	void BattleState::Update(GameEngine* game)
	{
		if (stage->isFinished())
		{
			game->PopState();
			return;
		}

		if (isEnd) game->Quit();

		for (auto it = battleObjects.begin(); it != battleObjects.end(); it++)
		{
			(**it).update();
		}

		if (getCurrentPlayer()->canUseSpecial() && 
			!getCurrentPlayer()->isActing() &&
			!getOtherPlayer()->getBoard()->isActing())
		{
			getCurrentPlayer()->useSpecial(*getOtherPlayer());
			getCurrentPlayer()->update();
		}

		if ((player1->getCurrentHP() == 0 || player2->getCurrentHP() == 0) &&
			!player1->isActing() && !player2->isActing() && getCurrentState() != State::GAME_OVER)
		{
			pushState(State::GAME_OVER);
			stage->stopGame();
		}

		stage->setDark(getCurrentPlayer()->isUsingSpecial() || getCurrentState() == State::GAME_OVER);
		stage->updateHPColour();
		stage->update();

		// If a player is acting (moving, attacking) set the state to ACTING
		bool isActing = false;
		for (auto it = battleObjects.begin(); it != battleObjects.end(); it++)
		{
			if ((**it).isActing())
			{
				isActing = true;
			}
		}
		if (isActing)
		{
			pushState(State::ACTING);
		}
		else if (!isActing && getCurrentState() == State::ACTING)
		{
			popState();
		}

		// If the player is out of action points
		if (getCurrentPlayer()->getCurrentAP() == 0 && 
			!getCurrentPlayer()->isActing() &&
			!getOtherPlayer()->getBoard()->isActing())
		{
			swapCurrentPlayer();
		}
		// FMOD updates automatically at end
	}

	void BattleState::Draw(GameEngine* game)
	{
		stage->drawBackground();
		for (auto it = battleObjects.begin(); it != battleObjects.end(); it++)
		{
			(**it).draw();
		}
		stage->drawHUD();
		// Fizzle swaps buffer automatically at end
	}

	void BattleState::swapCurrentPlayer()
	{
		getCurrentPlayer()->passTurn();

		// If both sides have chosen, switch to normal state
		if (choosingPos < 2 && getCurrentState() == State::POS_CHOOSE)
		{
			choosingPos++;
			getCurrentPlayer()->endChoosing();
			if (choosingPos == 2)
			{
				popState();
				pushState(State::MOVE);
				getCurrentPlayer()->clearBoardTiles();
				getOtherPlayer()->clearBoardTiles();
			}
		}

		// Swap the players
		if (playerTurn == Side::LEFT)
		{
			playerTurn = Side::RIGHT;
			mapper->PopContext();

			// Choose the correct context
			switch (getCurrentState())
			{
			case State::POS_CHOOSE:
				mapper->PushContext("player2ChoosePos");
				break;
			default:
				mapper->PushContext("player2Action");
				break;
			}
		}
		else
		{
			playerTurn = Side::LEFT;
			mapper->PopContext();

			// Choose the correct context
			switch (getCurrentState())
			{
			case State::POS_CHOOSE:
				mapper->PushContext("player1ChoosePos");
				break;
			default:
				mapper->PushContext("player1Action");
				break;
			}
		}

		stage->setActiveBoard(playerTurn);
		// Start the current player's turn
		getCurrentPlayer()->startTurn();
	}

	Player::Player* BattleState::getCurrentPlayer()
	{
		if (playerTurn == Side::LEFT)
			return player1;
		return player2;
	}

	Player::Player* BattleState::getOtherPlayer()
	{
		if (playerTurn == Side::LEFT)
			return player2;
		return player1;
	}

	State BattleState::getCurrentState() 
	{
		if (states.empty())
			return State::EMPTY;
		return states.back(); 
	}

	void BattleState::pushState(State newState)
	{
		if (getCurrentState() != newState) states.push_back(newState);
	}

	void BattleState::popState()
	{ 
		states.pop_back();
	}

	void BattleState::loadData()
	{
		std::ifstream fileStream("Save.txt");

		std::string player1Name, player2Name, stageName;
		std::getline(fileStream, player1Name);
		std::getline(fileStream, player2Name);
		std::getline(fileStream, stageName);

		player1 = new Player::Player(player1Name.c_str(), *board1);
		player2 = new Player::Player(player2Name.c_str(), *board2);
		stage = new Stage::Stage(stageName.c_str(), player1, player2);

		fileStream.close();
	}
}

