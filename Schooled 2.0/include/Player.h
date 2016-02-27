#ifndef PLAYER_H
#define PLAYER_H

#include "Stage.h"
#include <vector>

// Forward Declaration
namespace Board
{
	class Board;
}

namespace Sprite
{
	class Sprite;
}

namespace Player
{
	// Helper struct holds stats for a player
	struct Stats
	{
		int maxHP;
		int currentHP;
		int maxSP;
		int currentSP;
		int maxAP;
		int currentAP;
	};

	// Helper struct holds attack data
	struct Attack
	{
		bool range[Stage::BOARD_WIDTH][Stage::BOARD_HEIGHT];
		int damage;
		int cooldown;
		int currentCooldown;
	};

	// Helper struct holds special ability data
	struct SpecialAbility
	{
		bool removesAllTokens;	
		bool removesEnemyTokens;	// Both options for testing purposes
		bool resetCooldowns;
		int heal;
		int damage;
	};

	// Player class holds a player's data and tools to manipulate it.
	class Player
	{
	public:
		Player();

		// Constructor that takes in a player data file
		Player(const char* dataPath);

		// Rule of three
		Player(Player const&);
		Player& operator=(Player const&);
		~Player();

		// Changes health of enemy player, tokens on enemy board
		// And changes your cooldown and current AP
		void attack(Player const&);

		// Use your special ability. May affect the enemy player/board.
		void useSpecial(Player const&);

		// Move the character on the board.
		// Sets AP to the path of least resistance. (Board updatePath)
		void move();

		// Choose to end your turn.
		void passTurn();

		// Draw the player to the screen
		void draw();

	private:
		Stats stats;
		int numAttacks;	// Unsure if to be used
		std::vector<Attack> attacks;
		SpecialAbility ability;
		Sprite::Sprite *sprite;
		Board::Board *boardPtr;	// Pointer to the player's board

		int firstPos;	// The position at beginning of turn
	};

};

#endif