#include "Log.h"

#include <Windows.h>
#include <cassert>

#include "Console_color.h"
#include "Buffer.h"
#include "Schooled.h"

namespace con = JadedHoboConsole;	// Used for the color

using std::map;
using std::string;
using std::vector;

Log::Log()
{

}

void Log::push_back(string s)
{
	log.push_back(s);
	colours.push_back(con::fgLoWhite);
	newLines.push_back(true);
}

void Log::push_back(string s, WORD c)
{
	log.push_back(s);
	colours.push_back(c);
	newLines.push_back(true);
}

void Log::clear()
{
	log.clear();
	colours.clear();
	newLines.clear();
}

// UNUSED
/*
void Log::display(HANDLE hConsole)
{
	int max = (log.size() >= 3) ? (log.size() - 3) : 0;	// determines the number of lines to display
	int row, col;
	int grayscale = 2;
	row = schooled::SCREEN_HEIGHT - 3;
	col = TEXT_START;
	for (unsigned int i = max; i < log.size(); i++)
	{
		switch (grayscale)
		{
		case 0:
			SetConsoleTextAttribute(hConsole, con::fgHiWhite);
			for (unsigned int e = 0; e < log[i].size(); e++)
			{
				assert(col + e < schooled::SCREEN_WIDTH);
				console.Position(col + e, row);
				console << log[i][e];
				if (newLine)
				{
					Sleep(10);
				}
			}
			newLine = false;
			break;

		case 1:
			SetConsoleTextAttribute(hConsole, con::fgLoWhite);
			for (unsigned int e = 0; e < log[i].size(); e++)
			{
				assert(col + e < schooled::SCREEN_WIDTH);
				console.Position(col + e, row);
				console << log[i][e];
			}
			break;

		case 2:
			SetConsoleTextAttribute(hConsole, con::fgGray);
			for (unsigned int e = 0; e < log[i].size(); e++)
			{
				assert(col + e < schooled::SCREEN_WIDTH);
				console.Position(col + e, row);
				console << log[i][e];
			}
			break;
		}
		grayscale--;
		row++;
		col = TEXT_START;
	}

}
*/

void Log::display(Buffer& buffer, int row)
{
	const int SIZE = 4;

	// determines the number of lines to display
	int max = (log.size() >= SIZE) ? (log.size() - SIZE) : 0;	

	int col, tempInt, iModifier;
	int grayscale = 0;
	std::string temp;
	col = TEXT_START;

	// Modify the start if the row is given
	if (row == 0)
	{
		row = schooled::SCREEN_HEIGHT - 1;
		iModifier = 1;
	}
	else
	{
		iModifier = 2;
	}

	// Output the log
	for (int i = log.size() - iModifier; i >= max; i--)
	{
		// If there are newline symbols, decrease the row to fit
		temp = log[i];
		while (temp.find('#') != std::string::npos)
		{
			tempInt = temp.find('#');
			temp = temp.substr(tempInt + 1);
			row--;
		}

		// If the log entry can't fit on the screen, then break
		if (row < schooled::SCREEN_HEIGHT - SIZE) break;

		// Change the colour based on how old the line is.
		if (newLines[i] || i == log.size() - iModifier)
		{
			buffer.draw(log[i], colours[i] | FOREGROUND_INTENSITY, row, col);
			newLines[i] = false;
		}
		else
		{
			buffer.draw(log[i], colours[i], row, col);
		}

		row--;
		col = TEXT_START;
	}
}