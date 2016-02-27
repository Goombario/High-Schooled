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

		// Checks for matches on the board, and updates the player's stats accordingly.
		// It then removes the matching tokens
		void checkMatches(Player::Player&);

		// Place a single token on the board at given location
		void placeToken(int location);

		// Adds tokens onto the board
		inline Board& operator+=(Board const&);
		inline Board operator+(Board const&) const;

		// Takes players current and first position and draws the path of least resistance.
		// Returns the distance
		int updatePath(int first);

		// Draw the board tiles to the screen
		void draw();

		// Set the player location
		void setPlayerLocation(unsigned int);

	private:
		Tile boardTiles[Stage::BOARD_WIDTH][Stage::BOARD_HEIGHT];
		unsigned int playerLocation;
	};
}

#endif