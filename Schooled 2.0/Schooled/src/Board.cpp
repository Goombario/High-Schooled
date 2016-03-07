#include "Board.h"
#include "Player.h"

#include <iostream>

namespace Board
{
	Board::Board()
	{
		playerLocation = 0;
		firstPos = 0;

		for (int w = 0; w < Stage::BOARD_WIDTH; w++)
		{
			for (int h = 0; h < Stage::BOARD_HEIGHT; h++)
			{
				boardTiles[w][h].hasToken = false;
				boardTiles[w][h].isPassable = true;
				boardTiles[w][h].isPath = false;
			}
		}

		clearLeePath();
	}

	void Board::placeToken(int location)
	{
		boardTiles[location % Stage::BOARD_WIDTH]
		[location / Stage::BOARD_HEIGHT].hasToken = true;
	}

	void Board::placeToken(int w, int h)
	{
		boardTiles[w][h].hasToken = true;
	}

	void Board::removeToken(int location)
	{
		boardTiles[location % Stage::BOARD_WIDTH]
			[location / Stage::BOARD_HEIGHT].hasToken = false;
	}

	void Board::removeToken(int w, int h)
	{
		boardTiles[w][h].hasToken = false;
	}

	void Board::clearTokens()
	{
		for (int w = 0; w < Stage::BOARD_WIDTH; w++)
		{
			for (int h = 0; h < Stage::BOARD_HEIGHT; h++)
			{
				removeToken(w, h);
			}
		}
	}

	int Board::checkMatches()
	{
		unsigned int numCompleted = 0;
		// Temporary board holds all the matches
		Board tempBoard;

		// Check all the columns for matches
		for (int w = 0; w < Stage::BOARD_WIDTH; w++)
		{
			unsigned int countCheck = 0;
			for (int h = 0; h < Stage::BOARD_HEIGHT; h++)
			{
				if (boardTiles[w][h].hasToken)
					countCheck++;
			}

			// If all tiles in the column have tokens
			if (countCheck == Stage::BOARD_HEIGHT)
			{
				numCompleted++;
				// Add the completed row to the temporary board
				for (int h = 0; h < Stage::BOARD_HEIGHT; h++)
				{
					tempBoard.placeToken(w, h);
				}
			}
		}

		// Check all the rows for matches
		for (int h = 0; h < Stage::BOARD_HEIGHT; h++)
		{
			unsigned int countCheck = 0;
			for (int w = 0; w < Stage::BOARD_WIDTH; w++)
			{
				if (boardTiles[w][h].hasToken)
					countCheck++;
			}

			// If all tiles in the rows have tokens
			if (countCheck == Stage::BOARD_WIDTH)
			{
				numCompleted++;
				// Add the completed column to the temporary board
				for (int w = 0; w < Stage::BOARD_WIDTH; w++)
				{
					tempBoard.placeToken(w, h);
				}
			}
		}

		(*this) -= tempBoard;
		return numCompleted;
	}

	void Board::print()
	{
		for (int w = 0; w < Stage::BOARD_WIDTH; w++)
		{
			for (int h = 0; h < Stage::BOARD_HEIGHT; h++)
			{
				std::cout << boardTiles[w][h].hasToken;
			}
			std::cout << std::endl;
		}
	}

	bool Board::inBounds(int w, int h)
	{
		if (w < Stage::BOARD_WIDTH && w >= 0 &&
			h < Stage::BOARD_HEIGHT && h >= 0)
			return true;
		return false;
	}

	void Board::draw()
	{

	}
}

// Pathing
namespace Board
{
	void Board::clearPath()
	{
		for (int w = 0; w < Stage::BOARD_WIDTH; w++)
		{
			for (int h = 0; h < Stage::BOARD_HEIGHT; h++)
			{
				boardTiles[w][h].isPath = false;
			}
		}
	}

	int Board::updatePath()
	{
		clearPath();

		clearLeePath();
		waveMap.wMap[firstPos % Stage::BOARD_WIDTH][firstPos / Stage::BOARD_HEIGHT] = 0;

		// Set up the waveMap
		int distance = getLeePath(0);

		tracePath(getPlayerlocation() % Stage::BOARD_WIDTH, getPlayerlocation() / Stage::BOARD_HEIGHT);

		std::cout << "Distance :" << distance << std::endl;
		for (int w = 0; w < Stage::BOARD_WIDTH; w++)
		{
			for (int h = 0; h < Stage::BOARD_HEIGHT; h++)
			{
				std::cout << waveMap.wMap[w][h];
			}
			std::cout << std::endl;
		}

		for (int w = 0; w < Stage::BOARD_WIDTH; w++)
		{
			for (int h = 0; h < Stage::BOARD_HEIGHT; h++)
			{
				std::cout << boardTiles[w][h].isPath;
			}
			std::cout << std::endl;
		}
		return distance;
	}

	void Board::tracePath(int w, int h)
	{
		boardTiles[w][h].isPath = true;

		// IF in bounds AND set value AND less than current node
		if (inBounds(w + 1, h) && waveMap.wMap[w + 1][h] >= 0 && waveMap.wMap[w + 1][h] < waveMap.wMap[w][h])
		{
			tracePath(w + 1, h);
		}
		else if (inBounds(w - 1, h) && waveMap.wMap[w - 1][h] >= 0 && waveMap.wMap[w - 1][h] < waveMap.wMap[w][h])
		{
			tracePath(w - 1, h);
		}
		else if (inBounds(w, h + 1) && waveMap.wMap[w][h + 1] >= 0 && waveMap.wMap[w][h + 1] < waveMap.wMap[w][h])
		{
			tracePath(w, h + 1);
		}
		else if (inBounds(w, h - 1) && waveMap.wMap[w][h - 1] >= 0 && waveMap.wMap[w][h - 1] < waveMap.wMap[w][h])
		{
			tracePath(w, h - 1);
		}
	}

	void Board::clearLeePath()
	{
		for (int w = 0; w < Stage::BOARD_WIDTH; w++)
		{
			for (int h = 0; h < Stage::BOARD_HEIGHT; h++)
			{
				waveMap.wMap[w][h] = -1;
			}
		}
		waveMap.foundTarget = false;
	}

	void Board::setNeighbours(int w, int h, int i)
	{
		// Check neighbours and update their values
		// IF within bounds AND not labeled AND passable
		if (inBounds(w + 1, h) && waveMap.wMap[w + 1][h] < 0 && boardTiles[w + 1][h].isPassable)
		{
			waveMap.wMap[w + 1][h] = i + 1;
		}
		if (inBounds(w - 1, h) && waveMap.wMap[w - 1][h] < 0 && boardTiles[w - 1][h].isPassable)
		{
			waveMap.wMap[w - 1][h] = i + 1;
		}
		if (inBounds(w, h + 1) && waveMap.wMap[w][h + 1] < 0 && boardTiles[w][h + 1].isPassable)
		{
			waveMap.wMap[w][h + 1] = i + 1;
		}
		if (inBounds(w, h - 1) && waveMap.wMap[w][h - 1] < 0 && boardTiles[w][h - 1].isPassable)
		{
			waveMap.wMap[w][h - 1] = i + 1;
		}
	}

	int Board::getLeePath(int i)
	{
		for (int w = 0; w < Stage::BOARD_WIDTH; w++)
		{
			for (int h = 0; h < Stage::BOARD_HEIGHT; h++)
			{
				// If at the target, set foundTarget to true
				if (w + h*Stage::BOARD_HEIGHT == getPlayerlocation() && waveMap.wMap[w][h] >= 0)
				{
					waveMap.foundTarget = true;
					return waveMap.wMap[w][h];
				}

				// Sets all neighbors of the i value to be i+1
				if (waveMap.wMap[w][h] == i)
					setNeighbours(w, h, i);
			}
		}

		return getLeePath(i + 1);
	}
}
