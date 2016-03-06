#ifndef PLAYER_H
#define PLAYER_H

#include "BattleConstants.h"
#include <string>
#include <vector>
#include "Schooled.h"

// Forward Declaration
namespace Board
{
	class Board;
}

namespace Sprite
{
	class AnimatedSprite;
}

namespace Player
{
	float const OFFSET_X = 35;
	float const OFFSET_Y = 120;
	int const ROW_WIDTH = 40;
	int const ROW_HEIGHT = 31;
	int const ROW_OFFSET = 20;

	// Helper struct holds stats for a player
	struct Stats
	{
		int maxHP;	// Health points
		int currentHP;
		int maxSP;	// Special points
		int currentSP;
		int maxAP;	// Action points
		int currentAP;
		int lockedAP;	// The AP unable to be taken back
	};

	// Helper struct holds attack data
	struct Attack
	{
		std::string name;
		bool range[Stage::BOARD_WIDTH][Stage::BOARD_HEIGHT];
		bool isStatic;	// If the range moves with the player
		int damage;
		int cooldown;	// Number of turns before move can be used again
		int currentCooldown;
	};

	// Helper struct holds special ability data
	struct SpecialAbility
	{
		std::string name;
		bool removesAllTokens;	
		bool removesEnemyTokens;	// Both options for testing purposes
		bool resetCooldowns;
		int heal;	// How much the player is healed
		int damage;	// How much the enemy player is damaged
	};

	// Player class holds a player's data and tools to manipulate it.
	class Player
	{
	public:
		Player();

		// Constructor that takes in a player name and loads the data
		Player(const char* playerName, Board::Board*, Side);

		// Rule of three
		Player(Player const&);		// NEED UPDATE
		Player& operator=(Player const&);		// NEED UPDATE
		~Player();

		// Getters
		inline Side getSide() const { return side; }
		inline int getCurrentHP() const { return stats.currentHP; }
		inline int getCurrentAP() const { return stats.currentAP; }
		inline int getCurrentSP() const { return stats.currentSP; }

		// Changes health of enemy player, tokens on enemy board
		// And changes your cooldown and current AP
		void attack(Player& enemy, int attackNum);

		// Modifies the player's current health
		void changeHealth(int);

		// Use your special ability. May affect the enemy player/board.
		void useSpecial(Player&);

		// Move the character on the board.
		// Sets AP to the path of least resistance. (Board updatePath)
		void move(Direction);

		// Choose to end your turn.
		void passTurn();

		// Resets cooldowns
		void startTurn();

		// Update the player's animations
		void update();

		// Draw the player to the screen
		void draw() const;

	private:
		// Move sprite to relative postion
		void moveSpriteToSide(Side);

		Stats stats;
		int numAttacks;	// Unsure if to be used
		std::vector<Attack> attacks;
		SpecialAbility ability;
		Side side;
		Sprite::AnimatedSprite *sprite;
		Board::Board *boardPtr;	// Pointer to the player's board
	};

};

#endif