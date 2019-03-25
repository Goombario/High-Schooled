#include "BattleState.h"
#include "GameEngine.h"
#include "ShareState.h"
#include "BattleObject.h"
#include "Fizzle\Fizzle.h"
#include "Input\InputMapper.h"
#include <ctime>


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

	void BattleState::reset()
	{

		states.clear();

		board1->clearTokens();
		board2->clearTokens();
		player1->Reset();
		player2->Reset();

		pushState(State::POS_CHOOSE);
		playerTurn = Side::LEFT;
		stage->setActiveBoard(playerTurn);
		getCurrentPlayer()->startTurn();
		getCurrentPlayer()->firstTurn();

		isEnd = false;
		choosingPos = 0;

		swapCurrentPlayer();
		int randomX = (rand() % 3) + 1;
		int randomY = (rand() % 3) + 1;
		for (int i = 0; i < randomX; i++)
		{
			getCurrentPlayer()->move(Direction::DOWN);
		}
		for (int i = 0; i < randomY; i++)
		{
			getCurrentPlayer()->move(Direction::FORWARD);
		}
		swapCurrentPlayer();
	}

	void BattleState::Init()
	{
		// Initialize the mapper context
		// Tells the mapper to call the given function after the contexts have been mapped.
		mapper = new InputMapping::InputMapper();
		mapper->AddCallback(mainCallback, 0);

		network = Network(33, 80, 7, 6);
		//network = Network("neuralnet.txt");

		network.randomizeInputWeights(-0.1f, 0.1f);
		network.randomizeHiddenWeights(-0.005f, 0.005f);
		network.randomizeOutputWeights(-0.1f, .1f);

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

		swapCurrentPlayer();
		int randomX = (rand() % 3) + 1;
		int randomY = (rand() % 3) + 1;
		for (int i = 0; i < randomX; i++)
		{
			getCurrentPlayer()->move(Direction::DOWN);
		}
		for (int i = 0; i < randomY; i++)
		{
			getCurrentPlayer()->move(Direction::FORWARD);
		}
		swapCurrentPlayer();
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
		shared::fillPreviouslyPressed(previouslyPressed, validKeys);
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
		//for (FzlKey key : validKeys)
		//{
		//	if (FzlGetKey(key))
		//	{
		//		bool previouslyDown = previouslyPressed[key];
		//		previouslyPressed[key] = true;
		//		mapper->SetRawButtonState(key, true, previouslyDown);
		//	}
		//	else
		//	{
		//		previouslyPressed[key] = false;
		//		mapper->SetRawButtonState(key, false, true);
		//	}
		//}


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

		// If the game is over, don't allow other inputs
		/*if (self->getCurrentState() == State::GAME_OVER)
		{
			return;
		}*/

		// Do not let any commands through if attacking/moving
		if (self->getCurrentState() == State::ACTING)
		{
			return;
		}

		if (self->getCurrentSide() == Side::LEFT)
		{
			if (self->getCurrentPlayer()->isActing())
			{

			}
			else
			{
				self->updateNetwork();
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

			// If choosing menu options, don't allow anything else7
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
				int randomX = (rand() % 3) + 1;
				int randomY = (rand() % 3) + 1;
				for (int i = 0; i < randomX; i++)
				{
					self->getCurrentPlayer()->move(Direction::DOWN);
				}
				for (int i = 0; i < randomY; i++)
				{
					self->getCurrentPlayer()->move(Direction::FORWARD);
				}
				self->swapCurrentPlayer();
				inputs.EatAction(InputMapping::Action::SELECT_POS);
			}
		}
		else
		{
			// Cooldown on actions 
			static float aiMoveCooldown = 0.f;

			if (aiMoveCooldown > 0.f)
			{
				aiMoveCooldown -= (1.0 / schooled::FRAMERATE);	// Locked framerate
				if (aiMoveCooldown < 0.f) aiMoveCooldown = 0.f;
				return;
			}

			if (self->getCurrentState() == State::GAME_OVER)
			{
				self->swapCurrentPlayer();
				self->stage->setFinished();
			}
			if (self->getCurrentPlayer()->getCurrentAP() > 0)
			{
				int action = 0;
				if (self->getCurrentState() != State::ATTACK_CHOOSE && self->getCurrentPlayer()->getCurrentAP() == 2)
				{
					action = rand() % 2;
					self->getCurrentPlayer()->setLastMove(1);
				}

				if (action == 0 && self->getCurrentPlayer()->getLastMove() < 2 && self->getCurrentPlayer()->hasMovesAvailable())
				{
					if (self->getCurrentState() != State::ATTACK_CHOOSE)
					{
						self->pushState(State::ATTACK_CHOOSE);
						self->getCurrentPlayer()->initAttackMenu(*self->getOtherPlayer());
						self->mapper->PushContext("p2AttackMenu");

						aiMoveCooldown += 0.1f;
					}
					else
					{
						int option = (rand() % 3) - 1;
						while (!self->getCurrentPlayer()->canUseAttack(option)) option = (rand() % 3) - 1;
						self->getCurrentPlayer()->moveSelectedAttack(option, *self->getOtherPlayer());

						self->getCurrentPlayer()->attack(*self->getOtherPlayer());
						self->getCurrentPlayer()->clearAttackMenu(*self->getOtherPlayer());
						self->popState();
						self->mapper->PopContext();
						self->stage->updateHPColour();
						self->getCurrentPlayer()->setLastMove(3);

						aiMoveCooldown += 0.1f;
						//if (self->getCurrentPlayer()->getCurrentAP() == 0) aiMoveCooldown = 0.f;
					}
				}
				else /*if (self->getCurrentPlayer()->getLastMove() > 0)*/
				{
					int randMove = (rand() % 4) + 1;
					switch (randMove)
					{
					case 1:
						self->getCurrentPlayer()->move(Direction::DOWN);
						break;
					case 2:
						self->getCurrentPlayer()->move(Direction::FORWARD);
						break;
					case 3:
						self->getCurrentPlayer()->move(Direction::UP);
						break;
					case 4:
						self->getCurrentPlayer()->move(Direction::BACKWARD);
						break;
					default:
						break;
					}
					self->getCurrentPlayer()->setLastMove(0);

					aiMoveCooldown += 0.1f;
				}
			}
			else
			{
				self->swapCurrentPlayer();
			}
		}
	}


	void BattleState::Update(GameEngine* game)
	{
		if (stage->isFinished() || player1->getCurrentHP() <= 0 || player2->getCurrentHP() <= 0 || actions > 2000)
		{
			if (actions > 2000)
			{
				printf("actions overload");
				esum += INFINITY;
			}
			bool isActing = false;
			//if (getCurrentPlayer()->isActing() || getOtherPlayer()->isActing())
			//	isActing = true;
			for (auto it = battleObjects.begin(); it != battleObjects.end(); it++)
			{
				if ((**it).isActing())
				{
					isActing = true;
				}
			}

			if (isActing)
			{

			}
			else
			{

				reset();

				static int roundCounter = 0;
				static int iterations = 0;
				roundCounter++;

				if (roundCounter > 5 && iterations < 100)
				{
					iterations++;

					esum = esum / (double)actions;
					roundCounter = 0;
					actions = 0;
					printf("%f %f", esum, lastEsum);

					if (lastEsum < esum)
					{
						printf("uh oh it got worse");
						network.revertWeights();
					}
					else
					{
						lastEsum = esum;
						network.saveToFile("neuralNet", false);
					}
					esum = 0;

					printf("\n");
					if (randomize == true)
					{
						srand(time(NULL));
						network.IncrementRandomizeInputWeights(-0.05f, 0.05f);
						network.IncrementRandomizeHiddenWeights(-0.001f, 0.001f);
						network.IncrementRandomizeOutputWeights(-0.01f, 0.01f);
					}
				}
				//we have found the best one at the current moment
				else
				{
					drawGame = true;
					return;
				}

				srand(roundCounter);
				return;
			}
		}

		if (isEnd)
		{
			game->Quit();
		}

		for (auto it = battleObjects.begin(); it != battleObjects.end(); it++)
		{
			(**it).update();
		}

		player1->updateProjectiles(*player2);
		player2->updateProjectiles(*player1);

		if (getCurrentPlayer()->canUseSpecial() &&
			!getCurrentPlayer()->isActing() &&
			!getOtherPlayer()->getBoard()->isActing())
		{
			getCurrentPlayer()->useSpecial(*getOtherPlayer());
			getCurrentPlayer()->update();
		}

		if ((player1->getCurrentHP() <= 0 || player2->getCurrentHP() <= 0) &&
			!player1->isActing() && !player2->isActing() && getCurrentState() != State::GAME_OVER)
		{
			pushState(State::GAME_OVER);
			//stage->stopGame();
		}

		//stage->setDark(getCurrentPlayer()->isUsingSpecial() || getCurrentState() == State::GAME_OVER);
		stage->updateHPColour();
		stage->update();

		// If a player is acting (moving, attacking) set the state to ACTING
		bool isActing = false;
		//if (getCurrentPlayer()->isActing() || getOtherPlayer()->isActing())
		//	isActing = true;
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

		int currentPlayerAP = getCurrentPlayer()->getCurrentAP();
		bool currentPlayerIsActing = getCurrentPlayer()->isActing();
		bool otherBoardIsActing = getOtherPlayer()->getBoard()->isActing();

		// If the player is out of action points
		if (getCurrentPlayer()->getCurrentAP() == 0 &&
			!getCurrentPlayer()->isActing() &&
			!getOtherPlayer()->getBoard()->isActing())
		{
			swapCurrentPlayer();
		}
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

	static int lastAP = 2;
	void BattleState::updateNetwork()
	{
		actions++;
		int counter = 0;

		//get the board states
		for (int i = 0; i < 3; i++)
		{
			for (int j = 0; j < 3; j++)
			{
				data[counter] = board1->hasToken(i, j);
				counter++;
				data[counter] = board2->hasToken(i, j);
				counter++;
			}
		}

		//where we are
		//where the enemy is
		//moves we can make
		//hp, sp
		//enemy hp, sp
		data[counter++] = player1->getBoard()->getPlayerlocation().Y;
		data[counter++] = player1->getBoard()->getPlayerlocation().X;
		data[counter++] = player1->getMovesAvailable()[0];
		data[counter++] = player1->getMovesAvailable()[1];
		data[counter++] = player1->getMovesAvailable()[2];
		data[counter++] = player1->getCurrentHP();
		data[counter++] = player1->getCurrentSP();
		data[counter++] = player1->getCurrentAP();

		data[counter++] = player2->getBoard()->getPlayerlocation().Y;
		data[counter++] = player2->getBoard()->getPlayerlocation().X;
		data[counter++] = player2->getMovesAvailable()[0];
		data[counter++] = player2->getMovesAvailable()[1];
		data[counter++] = player2->getMovesAvailable()[2];
		data[counter++] = player2->getCurrentHP();
		data[counter++] = player2->getCurrentSP();
		data[counter++] = player2->getCurrentAP();

		float error = 0;
		//if the ap hasn't changed we failed to make a move therefore we 


		int index = 0;
		double max = 0;

		for (int i = 0; i < network.getOutputSize(); i++)
		{
			if (max < network.getFinalResult(i))
			{
				max = network.getFinalResult(i);
				index = i;
			}
		}

		int enemyHealth = player2->getCurrentHP();
		int ourSP = player1->getCurrentSP();


		// creating the stats we want to track
		while (lastAP == player1->getCurrentAP())
		{

			int index = 0;
			double max = 0;
			network.input(data);

			for (int i = 0; i < network.getOutputSize(); i++)
			{
				if (max < network.getFinalResult(i))
				{
					max = network.getFinalResult(i);
					index = i;
				}
			}

			switch (index)
			{
			case 0:
				player1->move(Direction::UP);
				break;
			case 1:
				player1->move(Direction::FORWARD);
				break;
			case 2:
				player1->move(Direction::DOWN);
				break;
			case 3:
				player1->move(Direction::BACKWARD);
				break;
			case 4:
				player1->initAttackMenu(*player2);
				player1->moveSelectedAttack(0, *player2);
				player1->attack(*player2);
				player1->clearAttackMenu(*player2);
				stage->updateHPColour();
				break;
			case 5:
				player1->initAttackMenu(*player2);
				player1->moveSelectedAttack(1, *player2);
				player1->attack(*player2);
				player1->clearAttackMenu(*player2);
				stage->updateHPColour();
				break;
			case 6:
				player1->initAttackMenu(*player2);
				player1->moveSelectedAttack(-1, *player2);
				player1->attack(*player2);
				player1->clearAttackMenu(*player2);
				stage->updateHPColour();
				break;

			}

			network.randomizeInputWeights(-0.1f, 0.1f);
			network.randomizeHiddenWeights(-0.005f, 0.005f);
			network.randomizeOutputWeights(-0.1f, .1f);
		}

		//if we are on a token at the end of the turn
		COORD pos = player1->getBoard()->getPlayerlocation();
		if (board1->hasToken(pos) && player1->getCurrentAP() == 0)
		{
			error -= 3;
		}
		//
		if (enemyHealth > player2->getCurrentHP())
		{
			error -= 5;
			error -= player2->getMaxHP() - player2->getCurrentHP();
		}
		if (ourSP > player1->getCurrentSP())
		{
			error -= player1->getCurrentSP();
		}
		if (player1->getCurrentSP() == player1->getMaxAP())
		{
			error -= 5;
		}
		if (index >= 4)
		{
			error -= 3;
		}
		else
		{
			error -= 2;
		}

		if (player1->getCurrentHP() == 0)
		{
			error += 10 * player2->getCurrentHP();
		}
		else if (player2->getCurrentHP() == 0)
		{
			esum -= 20 * player1->getCurrentHP();
		}


		esum += error;

		lastAP = player1->getCurrentAP();


	}

}