#ifndef MENU_STATE_H
#define MENU_STATE_H

#include "GameState.h"
#include "BattleConstants.h"
#include "Fizzle\DataTypes\FizzleDataTypes.h"
#include <map>
#include <vector>

namespace MenuState
{
	class MenuState : public GameState
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
		static MenuState* Instance() {
			return &m_MenuState;
		}

	protected:
		// Create an empty object
		MenuState() { }

	private:
		static MenuState m_MenuState;

		// Event handling helpers
		std::vector<FzlKey> validKeys;
		std::map<FzlKey, bool> previouslyPressed;

		bool isEnd;
	};
}

#endif