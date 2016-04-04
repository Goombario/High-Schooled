//Copyright Fissure Studios 2016

#define NDEBUG
#ifdef WIN32

#define WIN32_LEAN_AND_MEAN

#endif

#include "GameEngine.h"
#include "BattleState.h"
#include "TutorialState.h"
#include "MenuState.h"

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

	// Load the first state
	game.ChangeState(BattleState::BattleState::Instance());
	game.PushState(TutorialState::TutorialState::Instance());

	// Main game loop
	while (game.Running())
	{
		game.HandleEvents();
		game.Update();
		game.Draw();
	}

	// cleanup the engine
	game.Cleanup();

	return 0;
}