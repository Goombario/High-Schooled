#ifndef BATTLE_STATE_H
#define BATTLE_STATE_H

#include "GameState.h"
#include "BattleConstants.h"
#include "Fizzle\DataTypes\FizzleDataTypes.h"
#include <map>
#include <vector>

// Forward Declaration
namespace InputMapping
{
	struct MappedInput;
}

namespace Board
{
	class Board;
}

namespace Player
{
	class Player;
}

namespace Stage
{
	class Stage;
}

namespace BattleObject
{
	class BattleObject;
}

namespace BattleState
{
	enum class State : int
	{
		EMPTY = -1,
		POS_CHOOSE,
		ATTACK_CHOOSE,
		MOVE,
		ACTING,
	};

	class BattleState : public GameState
	{
	public:
		// Initialize the state
		void Init();

		// Cleanup the state before exiting
		void Cleanup();

		// Pause or resume the state before/after changing
		void Pause();
		void Resume();

		// Operations done by the state, can call to the game engine
		void HandleEvents(GameEngine* game);
		void Update(GameEngine* game);
		void Draw(GameEngine* game);

		// Return an instance of the object
		static BattleState* Instance() {
			return &m_BattleState;
		}

		// The callback used in the state
		friend void mainCallback(InputMapping::MappedInput& inputs);

		// Get the current state and side of the battle class
		State getCurrentState();
		inline Side getCurrentSide() const { return playerTurn; }


	protected:
		// Create an empty object
		BattleState() { }

	private:
		static BattleState m_BattleState;

		// Load the save data
		void loadData();

		// Change the turn to the other player
		void swapCurrentPlayer();

		// Get the current player and other player
		Player::Player* getCurrentPlayer();
		Player::Player* getOtherPlayer();

		// Modify the current state
		void pushState(State newState);
		void popState();

		// Event handling helpers
		std::vector<FzlKey> validKeys;
		std::map<FzlKey, bool> previouslyPressed;

		// Battle objects
		Player::Player* player1;
		Player::Player* player2;
		Board::Board* board1;
		Board::Board* board2;
		Stage::Stage* stage;

		// List of battle objects
		std::vector<BattleObject::BattleObject*> battleObjects;

		bool isEnd;
		Side playerTurn;
		unsigned int choosingPos;
		std::vector<State> states;
	};
}

#endif