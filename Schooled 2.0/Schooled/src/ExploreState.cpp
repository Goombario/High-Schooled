#include "ExploreState.h"
#include "GameEngine.h"
#include "ShareState.h"
#include "Fizzle\Fizzle.h"
#include "Input\InputMapper.h"

#include "Image.h"
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
		GameEngine::getMapper()->PushContext("globalContext");

		// Tells the mapper to call the given function after the contexts have been mapped.
		GameEngine::getMapper()->AddCallback(mainCallback, 0);

		// Hold valid keys
		shared::initValidKeys(validKeys);

		// Hold pressed keys
		shared::initPreviouslyPressed(previouslyPressed, validKeys);

		// Load a level
		string resPath = schooled::getResourcePath("levels");
		testLevel = Level::Level(resPath + "Level1.tmx", resPath + "Level1.xml");

		// Set up the player character
		Image::Image playerImage = GameEngine::getImageManager()->
			loadImage("C:/Hg/schooled-2.0/Schooled 2.0/res/img/battle_base_spritesheet.png", 64, 64);
		Animation::AnimationData playerSheet("PlayerAnimation.xml", 6);
		Sprite::AnimatedSprite playerS(playerImage, playerSheet);
		playerS.pushAnimation(Animation::AnimationEnum::ATTACK_1);
		player = Character::Character(playerS);
		player.moveSprite(300, 300);
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

		self = nullptr;
	}

	void ExploreState::Update(GameEngine* game)
	{
		player.update();
		// FMOD updates automatically at end
	}

	void ExploreState::Draw(GameEngine* game)
	{
		testLevel.draw();
		player.draw();
		// Fizzle swaps buffer automatically at end
	}
}

