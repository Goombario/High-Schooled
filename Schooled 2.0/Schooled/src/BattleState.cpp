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
		GameEngine::getMapper()->AddCallback(mainCallback, 0);

		// Tells the mapper to map a specific set of keys to a specific set of actions
		GameEngine::getMapper()->Clear();
		GameEngine::getMapper()->PushContext("globalContext");
		GameEngine::getMapper()->PushContext("player1Action");

		// Hold valid keys
		shared::initValidKeys(validKeys);

		// Hold pressed keys
		shared::initPreviouslyPressed(previouslyPressed, validKeys);

		board1 = new Board::Board(Side::LEFT);
		board2 = new Board::Board(Side::RIGHT);
		player1 = new Player::Player("Nate", board1, Side::LEFT);
		player2 = new Player::Player("Gym Teacher", board2, Side::RIGHT);
		stage = new Stage::Stage("Default", player1, player2);

		board1->setTokenSprite(player2->getTokenSprite());
		board2->setTokenSprite(player1->getTokenSprite());

		// Set the list of battle objects
		battleObjects.push_back(player1);
		battleObjects.push_back(player2);
		battleObjects.push_back(board1);
		battleObjects.push_back(board2);

		playerTurn = Side::LEFT;
		player1->startTurn();

		isEnd = false;
	}

	void BattleState::Cleanup()
	{
		// Cleanup
		for (auto it = battleObjects.begin(); it != battleObjects.end(); it++)
		{
			delete *it;
		}
		battleObjects.clear();

		delete stage;

		player1 = nullptr;
		player2 = nullptr;
		board1 = nullptr;
		board2 = nullptr;
		stage = nullptr;
	}

	void BattleState::Pause()
	{
		// Suspend sounds and potentially pop contexts
	}

	void BattleState::Resume()
	{
		// Resume sounds and push contexts
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
				game->getMapper()->SetRawButtonState(key, true, previouslyDown);
			}
			else
			{
				previouslyPressed[key] = false;
				game->getMapper()->SetRawButtonState(key, false, true);
			}
		}
		// Mapper dispatches automatically at end
	}

	void mainCallback(InputMapping::MappedInput& inputs)
	{
		BattleState *self = BattleState::Instance();

		if (inputs.Actions.find(InputMapping::Action::EXIT_GAME) != inputs.Actions.end())
		{
			self->isEnd = true;
			inputs.EatAction(InputMapping::Action::EXIT_GAME);
		}

		// If a player is acting (moving, attacking) don't let commands through
		for (auto it = self->battleObjects.begin(); it != self->battleObjects.end(); it++)
		{
			if ((**it).isActing())
			{
				return;
			}
		}

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

		if (inputs.Actions.find(InputMapping::Action::ATTACK_1) != inputs.Actions.end())
		{
			self->getCurrentPlayer()->attack(*self->getOtherPlayer(), 0);
			inputs.EatAction(InputMapping::Action::ATTACK_1);
		}

		if (inputs.Actions.find(InputMapping::Action::ATTACK_2) != inputs.Actions.end())
		{
			self->getCurrentPlayer()->attack(*self->getOtherPlayer(), 1);
			inputs.EatAction(InputMapping::Action::ATTACK_2);
		}

		if (inputs.Actions.find(InputMapping::Action::ATTACK_3) != inputs.Actions.end())
		{
			self->getCurrentPlayer()->attack(*self->getOtherPlayer(), 2);
			inputs.EatAction(InputMapping::Action::ATTACK_3);
		}
	}

	void BattleState::Update(GameEngine* game)
	{
		if (isEnd || player1->getCurrentHP() == 0 || player2->getCurrentHP() == 0) game->Quit();
		for (auto it = battleObjects.begin(); it != battleObjects.end(); it++)
		{
			(**it).update();
		}

		if (getCurrentPlayer()->getCurrentAP() == 0)
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

		if (playerTurn == Side::LEFT)
		{
			playerTurn = Side::RIGHT;
			GameEngine::getMapper()->PushContext("player2Action");
		}
		else
		{
			playerTurn = Side::LEFT;
			GameEngine::getMapper()->PopContext();
		}

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
}

