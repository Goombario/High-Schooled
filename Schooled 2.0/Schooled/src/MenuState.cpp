#include "MenuState.h"
#include "GameState.h"
#include "PlayingState.h"
#include "sound_effects.h"
#include "ShareState.h"

#include "Fizzle\Fizzle.h"

#include <fstream>
#include <stdio.h>
#include <iostream>

using schooled::getSetting;
using schooled::controlOptions;

namespace MenuState
{

	MenuState MenuState::m_MenuState;
	int MenuState::lSelect = 0;

	// FMOD sounds
	namespace snd
	{
		// Title Screen Music
		Sound::Description *m_title_desc = nullptr;
		Sound::Instance *m_title = nullptr;

		// Dungeon Music
		Sound::Description *m_dungeon_desc = nullptr;
		Sound::Instance *m_dungeon = nullptr;

		// Sound effects
		Sound::Description *s_start_desc = nullptr;

	}

	void MenuState::Init()
	{
		// Get the Title Screen Music
		//snd::m_title_desc = new Sound::Description("Tracks/m_title");
		//snd::m_title = new Sound::Instance(snd::m_title_desc);

		// Get the Dungeon Music
		//snd::m_dungeon_desc = new Sound::Description("Tracks/m_dungeon");
		//snd::m_dungeon = new Sound::Instance(snd::m_dungeon_desc);

		// Get the sound effects
		//snd::s_start_desc = new Sound::Description("SFX/s_start", true);


		// Set and clear the buffer
		HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
		buffer.open(hConsole);
		buffer.clear();
		buffer.close(hConsole);

		// Get the artwork and intro text
		art = getTextBlock(schooled::getResourcePath("text") + "title.txt");
		introText = getTextBlock(schooled::getResourcePath("text") + "intro.txt");

		// Check if settings file exists.
		if (!std::ifstream("Settings.txt"))
		{
			initSettings();
		}

		//Play the music
		snd::m_title->start();

		// Set the default location of the selection to "Start Game"
		menuSelect = 0;

		// Set the menu options
		menuSelections.push_back("Start Game");
		menuSelections.push_back("Control Options");
		menuSelections.push_back("Level Select");
		menuSelections.push_back("Credits");
		menuSelections.push_back("Quit");

		// Load schemes
		setSchemes();

		// Set flags
		selectingControl = false;
		selectingLevel = false;
		changedSettings = false;
		selectingCredits = false;
		startingGame = false;
		showObjective = false;

		// Get the level list
		levelSelections = shared::getRoomNames();

		// Get the control from the settings file
		selectedControl = 0;
		string sControl = schooled::getSetting("ControlScheme");

		for (unsigned int i = 0; i < controlOptions.size(); i++)
		{
			if (controlOptions[i] == sControl)
			{
				selectedControl = i;
				break;
			}
		}
	}

	void MenuState::Cleanup()
	{
		// Stop the main music
		snd::m_title->stop(FMOD_STUDIO_STOP_IMMEDIATE);
	}

	void MenuState::Pause()
	{
		// Stop the main music and if settings changed, save settings
		snd::m_title->stop();
		if (changedSettings)
		{
			saveSetting("ControlScheme", controlOptions[selectedControl]);
		}
	}

	void MenuState::Resume()
	{
		// Play the main music
		snd::m_title->start();
	}

	void MenuState::HandleEvents(GameEngine* game)
	{
		// SHOULD BE IN UPDATE
		// If the game is starting and the controls are being shown
		if (startingGame)
		{
			startingGame = false;
			showObjective = true;
			return;
		}
		// Showing the objective
		else if (showObjective)
		{
			showObjective = false;
			snd::m_title->stop();

			Sound::Instance s_start(snd::s_start_desc);
			s_start.start();
			game->PushState(PlayingState::Instance());
			return;
		}


		//Escape key
		if (FzlGetKey(FzlKeyEscape))
		{
			// If not in submenu, quit
			if (!selectingControl && !selectingLevel && !selectingCredits)
			{
				game->Quit();
			}
		}
		else if (FzlGetKey(FzlKeyX) || FzlGetKey(FzlKeyN))
		{
			if (selectingControl)	// Controls menu
			{
				selectingControl = false;
				changedSettings = false;
			}
			else if (selectingLevel)	// level select menu
			{
				selectingLevel = false;
				lSelect = 0;
				levelSelect = 0;
			}
			else if (selectingCredits)	// credits menu
			{
				selectingCredits = false;
			}
			return;
		}
		// Going up
		else if (FzlGetKey(FzlKeyW) || FzlGetKey(FzlKeyUpArrow))
		{
			if (selectingControl) return; // Don't move if changing controls
			//snd::menuHighlight->play();

			if (selectingLevel)	// Choosing a level to play
			{
				// Change the level select highlight
				levelSelect = (levelSelect > 0) ? levelSelect - 1 : levelSelections.size() - 1;
				return;
			}
			// Change the main menu highlight
			menuSelect = (menuSelect > 0) ? menuSelect - 1 : menuSelections.size() - 1;
			return;
		}

		// Going down
		else if (FzlGetKey(FzlKeyS) || FzlGetKey(FzlKeyDownArrow))
		{
			if (selectingControl) return; // Don't move if changing controls
			//snd::menuHighlight->play();
			if (selectingLevel)	// Choosing a level to play
			{
				// Change the level select highlight
				levelSelect = (levelSelect < levelSelections.size() - 1) ? levelSelect + 1 : 0;
				return;
			}
			// Change the main menu highlight
			menuSelect = (menuSelect < menuSelections.size() - 1) ? menuSelect + 1 : 0;
		}
		// Going left
		else if (FzlGetKey(FzlKeyA) || FzlGetKey(FzlKeyLeftArrow))
		{
			if (!selectingControl) return;	// If not selecting controls, break
			//snd::menuHighlight->play();

			selectedControl = (selectedControl > 0) ? selectedControl - 1 : controlOptions.size() - 1;
		}

		// Going right
		else if (FzlGetKey(FzlKeyD) || FzlGetKey(FzlKeyRightArrow))
		{

			if (!selectingControl) return;	// If not selecting controls, break
			//snd::menuHighlight->play();

			selectedControl = (selectedControl < controlOptions.size() - 1) ? selectedControl + 1 : 0;
		}
		// "Enter" key
		else if (FzlGetKey(FzlKeyZ) || FzlGetKey(FzlKeyM)
			|| FzlGetKey(FzlKeyEnter) || FzlGetKey(FzlKeySpace))
		{
			handleMenu(game);
		}
	}

	void MenuState::Update(GameEngine* game)
	{
		if (showObjective)
		{
			snd::m_title->stop();
			snd::m_dungeon->start();
		}

	}

	void MenuState::Draw(GameEngine* game)
	{
		
	}

	string MenuState::getTextBlock(string filename)
	{
		string tempLine, fullLine;
		std::ifstream stream(filename);
		if (!stream)
		{
			std::cerr << "File failed to load (GetTextBlock)" << std::endl;
			exit(1);
		}

		while (getline(stream, tempLine))
		{
			fullLine += tempLine + "#";
		}

		stream.close();
		return fullLine;
	}

	void MenuState::handleMenu(GameEngine* game)
	{
		switch (menuSelect)
		{
		case 0:
			// Start game
			startingGame = true;
			break;

		case 1:
			// Control settings
			if (!selectingControl)
			{
				selectingControl = true;
			}
			else
			{
				selectingControl = false;
				changedSettings = true;
			}
			break;

		case 2:
			// Level selector
			if (!selectingLevel)
			{
				selectingLevel = true;
			}
			else
			{
				lSelect = levelSelect + 1;
				snd::m_title->stop();
				Sound::Instance s_start(snd::s_start_desc);
				s_start.start();
				game->PushState(PlayingState::Instance());
			}
			break;

		case 3:
			// Credits
			if (!selectingCredits)
			{
				selectingCredits = true;
			}
			else
			{
				selectingCredits = false;
			}
			break;

		case 4:
			// Quit game
			game->Quit();
			break;

		default:
			break;
		}
	}

	void MenuState::saveSetting(string a_key, string change)
	{
		// Open the files for input/output
		std::ifstream inStream("Settings.txt");
		std::ofstream outStream("temp.txt");
		string line;

		// If there is an error with the files, error
		if (!inStream || !outStream)
		{
			std::cerr << "File failed to load (SaveSetting)" << std::endl;
			exit(1);
		}

		// Check for changes and put the information in the other file
		while (std::getline(inStream, line))
		{
			if (line.substr(0, line.find(':')) == a_key)
			{
				line = a_key + ": " + change;
			}
			outStream << line << std::endl;
		}
		outStream.flush();

		inStream.close();
		outStream.close();

		// Error checking and renaming
		if (remove("Settings.txt") != 0)
			std::cerr << "Error deleting file" << std::endl;
		else
			puts("File successfully deleted");

		int result = rename("temp.txt", "Settings.txt");
		if (result == 0)
			puts("File successfully renamed");
		else
			std::cerr << "Error renaming file" << std::endl;
	}

	void MenuState::initSettings()
	{
		// Open the file for output
		std::ofstream stream("Settings.txt");
		if (!stream)
		{
			std::cerr << "File open failed (initSettings)." << std::endl;
			exit(1);
		}
		stream << "ControlScheme: Double-Tap" << std::endl;
		stream.close();
	}

	void MenuState::setSchemes()
	{
		// Classic scheme
		cScheme.push_back("FACE UP = UP");
		cScheme.push_back("FACE DOWN = DOWN");
		cScheme.push_back("FACE LEFT = LEFT");
		cScheme.push_back("FACE RIGHT = RIGHT");
		cScheme.push_back("MOVE = Z");
		cScheme.push_back("ATTACK = X");
		cScheme.push_back("INTERACT = SPACE");

		// Double-Tap scheme
		dScheme.push_back("FACE UP = UP");
		dScheme.push_back("FACE DOWN = DOWN");
		dScheme.push_back("FACE LEFT = LEFT");
		dScheme.push_back("FACE RIGHT = RIGHT");
		dScheme.push_back("MOVE = FACE KEYS (TAP AGAIN)");
		dScheme.push_back("ATTACK = X");
		dScheme.push_back("INTERACT = SPACE");

		// Classic lefty scheme
		clScheme.push_back("FACE UP = W");
		clScheme.push_back("FACE DOWN = S");
		clScheme.push_back("FACE LEFT = A");
		clScheme.push_back("FACE RIGHT = D");
		clScheme.push_back("MOVE = M");
		clScheme.push_back("ATTACK = N");
		clScheme.push_back("INTERACT = SPACE");

		// Double-Tap lefty scheme
		dlScheme.push_back("FACE UP = W");
		dlScheme.push_back("FACE DOWN = S");
		dlScheme.push_back("FACE LEFT = A");
		dlScheme.push_back("FACE RIGHT = D");
		dlScheme.push_back("MOVE = FACE KEYS (TAP AGAIN)");
		dlScheme.push_back("ATTACK = N");
		dlScheme.push_back("INTERACT = SPACE");
	}
}