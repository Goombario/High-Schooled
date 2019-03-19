#include "GameEngine.h"
#include "GameState.h"
#include "Schooled.h"
#include "Image.h"

#include "Fizzle\Fizzle.h"

#include <ctime>

Image::ImageManager *GameEngine::imageManager = nullptr;

int GameEngine::Init()
{
	// Set the seed
	srand(0);

	// Initialize Fizzle
	FzlInit("High Schooled", static_cast<int>(schooled::SCREEN_WIDTH_PX * schooled::SCALE),
		static_cast<int>(schooled::SCREEN_HEIGHT_PX * schooled::SCALE), 0);
	FzlSetFrameRate(schooled::FRAMERATE);

	// Swaps buffers to set delta time between frames
	FzlSwapBuffers();

	// Initialize classes
	imageManager = new Image::ImageManager();

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

	// Delete classes
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

	// Draw Fizzle
	FzlSwapBuffers();
}