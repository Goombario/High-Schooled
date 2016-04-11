#ifndef BOARD_H
#define BOARD_H

#include "BattleConstants.h"
#include "BattleObject.h"
#include "Particle.h"
#include "Schooled.h"
#include <vector>
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
	float const OFFSET_X = 105;
	float const OFFSET_Y = 335;
	float const CENTER_X = schooled::SCREEN_WIDTH_PX / 2;
	int const ROW_WIDTH = 100;
	int const ROW_HEIGHT = 70;
	int const ROW_OFFSET = 70;

	enum class TileState
	{
		IDLE,
		SELECTED,
		BLOCKED,
		CRACKED,
		PLACING,
		COMPLETING,
		DESTROYING,
	};

	struct Tile
	{
		bool hasToken;
		bool isPassable;
		bool isPath;
		Vector::Vector2 pos;
		Sprite::AnimatedSprite *tileSprite;
		Sprite::AnimatedSprite *tokenSprite;
		TileState state;
		Particle::Emitter tokenDestroyEmitter;

		// Change the state of the tile
		void changeState(TileState, Side);

		// Make the tile to be idle state (without changing state)
		void makeIdle();
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
		void placeToken(unsigned int h, unsigned int w, double delay = 0.0);
		void placeToken(COORD c, double delay = 0.0);

		// Remove a token from given location
		void removeToken(unsigned int h, unsigned int w, double delay = 0.0);
		void removeToken(COORD c, double delay=0.0);

		// Destroys the token underfoot
		void destroyToken(unsigned int h, unsigned int w);
		void destroyToken(COORD c);

		// Set the cracked setting of the token.
		void crackToken(unsigned int h, unsigned int w, bool cracked = true);
		void crackToken(COORD c, bool cracked = true);

		// Clear all cracked tokens
		void destroyCrackedTokens();

		// Adds tokens onto the board
		inline Board& operator+=(Board const&);
		inline Board const operator+(Board const&) const;

		// Subtract tokens from the board
		inline Board& operator-=(Board const&);
		inline Board const operator-(Board const&) const;

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

		// Set the player location
		inline void setPlayerLocation(COORD newLocation) { playerLocation = newLocation; }
		inline void setPlayerFirstPos(COORD newLocation) { firstPos = newLocation; }
		inline COORD getPlayerlocation() const { return playerLocation; }
		inline Side getSide() const { return side; }

		// Modify the tile states
		// Set all tile states to idle
		void clearTiles();
		// Set the tiles at the given coordinates to selected
		void setSelectedTiles(std::vector<COORD> const&);

		// Get the tile vector
		Vector::Vector2 getTilePos(COORD location) const { return boardTiles[location.Y][location.X].pos; }

		// Get the tile state
		TileState getTileState(COORD location) const { return boardTiles[location.Y][location.X].state; }

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

		// Mark tiles as completed, and clear the temporary completeBoard
		void completeRows();

		// Helper function that checks if position is in bounds
		bool inBounds(int, int);

		// Helper functions create a path from one point to another (Lee's Formula)
		void clearPath();
		int getLeePath(int i);
		void clearLeePath();
		void setNeighbours(int h, int w, int i);	// Sets all invalid spaces around coordinate to i+1
		void tracePath(int h, int w);

	private:
		Side side;
		Tile boardTiles[Stage::BOARD_HEIGHT][Stage::BOARD_WIDTH];
		bool completedTiles[Stage::BOARD_HEIGHT][Stage::BOARD_WIDTH];
		WaveMap waveMap;
		COORD playerLocation;
		COORD firstPos;	// The position at beginning of turn
	};
}

// Operators for manipulating COORDS
bool operator ==(COORD a, COORD b);
bool operator !=(COORD a, COORD b);
COORD operator +(COORD a, COORD b);
COORD operator -(COORD a, COORD b);

#include "Board.inl"

#endif