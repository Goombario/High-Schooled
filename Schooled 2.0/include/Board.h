#ifndef BOARD_H
#define BOARD_H

#include "BattleConstants.h"

namespace Player
{
	class Player;
}

namespace Sprite
{
	class Sprite;
}

namespace Board
{

	struct Tile
	{
		bool hasToken;
		bool isPassable;
		bool isPath;
	};

	struct WaveMap
	{
		int wMap[Stage::BOARD_WIDTH][Stage::BOARD_HEIGHT];	// For pathing
		bool foundTarget;
	};

	class Board
	{
	public:
		Board();

		// Checks for matches on the board, and removes the matching lines.
		// Returns the number of lines completed.
		int checkMatches();

		// Place a single token on the board at given location
		void placeToken(int location);
		void placeToken(int w, int h);

		// Remove a token from given location
		void removeToken(int location);
		void removeToken(int w, int h);

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
		void draw();

		// Update board animations
		void update();

		// Set the player location
		inline void setPlayerLocation(unsigned int newLocation) { playerLocation = newLocation; }
		inline void setPlayerFirstPos(unsigned int newLocation) { firstPos = newLocation; }
		inline unsigned int getPlayerlocation() { return playerLocation; }

	private:
		// Helper function returns tile at number position
		inline Tile& getTile(int i)
		{
			return boardTiles[i % Stage::BOARD_WIDTH]
				[i / Stage::BOARD_HEIGHT];
		};
		inline Tile& getTile(int w, int h)
		{
			return boardTiles[w][h];
		}

		// Helper function that checks if position is in bounds
		bool inBounds(int, int);

		// Helper functions create a path from one point to another (Lee's Formula)
		void clearPath();
		int getLeePath(int i);
		void clearLeePath();
		void setNeighbours(int w, int h, int i);	// Sets all invalid spaces around coordinate to i+1
		void tracePath(int w, int h);

	private:
		Tile boardTiles[Stage::BOARD_WIDTH][Stage::BOARD_HEIGHT];
		WaveMap waveMap;
		unsigned int playerLocation;
		unsigned int firstPos;	// The position at beginning of turn
		Sprite::Sprite *tokenSprite;
	};
}

#include "Board.inl"

#endif