#ifndef GAMEENGINE_H
#define GAMEENGINE_H

#include <vector>
using std::vector;

// Forward Declarations
namespace FMOD
{
	namespace Studio
	{
		class System;
		class Bank;
	}
}

namespace InputMapping
{
	class InputMapper;
}

class GameState;

// Class that controls the state switching engine among other things
class GameEngine
{
public:
	// Initialize the engine
	int Init();

	// Cleanup the engine before exiting
	void Cleanup();

	// Manipulate the states
	void ChangeState(GameState* state);
	void PushState(GameState* state);
	void PopState();

	// Tell the state to call its own functions
	void HandleEvents();
	void Update();
	void Draw();

	// Modify the state
	bool Running() { return m_running; }
	void Quit() { m_running = false; }

	// Return the input mapper for input mapping
	inline static InputMapping::InputMapper* getMapper() { return mapper; }

	inline static FMOD::Studio::System* getSoundSystem() { return system; }
private:
	// Private function that initializes FMOD
	int Init_FMOD();

	// The stack of states
	vector <GameState*> states;

	// Represents if the program is running or not
	bool m_running;

	// FMOD variables
	static FMOD::Studio::System *system;
	FMOD::Studio::Bank *masterBank = nullptr;
	FMOD::Studio::Bank *stringsBank = nullptr;
	FMOD::Studio::Bank *SFXBank = nullptr;
	FMOD::Studio::Bank *tracksBank = nullptr;

	// Input Mapper
	static InputMapping::InputMapper *mapper;
};


#endif