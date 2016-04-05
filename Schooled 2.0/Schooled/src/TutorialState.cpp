#include "TutorialState.h"
#include "BattleState.h"
#include "GameEngine.h"
#include "ShareState.h"
#include "Fizzle\Fizzle.h"
#include "Input\InputMapper.h"

#include "Sprite.h"
#include "Tutorial.h"

using std::string;

namespace TutorialState
{
	TutorialState TutorialState::m_TutorialState;

	void TutorialState::Init()
	{
		// Initialize the mapper context
		// Tells the mapper to map a specific set of keys to a specific set of actions
		GameEngine::getMapper()->PushContext("globalContext");
		GameEngine::getMapper()->PushContext("altTutorial");
		//GameEngine::getMapper()->PushContext("tutorial");

		// Tells the mapper to call the given function after the contexts have been mapped.
		GameEngine::getMapper()->AddCallback(mainCallback, -1);

		// Hold valid keys
		shared::initValidKeys(validKeys);

		// Hold pressed keys
		shared::initPreviouslyPressed(previouslyPressed, validKeys);

		isEnd = false;
		background = new Sprite::Sprite();
		p1Tut = new Tutorial::Tutorial("Nate", Side::LEFT);
		p2Tut = new Tutorial::Tutorial("Nega", Side::RIGHT);
	}

	void TutorialState::Cleanup()
	{
		// Cleanup all pointers
		delete background;
		delete p1Tut;
		delete p2Tut;

		// Pop the contexts
		GameEngine::getMapper()->PopContext();
		GameEngine::getMapper()->PopContext();
	}

	void TutorialState::Pause()
	{
		// Suspend sounds and potentially pop contexts
		GameEngine::getMapper()->PopContext();
		GameEngine::getMapper()->PopContext();
	}

	void TutorialState::Resume()
	{
		// Resume sounds and push contexts
		GameEngine::getMapper()->PushContext("globalContext");
		GameEngine::getMapper()->PushContext("altTutorial");
	}

	void TutorialState::HandleEvents(GameEngine* game)
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
		TutorialState *self = TutorialState::Instance();

		if (inputs.Actions.find(InputMapping::Action::EXIT_GAME) != inputs.Actions.end())
		{
			self->isEnd = true;
		}

		if (inputs.Actions.find(InputMapping::Action::P1_ADVANCE) != inputs.Actions.end())
		{
			self->p1Tut->moveSlide(1);
			inputs.EatAction(InputMapping::Action::P1_ADVANCE);
		}

		if (inputs.Actions.find(InputMapping::Action::P2_ADVANCE) != inputs.Actions.end())
		{
			self->p2Tut->moveSlide(1);
			inputs.EatAction(InputMapping::Action::P2_ADVANCE);
		}

		if (inputs.Actions.find(InputMapping::Action::P1_BACK) != inputs.Actions.end())
		{
			self->p1Tut->moveSlide(-1);
			self->p1Tut->setReady(false);
			inputs.EatAction(InputMapping::Action::P1_BACK);
		}

		if (inputs.Actions.find(InputMapping::Action::P2_BACK) != inputs.Actions.end())
		{
			self->p2Tut->moveSlide(-1);
			self->p2Tut->setReady(false);
			inputs.EatAction(InputMapping::Action::P2_BACK);
		}

		if (inputs.Actions.find(InputMapping::Action::P1_READY) != inputs.Actions.end())
		{
			self->p1Tut->setReady();
			inputs.EatAction(InputMapping::Action::P1_READY);
		}

		if (inputs.Actions.find(InputMapping::Action::P2_READY) != inputs.Actions.end())
		{
			self->p2Tut->setReady();
			inputs.EatAction(InputMapping::Action::P2_READY);
		}
	}

	void TutorialState::Update(GameEngine* game)
	{
		if (isEnd)
		{
			game->Quit();
		}

		p1Tut->update();
		p2Tut->update();

		if (p1Tut->isReady() && p2Tut->isReady())
		{
			game->PopState();
		}
		// FMOD updates automatically at end
	}

	void TutorialState::Draw(GameEngine* game)
	{
		//background->draw();
		p1Tut->draw();
		p2Tut->draw();
		// Fizzle swaps buffer automatically at end
	}
}

