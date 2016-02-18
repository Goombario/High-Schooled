#include "ExploreState.h"
#include "GameEngine.h"
#include "ShareState.h"
#include "Fizzle\Fizzle.h"
#include "Input\InputMapper.h"
#include "Level.h"
#include "Schooled.h"
#include "Character.h"

using std::string;

namespace ExploreState
{
	ExploreState ExploreState::m_ExploreState;

	void ExploreState::Init()
	{
		// Initialize the mapper context
		// Tells the mapper to map a specific set of keys to a specific set of actions
		GameEngine::getMapper()->PushContext(L"exploreContext");

		// Tells the mapper to call the given function after the contexts have been mapped.
		GameEngine::getMapper()->AddCallback(mainCallback, 0);

		// Hold valid keys
		shared::initValidKeys(validKeys);

		// Hold pressed keys
		shared::initPreviouslyPressed(previouslyPressed, validKeys);

		string resPath = schooled::getResourcePath("levels");
		testLevel = Level::Level(resPath + "Level1.tmx", resPath + "Level1.xml");

		FzlSpriteHandle playerHandle = FzlLoadSprite("C:/Hg/schooled-2.0/Schooled 2.0/res/img/battle_idle_spritesheet.png", 64, 64);
		Sprite::AnimatedSprite playerS(playerHandle, 64, 64, 1, 1);
		player = Character::Character(playerS);
	}

	void ExploreState::Cleanup()
	{
		// Cleanup all pointers
	}

	void ExploreState::Pause()
	{
		// Suspend sounds and potentially pop contexts
	}

	void ExploreState::Resume()
	{
		// Resume sounds and push contexts
	}

	void ExploreState::HandleEvents(GameEngine* game)
	{
		// Go through all valid keys and set if they are down or not, and if they have been previously down.
		for (FzlKey key : validKeys)
		{
			if (FzlGetKey(key))
			{
				bool previouslyDown = previouslyPressed[key];
				previouslyPressed[key] = true;
				game->getMapper()->SetRawButtonState(key, true, previouslyDown);
			}
			else
			{
				previouslyPressed[key] = false;
				game->getMapper()->SetRawButtonState(key, false, true);
			}
		}
		// Mapper dispatches automatically at end
	}

	void mainCallback(InputMapping::MappedInput& inputs)
	{
		ExploreState *self = ExploreState::Instance();
	}

	void ExploreState::Update(GameEngine* game)
	{

		// FMOD updates automatically at end
	}

	void ExploreState::Draw(GameEngine* game)
	{
		testLevel.draw();
		player.draw();
		// Fizzle swaps buffer automatically at end
	}
}

