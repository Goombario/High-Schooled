#include "GameEngine.h"
#include "GameState.h"
#include "Room.h"

#include "res_path.h"
#include "Cleanup.h"
#include "SDL_util.h"
#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>

using namespace SDL_util;

SDL_Renderer *GameEngine::renderer;
SDL_Window *GameEngine::window;

int GameEngine::Init()
{
	int const SCREEN_WIDTH = 640;
	int const SCREEN_HEIGHT = 480;
	//Start up SDL and make sure it went ok
	if (SDL_Init(SDL_INIT_VIDEO) != 0)
	{
		logSDLError(std::cout, "SDL_Init");
		return 1;
	}
	//Also need to init SDL_ttf (for text)
	if (TTF_Init() != 0)
	{
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

	//Setup our window and renderer
	window = SDL_CreateWindow("Schooled 2.0", SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	if (window == nullptr)
	{
		logSDLError(std::cout, "CreateWindow");
		TTF_Quit();
		IMG_Quit();
		SDL_Quit();
		return 1;
	}

	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (renderer == nullptr)
	{
		logSDLError(std::cout, "CreateRenderer");
		cleanup(window);
		TTF_Quit();
		IMG_Quit();
		SDL_Quit();
		return 1;
	}

	// Load the room indices
	Room::loadTileIndex("tileIndex.txt");
	Room::loadItemIndex("itemIndex.txt");
	Room::loadActorIndex("actorIndex.txt");

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
	cleanup(renderer, window);
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