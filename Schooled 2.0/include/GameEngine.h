#ifndef GAMEENGINE_H
#define GAMEENGINE_H

#include <vector>

// Forward Declarations
namespace Image
{
	class ImageManager;
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
	inline static Image::ImageManager* getImageManager() { return imageManager; }
private:
	// The stack of states
	std::vector <GameState*> states;

	// Represents if the program is running or not
	bool m_running;

	// Input Mapper
	static Image::ImageManager *imageManager;
};


#endif