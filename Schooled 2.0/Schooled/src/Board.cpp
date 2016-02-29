#include "Board.h"
#include "Player.h"

#include <iostream>

namespace Board
{
	Board::Board()
	{
		playerLocation = 0;
		for (int w = 0; w < Stage::BOARD_WIDTH; w++)
		{
			for (int h = 0; h < Stage::BOARD_HEIGHT; h++)
			{
				boardTiles[w][h].hasToken = false;
				boardTiles[w][h].isPassable = true;
				boardTiles[w][h].isPath = false;
			}
		}
	}

	void Board::placeToken(int location)
	{
		boardTiles[location / Stage::BOARD_WIDTH]
		[location % Stage::BOARD_HEIGHT].hasToken = true;	// UNSURE OF MATH
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
					tempBoard.boardTiles[w][h].hasToken = true;
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
					tempBoard.boardTiles[w][h].hasToken = true;
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
}