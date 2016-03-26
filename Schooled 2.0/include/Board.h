#ifndef BOARD_H
#define BOARD_H

#include "BattleConstants.h"
#include "BattleObject.h"
#include "Schooled.h"
#include <Windows.h>

namespace Player
{
	class Player;
}

namespace Sprite
{
	class Sprite;
	class AnimatedSprite;
}

namespace Board
{
	float const OFFSET_X = 100;
	float const OFFSET_Y = 337;
	float const CENTER_X = schooled::SCREEN_WIDTH_PX / 2;
	int const ROW_WIDTH = 100;
	int const ROW_HEIGHT = 75;
	int const ROW_OFFSET = 75;

	struct Tile
	{
		bool hasToken;
		bool isPassable;
		bool isPath;
		Vector::Vector2 pos;
		Sprite::AnimatedSprite *tileSprite;
	};

	struct WaveMap
	{
		int wMap[Stage::BOARD_WIDTH][Stage::BOARD_HEIGHT];	// For pathing
		bool foundTarget;
	};

	class Board : public BattleObject::BattleObject
	{
	public:
		Board();
		Board(Side);

		~Board();

		// Checks for matches on the board, and removes the matching lines.
		// Returns the number of lines completed.
		int checkMatches();

		// Place a single token on the board at given location
		void placeToken(unsigned int h, unsigned int w);
		void placeToken(COORD c);

		// Remove a token from given location
		void removeToken(unsigned int h, unsigned int w);
		void removeToken(COORD c);

		// Adds tokens onto the board
		inline Board& operator+=(Board const&);
		inline Board const operator+(Board const&) const;

		// Subtract tokens from the board
		inline Board& operator-=(Board const&);
		inline Board const operator-(Board const&) const;

		// Set the token sprite
		inline void setTokenSprite(Sprite::Sprite &spritePtr) { tokenSprite = &spritePtr; }

		// Takes players current and first position and draws the path of least resistance.
		// Returns the distance
		int updatePath();

		// Removes all tokens from the board
		void clearTokens();

		// Print the token properties to the console (DEBUG)
		void print();

		// Draw the board and tiles to the screen
		void draw() const;

		// Update board animations
		void update();

		// Check if the board is acting (i.e. tokens are animating)
		inline bool isActing() const { return acting; }

		// Set the player location
		inline void setPlayerLocation(COORD newLocation) { playerLocation = newLocation; }
		inline void setPlayerFirstPos(COORD newLocation) { firstPos = newLocation; }
		inline COORD getPlayerlocation() const { return playerLocation; }
		inline Side getSide() const { return side; }

		// Get the tile vector
		Vector::Vector2 getTilePos(COORD location) const { return boardTiles[location.Y][location.X].pos; }

	private:
		// Helper function returns tile at number position
		inline Tile& getTile(int i)
		{
			return boardTiles[i % Stage::BOARD_WIDTH]
				[i / Stage::BOARD_HEIGHT];
		};
		inline Tile& getTile(int h, int w)
		{
			return boardTiles[h][w];
		}

		// Helper function that checks if position is in bounds
		bool inBounds(int, int);

		// Helper functions create a path from one point to another (Lee's Formula)
		void clearPath();
		int getLeePath(int i);
		void clearLeePath();
		void setNeighbours(int h, int w, int i);	// Sets all invalid spaces around coordinate to i+1
		void tracePath(int h, int w);

	private:
		bool acting;
		Side side;
		Tile boardTiles[Stage::BOARD_HEIGHT][Stage::BOARD_WIDTH];
		WaveMap waveMap;
		COORD playerLocation;
		COORD firstPos;	// The position at beginning of turn
		Sprite::Sprite *tokenSprite;
	};
}

// Operators for manipulating COORDS
bool operator ==(COORD a, COORD b);
bool operator !=(COORD a, COORD b);
COORD operator +(COORD a, COORD b);
COORD operator -(COORD a, COORD b);

#include "Board.inl"

#endif