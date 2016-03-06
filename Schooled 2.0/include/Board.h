#ifndef BOARD_H
#define BOARD_H

#include "BattleConstants.h"

namespace Player
{
	class Player;
}

namespace Board
{

	struct Tile
	{
		bool hasToken;
		bool isPassable;
		bool isPath;
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

		// Takes players current and first position and draws the path of least resistance.
		// Returns the distance
		int updatePath();
		void clearPath();

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
			return boardTiles[i / Stage::BOARD_WIDTH]
				[i % Stage::BOARD_HEIGHT];
		};

		Tile boardTiles[Stage::BOARD_WIDTH][Stage::BOARD_HEIGHT];
		unsigned int playerLocation;
		unsigned int firstPos;	// The position at beginning of turn

	};
}

#include "Board.inl"

#endif