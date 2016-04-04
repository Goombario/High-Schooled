#include "MenuState.h"
#include "GameEngine.h"
#include "ShareState.h"
#include "Fizzle\Fizzle.h"
#include "Input\InputMapper.h"

#include "Menu.h"
#include "Sprite.h"

#include <iostream>
using std::string;

namespace MenuState
{
	MenuState MenuState::m_MenuState;

	void MenuState::Init()
	{
		// Initialize the mapper context
		// Tells the mapper to map a specific set of keys to a specific set of actions
		GameEngine::getMapper()->PushContext("globalContext");
		GameEngine::getMapper()->PushContext("mainMenuContext");

		// Tells the mapper to call the given function after the contexts have been mapped.
		GameEngine::getMapper()->AddCallback(mainCallback, 0);

		// Hold valid keys
		shared::initValidKeys(validKeys);

		// Hold pressed keys
		shared::initPreviouslyPressed(previouslyPressed, validKeys);

		charMenu = new Menu::CharMenu("d");
		selectedMenu = charMenu;
	}

	void MenuState::Cleanup()
	{
		// Cleanup all pointers
	}

	void MenuState::Pause()
	{
		// Suspend sounds and potentially pop contexts
	}

	void MenuState::Resume()
	{
		// Resume sounds and push contexts
	}

	void MenuState::HandleEvents(GameEngine* game)
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
		MenuState *self = MenuState::Instance();

		if (inputs.Actions.find(InputMapping::Action::EXIT_GAME) != inputs.Actions.end())
		{
			self->isEnd = true;
			inputs.EatAction(InputMapping::Action::EXIT_GAME);
		}

		if (inputs.Actions.find(InputMapping::Action::MENU_UP) != inputs.Actions.end())
		{
			inputs.EatAction(InputMapping::Action::MENU_UP);
		}

		if (inputs.Actions.find(InputMapping::Action::MENU_DOWN) != inputs.Actions.end())
		{
			inputs.EatAction(InputMapping::Action::MENU_DOWN);
		}

		if (inputs.Actions.find(InputMapping::Action::MENU_SELECT) != inputs.Actions.end())
		{
			inputs.EatAction(InputMapping::Action::MENU_SELECT);
		}

		if (inputs.Actions.find(InputMapping::Action::MENU_BACK) != inputs.Actions.end())
		{
			inputs.EatAction(InputMapping::Action::MENU_BACK);
		}

		if (inputs.Actions.find(InputMapping::Action::MENU_P1_RIGHT) != inputs.Actions.end())
		{
			inputs.EatAction(InputMapping::Action::MENU_P1_RIGHT);
		}

		if (inputs.Actions.find(InputMapping::Action::MENU_P1_LEFT) != inputs.Actions.end())
		{
			inputs.EatAction(InputMapping::Action::MENU_P1_LEFT);
		}

		if (inputs.Actions.find(InputMapping::Action::MENU_P2_RIGHT) != inputs.Actions.end())
		{
			inputs.EatAction(InputMapping::Action::MENU_P2_RIGHT);
		}

		if (inputs.Actions.find(InputMapping::Action::MENU_P2_LEFT) != inputs.Actions.end())
		{
			inputs.EatAction(InputMapping::Action::MENU_P2_LEFT);
		}
	}

	void MenuState::Update(GameEngine* game)
	{
		if (isEnd) game->Quit();
		// FMOD updates automatically at end
	}

	void MenuState::Draw(GameEngine* game)
	{
		//menu->draw();
		// Fizzle swaps buffer automatically at end
	}
}

