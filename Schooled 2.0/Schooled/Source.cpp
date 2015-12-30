//(C) Fissure Studios 2016

#include "GameEngine.h"
#include "MenuState.h"
#include <ctime>

#define NDEBUG
#define WIN32_LEAN_AND_MEAN

int main(int argc, char **argv)
{
	GameEngine game;

	// Initialize the game
	int init = game.Init();
	if (init != 0)
	{
		game.Cleanup();
		return init;
	}

	// Set the seed
	srand((unsigned int)time(0));

	// Load the main menu
	game.ChangeState(MenuState::Instance());

	// Main game loop
	while (game.Running())
	{
		game.Update();
		game.Draw();
		game.HandleEvents();
	}

	// cleanup the engine
	game.Cleanup();

	return 0;
}