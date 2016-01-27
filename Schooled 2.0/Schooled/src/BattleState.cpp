#include "BattleState.h"
#include "Fizzle\Fizzle.h"
#include "GameEngine.h"
#include "Input\InputMapper.h"

using std::string;

namespace BattleState
{
	BattleState BattleState::m_BattleState;

	void BattleState::Init()
	{
		// Initialize the mapper context
		// Tells the mapper to map a specific set of keys to a specific set of actions
		GameEngine::getMapper()->PushContext(L"mainContext");

		// Tells the mapper to call the given function after the contexts have been mapped.
		GameEngine::getMapper()->AddCallback(mainCallback, 0);
	}

	void BattleState::Cleanup()
	{
		// Cleanup all pointers
	}

	void BattleState::Pause()
	{
		// Suspend sounds and potentially pop contexts
	}

	void BattleState::Resume()
	{
		// Resume sounds and push contexts
	}

	void BattleState::HandleEvents(GameEngine* game)
	{

		// Mapper dispatches automatically at end
	}

	void BattleState::Update(GameEngine* game)
	{

		// FMOD updates automatically at end
	}

	void BattleState::Draw(GameEngine* game)
	{

		// Fizzle swaps buffer automatically at end
	}

	void mainCallback(InputMapping::MappedInput& inputs)
	{
		BattleState *self = BattleState::Instance();
	}
}

