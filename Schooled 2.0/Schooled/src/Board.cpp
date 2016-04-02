#include "Board.h"
#include "Player.h"
#include "Sprite.h"
#include "GameEngine.h"
#include "Image.h"

#include <iostream>

namespace Board
{
	void Tile::changeState(TileState s, Side side)
	{
		switch (s)
		{
		case TileState::IDLE:
			tileSprite->changeAnimation(Animation::AnimationEnum::IDLE);
			if (!hasToken)
			{
				tokenSprite->changeAnimation(Animation::AnimationEnum::TOKEN_EMPTY);
			}
			else if (hasToken && tokenSprite->isIdle() ||
				hasToken && tokenSprite->getCurrentAnimation() == Animation::AnimationEnum::TOKEN_GHOST)
			{
				tokenSprite->changeAnimation(Animation::AnimationEnum::IDLE);
			}
			break;

		case TileState::SELECTED:
			if (side == Side::LEFT)
			{
				tileSprite->changeAnimation(Animation::AnimationEnum::RED_PULSE);
			}
			else
			{
				tileSprite->changeAnimation(Animation::AnimationEnum::BLUE_PULSE);
			}
			tokenSprite->changeAnimation(Animation::AnimationEnum::TOKEN_GHOST);
			break;

		case TileState::BLOCKED:
			tileSprite->changeAnimation(Animation::AnimationEnum::BLOCKED);
			break;

		case TileState::PLACING:
			tokenSprite->pushAnimation(Animation::AnimationEnum::TOKEN_PLACE);
			tokenSprite->pushAnimation(Animation::AnimationEnum::TOKEN_EMPTY);
			tokenSprite->addDelay(1.0);
			break;

		default:
			break;
		}
	}

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
				boardTiles[h][w].pos = Vector::Vector2();
				boardTiles[h][w].tileSprite = nullptr;
				boardTiles[h][w].tokenSprite = nullptr;
			}
		}

		clearLeePath();
		side = Side::LEFT;
		setActing(false);
	}

	Board::Board(Side s) : Board()
	{
		side = s;
		float initX = (side == Side::LEFT) ? (OFFSET_X + ROW_OFFSET) : (OFFSET_X + CENTER_X);
		int wPos;

		// Set the board position
		setPos(Vector::Vector2(initX + 200, OFFSET_Y - 80));

		// Get tile data
		// Set the tile sprite
		Image::Image tileImage = GameEngine::getImageManager()->loadImage(schooled::getResourcePath("img") + "GlowyTiles.png", 150, 65);
		Animation::AnimationData tileData(schooled::getResourcePath("img/Image_Data") + "TileGlows.xml");
		Sprite::AnimatedSprite tileSprite(tileImage, tileData);

		Image::Image tokenImage = GameEngine::getImageManager()->loadImage(schooled::getResourcePath("img") + "Token.png", 80, 80);
		Animation::AnimationData tokenData(schooled::getResourcePath("img/Image_Data") + "Token.xml");
		Sprite::AnimatedSprite tokenSprite(tokenImage, tokenData);

		for (int h = 0; h < Stage::BOARD_HEIGHT; h++)
		{
			for (int w = 0; w < Stage::BOARD_WIDTH; w++)
			{
				wPos = (side == Side::LEFT) ? w : Stage::BOARD_WIDTH - 1 - w;
				boardTiles[h][wPos].pos = Vector::Vector2(
					(initX + w * ROW_WIDTH) +
					(h * ROW_OFFSET) * schooled::SCALE,
					(OFFSET_Y - (h * ROW_HEIGHT) * schooled::SCALE));

				// Create the tile sprite
				boardTiles[h][wPos].tileSprite = new Sprite::AnimatedSprite(tileSprite);
				boardTiles[h][wPos].tileSprite->setPos(boardTiles[h][wPos].pos);

				// Create the token sprite
				boardTiles[h][wPos].tokenSprite = new Sprite::AnimatedSprite(tokenSprite);
				boardTiles[h][wPos].tokenSprite->setPos(boardTiles[h][wPos].pos);

				// Set the tile state
				boardTiles[h][wPos].changeState(TileState::IDLE, getSide());
			}
		}

		boardTiles[1][1].changeState(TileState::BLOCKED, getSide());
	}

	Board::~Board()
	{
		for (int h = 0; h < Stage::BOARD_HEIGHT; h++)
		{
			for (int w = 0; w < Stage::BOARD_WIDTH; w++)
			{
				delete boardTiles[h][w].tileSprite;
				delete boardTiles[h][w].tokenSprite;
			}
		}
	}

	void Board::placeToken(unsigned int h, unsigned int w, double delay)
	{
		boardTiles[h][w].hasToken = true;
		boardTiles[h][w].tokenSprite->changeAnimation(Animation::AnimationEnum::TOKEN_PLACE);
		boardTiles[h][w].tokenSprite->addDelay(delay);
	}

	void Board::placeToken(COORD c, double delay)
	{
		placeToken(c.Y, c.X, delay);
	}

	void Board::removeToken(unsigned int h, unsigned int w, double delay)
	{
		boardTiles[h][w].hasToken = false;
		boardTiles[h][w].tokenSprite->changeAnimation(Animation::AnimationEnum::TOKEN_EMPTY);
		boardTiles[h][w].tokenSprite->addDelay(delay);
	}

	void Board::removeToken(COORD c, double delay)
	{
		removeToken(c.Y, c.X, delay);
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
					tempBoard.boardTiles[h][w].hasToken = true;
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
					tempBoard.boardTiles[h][w].hasToken = true;
				}
			}
		}


		(*this) -= tempBoard;
		// Go through the board, and set all the ones that are going to PLACING
		for (int h = 0; h < Stage::BOARD_HEIGHT; h++)
		{
			for (int w = 0; w < Stage::BOARD_WIDTH; w++)
			{
				if (tempBoard.boardTiles[h][w].hasToken)
				{
					(*this).boardTiles[h][w].changeState(TileState::PLACING, getSide());
				}
			}
		}

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
		float initX = (side == Side::LEFT) ? OFFSET_X + ROW_OFFSET : CENTER_X + OFFSET_X;

		for (int h = 0; h < Stage::BOARD_HEIGHT; h++)
		{
			for (int w = 0; w < Stage::BOARD_WIDTH; w++)
			{
				boardTiles[h][w].tileSprite->draw();
				boardTiles[h][w].tokenSprite->draw();

			}
		}
	}

	void Board::update()
	{
		for (int h = 0; h < Stage::BOARD_HEIGHT; h++)
		{
			for (int w = 0; w < Stage::BOARD_WIDTH; w++)
			{
				boardTiles[h][w].tileSprite->update();
				boardTiles[h][w].tokenSprite->update();
			}
		}
	}

	void Board::clearTiles()
	{
		for (int h = 0; h < Stage::BOARD_HEIGHT; h++)
		{
			for (int w = 0; w < Stage::BOARD_WIDTH; w++)
			{
				boardTiles[h][w].changeState(TileState::IDLE, getSide());
			}
		}
	}

	void Board::setSelectedTiles(std::vector<COORD> const& pattern)
	{
		for (auto it = pattern.begin(); it != pattern.end(); it++)
		{
			boardTiles[(*it).Y][(*it).X].changeState(TileState::SELECTED, getSide());
			if (getPlayerlocation() == COORD{ (*it).X, (*it).Y })
			{
				boardTiles[(*it).Y][(*it).X].tokenSprite->changeAnimation(Animation::AnimationEnum::TOKEN_EMPTY);
			}
		}
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
