#ifndef MENU_STATE_H
#define MENU_STATE_H

#include "GameState.h"
#include "Fizzle\DataTypes\FizzleDataTypes.h"
#include <map>
#include <vector>

// Forward Declaration
namespace InputMapping
{
	struct MappedInput;
}

namespace Menu
{
	class Menu;
	class StageMenu;
	class MainMenu;
	class CharMenu;
}

namespace MenuState
{
	enum class State
	{
		MAIN_MENU,
		CHAR_MENU,
		STAGE_MENU
	};

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

		// Get the current state of the menu
		State getCurrentState() const { return currentState; }

		// The callback used in the state
		friend void mainCallback(InputMapping::MappedInput& inputs);

	protected:
		// Create an empty object
		MenuState() { }

	private:
		static MenuState m_MenuState;

		// Change the current state of the menu
		void changeMenuState(State);

		// Save the pertinent data to a file
		void saveData();

		// Event handling helpers
		std::vector<FzlKey> validKeys;
		std::map<FzlKey, bool> previouslyPressed;

		// State variables
		bool isEnd;
		State currentState;

		// Menu objects
		Menu::MainMenu *mainMenu;
		Menu::CharMenu *p1CharMenu, *p2CharMenu;
		Menu::StageMenu *stageMenu;
		
		double timer;
	};
}

#endif