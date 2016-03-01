#include "BattleState.h"
#include "GameEngine.h"
#include "ShareState.h"
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
		// Tells the mapper to map a specific set of keys to a specific set of actions
		GameEngine::getMapper()->PushContext("globalContext");

		// Tells the mapper to call the given function after the contexts have been mapped.
		GameEngine::getMapper()->AddCallback(mainCallback, 0);

		// Hold valid keys
		shared::initValidKeys(validKeys);

		// Hold pressed keys
		shared::initPreviouslyPressed(previouslyPressed, validKeys);

		board1 = new Board::Board();
		board2 = new Board::Board();
		player1 = new Player::Player("Gym Teacher", board1);
		player2 = new Player::Player("Gym Teacher", board2);

		// Place tokens onto a board, and check for matches (DEBUG)
		Board::Board testBoard;
		testBoard.placeToken(0);
		testBoard.placeToken(1);
		testBoard.placeToken(2);
		testBoard.placeToken(3);
		testBoard.print();

		std::cout << testBoard.checkMatches() << std::endl;
		testBoard.print();

	}

	void BattleState::Cleanup()
	{
		// Cleanup
		delete player1;
		delete player2;
		delete board1;
		delete board2;

		player1 = nullptr;
		player2 = nullptr;
		board1 = nullptr;
		board2 = nullptr;
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
	}

	void BattleState::Update(GameEngine* game)
	{
		player1->update();
		player2->update();
		// FMOD updates automatically at end
	}

	void BattleState::Draw(GameEngine* game)
	{
		player1->draw();
		player2->draw();
		// Fizzle swaps buffer automatically at end
	}
}

