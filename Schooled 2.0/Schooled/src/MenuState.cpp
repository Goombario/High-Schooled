#include "MenuState.h"
#include "GameState.h"
#include "PlayingState.h"
#include "Console_color.h"
#include "sound_effects.h"
#include "ShareState.h"

//#include <SDL.h>
#include <SDL_image.h>
#include <SDL_util.h>

#include <fstream>
#include <stdio.h>
using schooled::getSetting;
using schooled::controlOptions;
	
namespace con = JadedHoboConsole;

MenuState MenuState::m_MenuState;
int MenuState::lSelect = 0;

void MenuState::Init()
{
	// Set and clear the buffer
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	buffer.open(hConsole);
	buffer.clear();
	buffer.close(hConsole);

	// Get the artwork and intro text
	art = getTextBlock(SDL_util::getResourcePath("text") + "title.txt");
	introText = getTextBlock(SDL_util::getResourcePath("text") + "intro.txt");



	// Check if settings file exists.
	if (!std::ifstream("Settings.txt"))
	{
		initSettings();
	}

	//Play the music
	snd::title->play();

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
	snd::title->stop();
	//cleanup(logo);
}

void MenuState::Pause()
{
	// Stop the main music and if settings changed, save settings
	snd::title->stop();
	if (changedSettings)
	{
		saveSetting("ControlScheme", controlOptions[selectedControl]);
	}
}

void MenuState::Resume()
{
	// Play the main music
	snd::title->play();
}

void MenuState::HandleEvents(GameEngine* game)
{

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
		//snd::title->stop();
		snd::startGame->play();
		game->PushState(PlayingState::Instance());
		return;
	}

	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		if (event.type == SDL_QUIT){
			game->Quit();
		}
		else if (event.type == SDL_KEYDOWN){
			switch (event.key.keysym.scancode)
			{
				//Escape key
			case SDL_SCANCODE_ESCAPE:
				// If not in submenu, quit
				if (!selectingControl && !selectingLevel && !selectingCredits)
				{
					game->Quit();
				}
				// No break so it can be used as a "back" key

			case SDL_SCANCODE_X:
			case SDL_SCANCODE_N:
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
				break;

				// Going up
			case SDL_SCANCODE_W:
			case SDL_SCANCODE_UP:
				if (selectingControl) break; // Don't move if changing controls
				//snd::menuHighlight->play();

				if (selectingLevel)	// Choosing a level to play
				{
					// Change the level select highlight
					levelSelect = (levelSelect > 0) ? levelSelect - 1 : levelSelections.size() - 1;
					break;
				}
				// Change the main menu highlight
				menuSelect = (menuSelect > 0) ? menuSelect - 1 : menuSelections.size() - 1;
				break;

				// Going down
			case SDL_SCANCODE_DOWN:
			case SDL_SCANCODE_S:
				if (selectingControl) break; // Don't move if changing controls
				//snd::menuHighlight->play();
				if (selectingLevel)	// Choosing a level to play
				{
					// Change the level select highlight
					levelSelect = (levelSelect < levelSelections.size() - 1) ? levelSelect + 1 : 0;
					break;
				}
				// Change the main menu highlight
				menuSelect = (menuSelect < menuSelections.size() - 1) ? menuSelect + 1 : 0;
				break;

				// Going left
			case SDL_SCANCODE_LEFT:
			case SDL_SCANCODE_A:
				if (!selectingControl) break;	// If not selecting controls, break
				//snd::menuHighlight->play();

				selectedControl = (selectedControl > 0) ? selectedControl - 1 : controlOptions.size() - 1;
				break;

				// Going right
			case SDL_SCANCODE_RIGHT:
			case SDL_SCANCODE_D:
				if (!selectingControl) break;	// If not selecting controls, break
				//snd::menuHighlight->play();

				selectedControl = (selectedControl < controlOptions.size() - 1) ? selectedControl + 1 : 0;
				break;

				// "Enter" key
			case SDL_SCANCODE_RETURN:
			case SDL_SCANCODE_Z:
			case SDL_SCANCODE_M:
			case SDL_SCANCODE_SPACE:
				handleMenu(game);
				break;
			default:
				break;
			}
		}
	}
}

void MenuState::Update(GameEngine* game)
{
	if (showObjective)
	{
		snd::title->stop();
		snd::dungeonMusic->play();
	}

}

void MenuState::Draw(GameEngine* game)
{
	SDL_Renderer *ren = game->getRenderer();
	SDL_Window *win = game->getWindow();



	// Get the current handle
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	
	// Open the buffer for writing
	buffer.open(hConsole);

	// Clear the buffer
	buffer.clear();

	if (showObjective)
	{
		// Show the objective
		buffer.draw(introText, con::fgHiWhite, 5, 0);

		// Draw the "Press any key to continue"
		string temp = "Press any key to continue";
		buffer.draw(temp, con::fgHiCyan, 21, (30 - temp.size() / 2));
	}
	else if (startingGame)
	{
		int row = 4;
		int col;
		vector<string> tempScheme;
		string temp = "Press any key to continue";
		// Draw the "Press any key to continue"
		buffer.draw(temp, con::fgHiCyan, 21, (30 - temp.size() / 2));

		// Choose the correct scheme to display
		switch (selectedControl)
		{
		case 0:
			tempScheme = cScheme;
			break;
		case 1:
			tempScheme = dScheme;
			break;
		case 2:
			tempScheme = clScheme;
			break;
		case 3:
			tempScheme = dlScheme;
			break;
		}
		for (string s : tempScheme)
		{
			col = 30 - s.size() / 2;
			buffer.draw(s, con::fgHiWhite, row, col);
			row += 2;
		}
	}
	// Level select screen
	else if (selectingLevel)
	{
		buffer.draw("Choose a level (ESC to go back)", con::fgHiWhite, 1, 20);
		int row = 3;
		WORD colour;
		int col;
		for (unsigned int i = 0; i < levelSelections.size(); i++)
		{
			col = 30 - levelSelections[i].size() / 2;	// Centering the text

			// Determine the colour of the text
			if (levelSelect == i)
			{
				colour = con::fgHiWhite;
			}
			else
			{
				colour = con::fgGray;
			}
			buffer.draw(levelSelections[i], colour, row, col);
			row ++;
		}
	}
	// Choosing the control scheme and show the actual changes
	else if (selectingControl)
	{
		int row = 4;
		int col;
		string temp = "<- " + controlOptions[selectedControl] + " ->";
		vector<string> tempScheme;

		// Draw the selected control scheme
		buffer.draw(temp, con::fgHiWhite, 21, (30 - temp.size() / 2));
		
		// Choose the correct scheme to display
		switch (selectedControl)
		{
		case 0:
			tempScheme = cScheme;
			break;
		case 1:
			tempScheme = dScheme;
			break;
		case 2:
			tempScheme = clScheme;
			break;
		case 3:
			tempScheme = dlScheme;
			break;
		}
		for (string s : tempScheme)
		{
			col = 30 - s.size() / 2;
			buffer.draw(s, con::fgHiWhite, row, col);
			row += 2;
		}
	}
	else if (selectingCredits)
	{
		// Display the credits
		string temp = "Robbie Savaglio\nRebecca Joly\nSam \"Lili\" Bouffard\nPavlo Salimon\nGraham Watson";
		buffer.draw(art, con::fgHiWhite, 1, 3);
		buffer.draw(temp, con::fgHiMagenta, 16, 23);
		buffer.draw("PRESS ESC TO RETURN", con::fgHiWhite, 22, 23);
	}
	else
	{
		// Draw the main menu
		buffer.draw(art, con::fgHiYellow, 1, 3);

		// Draw the menu options to the screen
		int row = 16;
		WORD colour;
		int col;
		for (unsigned int i = 0; i < menuSelections.size(); i++)
		{
			col = 30 - menuSelections[i].size() / 2; // Centering the text

			// Determine the colour of the text
			if (menuSelect == i)
			{
				colour = con::fgHiWhite;
			}
			else
			{
				colour = con::fgGray;
			}
			buffer.draw(menuSelections[i], colour, row, col);
			row += 2;
		}
	}
	
	// Close the buffer
	buffer.close(hConsole);
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
			lSelect = levelSelect+1;
			snd::title->stop();
			snd::startGame->play();
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