#include "GameEngine.h"
#include "GameState.h"
#include "Console.h"

#include "res_path.h"
#include "Cleanup.h"
#include "SDL_util.h"
#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>

using namespace SDL_util;

int GameEngine::Init()
{
	//Start up SDL and make sure it went ok
	if (SDL_Init(SDL_INIT_VIDEO) != 0){
		logSDLError(std::cout, "SDL_Init");
		return 1;
	}
	//Also need to init SDL_ttf
	if (TTF_Init() != 0){
		logSDLError(std::cout, "TTF_Init");
		SDL_Quit();
		return 1;
	}

	// Check if SDL_Image initialized
	if ((IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG) != IMG_INIT_PNG)
	{
		logSDLError(std::cout, "IMG_Init");
		TTF_Quit();
		SDL_Quit();
		return 1;
	}
	return 0;
}

void GameEngine::Cleanup()
{
	// Clean up all states
	while (!states.empty())
	{
		states.back()->Cleanup();
		states.pop_back();
	}

	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
}

void GameEngine::ChangeState(GameState* state)
{
	// Clean up the current state
	if (!states.empty()) 
	{
		states.back()->Cleanup();
		states.pop_back();
	}

	// store and init the new state
	states.push_back(state);
	states.back()->Init();
}

void GameEngine::PushState(GameState* state)
{
	// pause current state
	if (!states.empty())
	{
		states.back()->Pause();
	}

	// store and init the new state
	states.push_back(state);
	states.back()->Init();
}

void GameEngine::PopState()
{
	// cleanup the current state
	if (!states.empty())
	{
		states.back()->Cleanup();
		states.pop_back();
	}

	// resume previous state
	if (!states.empty()) 
	{
		states.back()->Resume();
	}
}

void GameEngine::HandleEvents()
{
	// let the state handle events
	states.back()->HandleEvents(this);
}

void GameEngine::Update()
{
	// let the state update the game
	states.back()->Update(this);
}

void GameEngine::Draw()
{
	// let the state draw the screen
	states.back()->Draw(this);
}