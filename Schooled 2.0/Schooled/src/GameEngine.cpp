#include "GameEngine.h"
#include "GameState.h"
#include "Schooled.h"
#include "Input\InputMapper.h"
#include "Image.h"

#include "fmod_studio.hpp"
#include "fmod.hpp"
#include "Sound.h"

#include "Fizzle\Fizzle.h"

#include <ctime>

using namespace FMOD_util;

FMOD::Studio::System *GameEngine::system = nullptr;
InputMapping::InputMapper *GameEngine::mapper = nullptr;
Image::ImageManager *GameEngine::imageManager = nullptr;

int GameEngine::Init()
{
	// Set the seed
	srand((unsigned int)time(0));

	// Initialize FMOD
	if (Init_FMOD() != 0)
	{
		return 1;
	}

	// Initialize Fizzle
	FzlInit("High Schooled", static_cast<int>(schooled::SCREEN_WIDTH_PX * schooled::SCALE),
		static_cast<int>(schooled::SCREEN_HEIGHT_PX * schooled::SCALE), 1);
	FzlSetFrameRate(schooled::FRAMERATE);

	// Swaps buffers to set delta time between frames
	FzlSwapBuffers();

	// Initialize classes
	mapper = new InputMapping::InputMapper();
	imageManager = new Image::ImageManager();

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
	std::string bankPath = schooled::getResourcePath() + "../FMOD_Schooled/Build/Desktop/";
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

	masterBank = nullptr;
	stringsBank = nullptr;
	SFXBank = nullptr;
	tracksBank = nullptr;

	system->release();
	system = nullptr;

	// Delete classes
	delete mapper;
	mapper = nullptr;
	delete imageManager;
	imageManager = nullptr;

	// Cleanup Fizzle
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

	// push the handled inputs to the mapper
	mapper->Dispatch();
	mapper->Clear();
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

	// Draw Fizzle
	FzlSwapBuffers();
}