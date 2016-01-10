#include "GameEngine.h"
#include "GameState.h"
#include "Room.h"
#include "Schooled.h"

#include "res_path.h"
#include "SDL_util.h"
#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>
#include <Cleanup.h>

#include "FMOD_util.h"
#include "fmod_studio.hpp"
#include "fmod.hpp"

using namespace SDL_util;
using namespace FMOD_util;

SDL_Renderer *GameEngine::renderer;
SDL_Window *GameEngine::window;
FMOD::Studio::System *GameEngine::system = nullptr;

int GameEngine::Init()
{
	if (Init_SDL() != 0 || Init_FMOD() != 0)
	{
		return 1;
	}

	// Load the room indices
	if (Room::loadTileIndex("tileIndex.txt", renderer) != 0 ||
		Room::loadItemIndex("itemIndex.txt", renderer) != 0 ||
		Room::loadActorIndex("actorIndex.txt", renderer) != 0)
	{
		cleanup(window, renderer);
		TTF_Quit();
		IMG_Quit();
		SDL_Quit();
		return 1;
	}

	return 0;
}

int GameEngine::Init_SDL()
{
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
		SDL_WINDOWPOS_CENTERED, schooled::SCREEN_WIDTH_PX,
		schooled::SCREEN_HEIGHT_PX, SDL_WINDOW_SHOWN);
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
	return 0;
}

int GameEngine::Init_FMOD()
{
	FMOD_RESULT result;

	result = FMOD::Studio::System::create(&system); // Create the Studio System object.
	FMODErrorCheck(result);

	// Initialize FMOD Studio, which will also initialize FMOD Low Level
	result = system->initialize(512, FMOD_STUDIO_INIT_NORMAL, FMOD_INIT_NORMAL, 0);
	FMODErrorCheck(result);

	// Load the banks
	string bankPath = SDL_util::getResourcePath() + "../FMOD_Schooled/Build/Desktop/";
	FMOD::Studio::Bank *masterBank = nullptr;
	FMODErrorCheck(system->loadBankFile((bankPath + "Master Bank.bank").c_str(), FMOD_STUDIO_LOAD_BANK_NORMAL, &masterBank));

	FMOD::Studio::Bank *stringsBank = nullptr;
	FMODErrorCheck(system->loadBankFile((bankPath + "Master Bank.strings.bank").c_str(), FMOD_STUDIO_LOAD_BANK_NORMAL, &stringsBank));

	// Load additional banks
	FMOD::Studio::Bank *SFXBank = nullptr;
	FMODErrorCheck(system->loadBankFile((bankPath + "SFX.bank").c_str(), FMOD_STUDIO_LOAD_BANK_NORMAL, &SFXBank));

	FMOD::Studio::Bank *tracksBank = nullptr;
	FMODErrorCheck(system->loadBankFile((bankPath + "Tracks.bank").c_str(), FMOD_STUDIO_LOAD_BANK_NORMAL, &tracksBank));
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

	// Cleanup SDL
	cleanup(renderer, window);
	TTF_Quit();
	IMG_Quit();
	SDL_Quit();

	// Cleanup FMOD
	masterBank->unload();
	stringsBank->unload();
	SFXBank->unload();
	tracksBank->unload();

	system->release();
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