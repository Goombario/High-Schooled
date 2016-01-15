#ifndef GAMEENGINE_H
#define GAMEENGINE_H

#include <vector>
using std::vector;

namespace FMOD
{
	namespace Studio
	{
		class System;
		class Bank;
	}
}

class GameState;

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

	static FMOD::Studio::System* getSoundSystem() { return system; }
private:
	int Init_FMOD();
	// The stack of states
	vector <GameState*> states;
	bool m_running;

	static FMOD::Studio::System *system;
	FMOD::Studio::Bank *masterBank = nullptr;
	FMOD::Studio::Bank *stringsBank = nullptr;
	FMOD::Studio::Bank *SFXBank = nullptr;
	FMOD::Studio::Bank *tracksBank = nullptr;

};


#endif