#include "Board.h"
#include "Player.h"
#include "Sprite.h"

#include <iostream>

namespace Board
{
	Board::Board()
	{
		playerLocation = { 0, 0 };
		firstPos = { 0, 0 };

		for (int h = 0; h < Stage::BOARD_HEIGHT; h++)
		{
			for (int w = 0; w < Stage::BOARD_WIDTH; w++)
			{
				boardTiles[h][w].hasToken = false;
				boardTiles[h][w].isPassable = true;
				boardTiles[h][w].isPath = false;
			}
		}

		clearLeePath();
		tokenSprite = nullptr;
		side = Side::LEFT;
		setActing(false);
	}

	Board::Board(Side s) : Board()
	{
		side = s;
	}

	Board::~Board()
	{

	}

	void Board::placeToken(unsigned int h, unsigned int w)
	{
		boardTiles[h][w].hasToken = true;
	}

	void Board::placeToken(COORD c)
	{
		boardTiles[c.Y][c.X].hasToken = true;
	}

	void Board::removeToken(unsigned int h, unsigned int w)
	{
		boardTiles[h][w].hasToken = false;
	}

	void Board::removeToken(COORD c)
	{
		boardTiles[c.Y][c.X].hasToken = false;
	}


	void Board::clearTokens()
	{
		for (int h = 0; h < Stage::BOARD_HEIGHT; h++)
		{
			for (int w = 0; w < Stage::BOARD_WIDTH; w++)
			{
				removeToken(h, w);
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
				if (boardTiles[h][w].hasToken)
					countCheck++;
			}

			// If all tiles in the column have tokens
			if (countCheck == Stage::BOARD_HEIGHT)
			{
				numCompleted++;
				// Add the completed row to the temporary board
				for (int h = 0; h < Stage::BOARD_HEIGHT; h++)
				{
					tempBoard.placeToken(h, w);
				}
			}
		}

		// Check all the rows for matches
		for (int h = 0; h < Stage::BOARD_HEIGHT; h++)
		{
			unsigned int countCheck = 0;
			for (int w = 0; w < Stage::BOARD_WIDTH; w++)
			{
				if (boardTiles[h][w].hasToken)
					countCheck++;
			}

			// If all tiles in the rows have tokens
			if (countCheck == Stage::BOARD_WIDTH)
			{
				numCompleted++;
				// Add the completed column to the temporary board
				for (int w = 0; w < Stage::BOARD_WIDTH; w++)
				{
					tempBoard.placeToken(h, w);
				}
			}
		}

		(*this) -= tempBoard;
		print();
		return numCompleted;
	}

	void Board::print()
	{
		std::cout << "Tokens:" << std::endl;
		for (int h = 0; h < Stage::BOARD_HEIGHT; h++)
		{
			for (int w = 0; w < Stage::BOARD_WIDTH; w++)
			{
				std::cout << boardTiles[h][w].hasToken;
			}
			std::cout << std::endl;
		}
	}

	bool Board::inBounds(int h, int w)
	{
		if (w < Stage::BOARD_WIDTH && w >= 0 &&
			h < Stage::BOARD_HEIGHT && h >= 0)
			return true;
		return false;
	}

	void Board::draw() const
	{
		float initX = (side == Side::LEFT) ? OFFSET_X : CENTER_X + OFFSET_X;
		int offsetRight = Stage::BOARD_WIDTH - 1;

		for (int h = 0; h < Stage::BOARD_HEIGHT; h++)
		{
			for (int w = 0; w < Stage::BOARD_WIDTH; w++)
			{
				if (boardTiles[h][w].hasToken)
				{
					int wPos = w;
					if (side == Side::RIGHT) wPos = Stage::BOARD_WIDTH - 1 - wPos;
					tokenSprite->drawAt((initX + 
						(wPos * ROW_WIDTH) +
						(h * ROW_OFFSET)) * schooled::SCALE,
						(OFFSET_Y - (h * ROW_HEIGHT)) * schooled::SCALE);
				}
			}
		}
	}

	void Board::update()
	{
		
	}
}

// Pathing
namespace Board
{
	void Board::clearPath()
	{
		for (int h = 0; h < Stage::BOARD_HEIGHT; h++)
		{
			for (int w = 0; w < Stage::BOARD_WIDTH; w++)
			{
				boardTiles[h][w].isPath = false;
			}
		}
	}

	int Board::updatePath()
	{
		clearPath();

		clearLeePath();
		waveMap.wMap[firstPos.Y][firstPos.X] = 0;

		// Set up the waveMap
		int distance = getLeePath(0);

		tracePath(getPlayerlocation().Y, getPlayerlocation().X);

		std::cout << "Distance:" << distance << std::endl;
		std::cout << "Wave Map:" << std::endl;
		for (int h = 0; h < Stage::BOARD_HEIGHT; h++)
		{
			for (int w = 0; w < Stage::BOARD_WIDTH; w++)
			{
				std::cout << waveMap.wMap[h][w];
			}
			std::cout << std::endl;
		}

		std::cout << "Board Path" << std::endl;
		for (int h = 0; h < Stage::BOARD_HEIGHT; h++)
		{
			for (int w = 0; w < Stage::BOARD_WIDTH; w++)
			{
				std::cout << boardTiles[h][w].isPath;
			}
			std::cout << std::endl;
		}
		std::cout << std::endl;
		return distance;
	}

	void Board::tracePath(int h, int w)
	{
		boardTiles[h][w].isPath = true;

		// IF in bounds AND set value AND less than current node
		if (inBounds(h, w + 1) && waveMap.wMap[h][w + 1] >= 0 && waveMap.wMap[h][w + 1] < waveMap.wMap[h][w])
		{
			tracePath(h, w + 1);
		}
		else if (inBounds(h, w - 1) && waveMap.wMap[h][w - 1] >= 0 && waveMap.wMap[h][w - 1] < waveMap.wMap[h][w])
		{
			tracePath(h, w - 1);
		}
		else if (inBounds(h + 1, w) && waveMap.wMap[h + 1][w] >= 0 && waveMap.wMap[h + 1][w] < waveMap.wMap[h][w])
		{
			tracePath(h + 1, w);
		}
		else if (inBounds(h - 1, w) && waveMap.wMap[h - 1][w] >= 0 && waveMap.wMap[h - 1][w] < waveMap.wMap[h][w])
		{
			tracePath(h - 1, w);
		}
	}

	void Board::clearLeePath()
	{
		for (int h = 0; h < Stage::BOARD_HEIGHT; h++)
		{
			for (int w = 0; w < Stage::BOARD_WIDTH; w++)
			{
				waveMap.wMap[h][w] = -1;
			}
		}
		waveMap.foundTarget = false;
	}

	void Board::setNeighbours(int h, int w, int i)
	{
		// Check neighbours and update their values
		// IF within bounds AND not labeled AND passable
		if (inBounds(h, w + 1) && waveMap.wMap[h][w + 1] < 0 && boardTiles[h][w + 1].isPassable)
		{
			waveMap.wMap[h][w + 1] = i + 1;
		}
		if (inBounds(h, w - 1) && waveMap.wMap[h][w - 1] < 0 && boardTiles[h][w - 1].isPassable)
		{
			waveMap.wMap[h][w - 1] = i + 1;
		}
		if (inBounds(h + 1, w) && waveMap.wMap[h + 1][w] < 0 && boardTiles[h + 1][w].isPassable)
		{
			waveMap.wMap[h + 1][w] = i + 1;
		}
		if (inBounds(h - 1, w) && waveMap.wMap[h - 1][w] < 0 && boardTiles[h - 1][w].isPassable)
		{
			waveMap.wMap[h - 1][w] = i + 1;
		}
	}

	int Board::getLeePath(int i)
	{
		for (int h = 0; h < Stage::BOARD_HEIGHT; h++)
		{
			for (int w = 0; w < Stage::BOARD_WIDTH; w++)
			{
				// If at the target, set foundTarget to true
				if (getPlayerlocation() == COORD{w, h} && waveMap.wMap[h][w] >= 0)		// CHECK
				{
					waveMap.foundTarget = true;
					return waveMap.wMap[h][w];
				}

				// Sets all neighbors of the i value to be i+1
				if (waveMap.wMap[h][w] == i)
					setNeighbours(h, w, i);
			}
		}

		return getLeePath(i + 1);
	}
}

bool operator ==(COORD a, COORD b)
{
	return (a.X == b.X && a.Y == b.Y);
}

bool operator !=(COORD a, COORD b)
{
	return(a.X != b.X || a.Y != b.Y);
}

COORD operator +(COORD a, COORD b)
{
	return{ a.X + b.X, a.Y + b.Y };
}

COORD operator -(COORD a, COORD b)
{
	return{ a.X - b.X, a.Y - b.Y };
}
