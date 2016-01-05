#ifndef GAMEENGINE_H
#define GAMEENGINE_H

#include <vector>
using std::vector;

struct SDL_Window;
struct SDL_Renderer;

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

	static SDL_Renderer* getRenderer() { return renderer; }
	static SDL_Window* getWindow() { return window; }
private:
	// The stack of states
	vector <GameState*> states;
	bool m_running;
	static SDL_Window *window;
	static SDL_Renderer *renderer;
};


#endif