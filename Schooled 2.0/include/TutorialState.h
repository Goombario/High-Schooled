#ifndef TUTORIAL_STATE_H
#define TUTORIAL_STATE_H

#include "GameState.h"
#include "Fizzle\DataTypes\FizzleDataTypes.h"
#include <map>
#include <vector>

// Forward Declaration
namespace InputMapping
{
	struct MappedInput;
}

namespace Sprite
{
	class Sprite;
}

namespace Tutorial
{
	class Tutorial;
}

namespace TutorialState
{
	class TutorialState : public GameState
	{
	public:
		// Initialize the state
		void Init();

		// Cleanup the state before exiting
		void Cleanup();

		// Pause or resume the state before/after changing
		void Pause();
		void Resume();

		// Operations done by the state, can call to the game engine
		void HandleEvents(GameEngine* game);
		void Update(GameEngine* game);
		void Draw(GameEngine* game);

		// Return an instance of the object
		static TutorialState* Instance() {
			return &m_TutorialState;
		}

		// The callback used in the state
		friend void mainCallback(InputMapping::MappedInput& inputs);

	protected:
		// Create an empty object
		TutorialState() { }

	private:
		static TutorialState m_TutorialState;
		std::vector<FzlKey> validKeys;
		std::map<FzlKey, bool> previouslyPressed;
		
		Sprite::Sprite *background;
		Tutorial::Tutorial *p1Tut, *p2Tut;
		bool isEnd;
		
	};
}

#endif