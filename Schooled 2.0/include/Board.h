#ifndef BOARD_H
#define BOARD_H

#include "Stage.h"

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

		// Adds tokens onto the board
		inline Board& operator+=(Board const&);
		inline Board const operator+(Board const&) const;

		// Subtract tokens from the board
		inline Board& operator-=(Board const&);
		inline Board const operator-(Board const&) const;

		// Takes players current and first position and draws the path of least resistance.
		// Returns the distance
		int updatePath(int first);

		// Draw the board tiles to the screen
		void draw();

		// Print the tile properties to the console (DEBUG)
		void print();

		// Set the player location
		inline void setPlayerLocation(unsigned int newLocation) { playerLocation = newLocation; }

	private:
		Tile boardTiles[Stage::BOARD_WIDTH][Stage::BOARD_HEIGHT];
		unsigned int playerLocation;
	};
}

#include "Board.inl"

#endif