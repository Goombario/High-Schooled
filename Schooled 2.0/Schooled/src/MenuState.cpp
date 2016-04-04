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
		GameEngine::getMapper()->AddCallback(mainCallback, -2);

		// Hold valid keys
		shared::initValidKeys(validKeys);

		// Hold pressed keys
		shared::initPreviouslyPressed(previouslyPressed, validKeys);

		mainMenu = new Menu::MainMenu();
		p1CharMenu = new Menu::CharMenu(Side::LEFT);
		p2CharMenu = new Menu::CharMenu(Side::RIGHT);

		currentState = State::MAIN_MENU;
	}

	void MenuState::Cleanup()
	{
		// Cleanup all pointers
		delete mainMenu;
		delete p1CharMenu;
		delete p2CharMenu;

		mainMenu = nullptr;
		p1CharMenu = nullptr;
		p2CharMenu = nullptr;

		currentState = State::MAIN_MENU;
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
		}

		if (inputs.Actions.find(InputMapping::Action::MENU_P1_UP) != inputs.Actions.end() ||
			inputs.Actions.find(InputMapping::Action::MENU_P2_UP) != inputs.Actions.end())
		{
			switch (self->getCurrentState())
			{
			case State::MAIN_MENU:
				self->mainMenu->moveSelectionNum(-1);
				break;
			default:
				break;
			}
		}

		if (inputs.Actions.find(InputMapping::Action::MENU_P1_DOWN) != inputs.Actions.end() ||
			inputs.Actions.find(InputMapping::Action::MENU_P2_DOWN) != inputs.Actions.end())
		{
			switch (self->getCurrentState())
			{
			case State::MAIN_MENU:
				self->mainMenu->moveSelectionNum(1);
				break;
			default:
				break;
			}
		}

		if (inputs.Actions.find(InputMapping::Action::MENU_P1_SELECT) != inputs.Actions.end())
		{
			switch (self->getCurrentState())
			{
				// If the main menu, do this
			case State::MAIN_MENU:
				switch (self->mainMenu->getSelectionNum())
				{
				case 0:
					self->changeMenuState(State::CHAR_MENU);
					break;
				case 1:
					break;
				case 2:
					self->isEnd = true;
					return;
					break;
				default:
					break;
				}
				break;

				// If the character select menu, do this
			case State::CHAR_MENU:
				self->p1CharMenu->setFinished();
				break;
			case State::STAGE_MENU:
				break;
			default:
				break;
			}
		}

		if (inputs.Actions.find(InputMapping::Action::MENU_P2_SELECT) != inputs.Actions.end())
		{
			switch (self->getCurrentState())
			{
			case State::MAIN_MENU:
				switch (self->mainMenu->getSelectionNum())
				{
				case 0:
					// Change to the character select and stop taking input
					self->changeMenuState(State::CHAR_MENU);
					return;
					break;
				case 1:
					break;
				case 2:
					self->isEnd = true;
					return;
					break;
				default:
					break;
				}
				break;
			case State::CHAR_MENU:
				self->p2CharMenu->setFinished();
				break;
			case State::STAGE_MENU:
				break;
			default:
				break;
			}
		}

		if (inputs.Actions.find(InputMapping::Action::MENU_P1_BACK) != inputs.Actions.end())
		{
			switch (self->getCurrentState())
			{
			case State::MAIN_MENU:
				break;
			case State::CHAR_MENU:
				// Deselect the character, or return to main menu
				if (self->p1CharMenu->isFinished())
				{
					self->p1CharMenu->setFinished(false);
				}
				else
				{
					self->changeMenuState(State::MAIN_MENU);
					return;
				}
				break;
			case State::STAGE_MENU:
				// Change to the character menu and stop taking input
				self->changeMenuState(State::CHAR_MENU);
				return;
				break;
			default:
				break;
			}
		}

		if (inputs.Actions.find(InputMapping::Action::MENU_P2_BACK) != inputs.Actions.end())
		{
			switch (self->getCurrentState())
			{
			case State::MAIN_MENU:
				break;
			case State::CHAR_MENU:
				if (self->p2CharMenu->isFinished())
				{
					self->p2CharMenu->setFinished(false);
				}
				else
				{
					self->changeMenuState(State::MAIN_MENU);
					return;
				}
				break;
			case State::STAGE_MENU:
				// Change to the character menu and stop taking input
				self->changeMenuState(State::CHAR_MENU);
				return;
				break;
			default:
				break;
			}
		}

		if (inputs.Actions.find(InputMapping::Action::MENU_P1_RIGHT) != inputs.Actions.end())
		{
			switch (self->getCurrentState())
			{
			case State::MAIN_MENU:
				break;
			case State::CHAR_MENU:
				self->p1CharMenu->moveSelectionNum(1);
				break;
			case State::STAGE_MENU:
				
				break;
			default:
				break;
			}			
		}

		if (inputs.Actions.find(InputMapping::Action::MENU_P2_RIGHT) != inputs.Actions.end())
		{
			switch (self->getCurrentState())
			{
			case State::MAIN_MENU:
				break;
			case State::CHAR_MENU:
				self->p2CharMenu->moveSelectionNum(1);
				break;
			case State::STAGE_MENU:
				break;
			default:
				break;
			}
		}

		if (inputs.Actions.find(InputMapping::Action::MENU_P1_LEFT) != inputs.Actions.end())
		{
			switch (self->getCurrentState())
			{
			case State::MAIN_MENU:
				break;
			case State::CHAR_MENU:
				self->p1CharMenu->moveSelectionNum(-1);
				break;
			case State::STAGE_MENU:
				break;
			default:
				break;
			}
		}

		if (inputs.Actions.find(InputMapping::Action::MENU_P2_LEFT) != inputs.Actions.end())
		{
			switch (self->getCurrentState())
			{
			case State::MAIN_MENU:
				break;
			case State::CHAR_MENU:
				self->p2CharMenu->moveSelectionNum(-1);
				break;
			case State::STAGE_MENU:
				break;
			default:
				break;
			}
		}
	}

	void MenuState::Update(GameEngine* game)
	{
		if (isEnd) game->Quit();
		switch (getCurrentState())
		{
		case State::MAIN_MENU:
			mainMenu->update();
			break;
		case State::CHAR_MENU:
			p1CharMenu->update();
			p2CharMenu->update();
			break;
		case State::STAGE_MENU:
			break;
		default:
			break;
		}
		// FMOD updates automatically at end
	}

	void MenuState::Draw(GameEngine* game)
	{
		switch (getCurrentState())
		{
		case State::MAIN_MENU:
			mainMenu->draw();
			break;
		case State::CHAR_MENU:
			p1CharMenu->draw();
			p2CharMenu->draw();
			break;
		case State::STAGE_MENU:
			break;
		default:
			break;
		}
		// Fizzle swaps buffer automatically at end
	}

	void MenuState::changeMenuState(State s)
	{
		switch (getCurrentState())
		{
		case State::MAIN_MENU:
			mainMenu->reset();
			break;
		case State::CHAR_MENU:
			p1CharMenu->reset();
			p2CharMenu->reset();
			break;
		case State::STAGE_MENU:
			break;
		default:
			break;
		}

		currentState = s;
	}
}

