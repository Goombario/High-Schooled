#include "WinState.h"
#include <string>
#include <fstream>
#include "Console_color.h"
#include "Fizzle\Fizzle.h"

using std::string;

namespace con = JadedHoboConsole;

WinState WinState::m_WinState;

void WinState::Init()
{
	finished = false;
	winText = getTextBlock("outro.txt");
}

void WinState::Cleanup()
{

}

void WinState::Pause()
{

}

void WinState::Resume()
{

}

void WinState::HandleEvents(GameEngine* game)
{
	if (FzlGetKey(FzlKeyEnter))
	{
		finished = true;
	}
}

void WinState::Update(GameEngine* game)
{
	if (finished)
	{
		game->PopState();
	}
}

void WinState::Draw(GameEngine* game)
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	// Open the buffer for writing
	buffer.open(hConsole);

	buffer.clear();

	// Draw congratulations text
	buffer.draw(winText, con::fgHiWhite, 0, 0);
	
	string temp = "Press any key to return to menu";
	int tempCol = 30 - temp.length() / 2;
	buffer.draw(temp, con::fgHiCyan, 15, tempCol);

	buffer.close(hConsole);
	
}

string WinState::getTextBlock(string filename)
{
	string tempLine, fullLine;
	std::ifstream stream(filename);
	if (!stream)
	{
		std::cerr << "File failed to load" << std::endl;
		exit(1);
	}

	while (getline(stream, tempLine))
	{
		fullLine += tempLine + "#";
	}

	stream.close();
	return fullLine;
}

