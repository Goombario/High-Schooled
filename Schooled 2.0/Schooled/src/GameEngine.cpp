#include "GameEngine.h"
#include "GameState.h"
#include "Room.h"
#include "Schooled.h"

#include "FMOD_util.h"
#include "fmod_studio.hpp"
#include "fmod.hpp"

#include "Fizzle\Fizzle.h"

using namespace FMOD_util;

FMOD::Studio::System *GameEngine::system = nullptr;

int GameEngine::Init()
{
	if (Init_FMOD() != 0)
	{
		return 1;
	}

	FzlInit("Schooled 2.0", 640, 480, 0);
	FzlSetFrameRate(60);

	// Load the room indices
	if (Room::loadTileIndex("tileIndex.txt") != 0 ||
		Room::loadItemIndex("itemIndex.txt") != 0 ||
		Room::loadActorIndex("actorIndex.txt") != 0)
	{
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
	string bankPath = schooled::getResourcePath() + "../FMOD_Schooled/Build/Desktop/";
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

	// Cleanup FMOD
	masterBank->unload();
	stringsBank->unload();
	SFXBank->unload();
	tracksBank->unload();

	system->release();

	FzlDestroy();
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

	// Update FMOD
	system->update();
	
}

void GameEngine::Draw()
{
	// let the state draw the screen
	states.back()->Draw(this);
	FzlSwapBuffers();
}