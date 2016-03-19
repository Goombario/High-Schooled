#ifndef PLAYER_H
#define PLAYER_H

#include "BattleConstants.h"
#include "BattleObject.h"
#include "Projectile.h"
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
	class Sprite;
	class AnimatedSprite;
}

namespace Player
{

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
		Projectile::Projectile projectile;
		Sprite::AnimatedSprite *icon;
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
	class Player : public BattleObject::BattleObject
	{
	public:
		Player();

		// Constructor that takes in a player name and loads the data
		Player(const char* playerName, Board::Board&);

		// Rule of three
		Player(Player const&);		// NEED UPDATE
		Player& operator=(Player const&);		// NEED UPDATE
		~Player();

		// Getters
		inline int getCurrentHP() const { return stats.currentHP; }
		inline int getCurrentAP() const { return stats.currentAP; }
		inline int getCurrentSP() const { return stats.currentSP; }
		inline int getMaxHP() const { return stats.maxHP; }
		Side getSide() const;

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
		void moveArrow(Direction);

		// Choose to end your turn.
		void passTurn();

		// Resets cooldowns
		void startTurn();

		// Update the player's animations
		void update();

		// Draw the player to the screen
		void draw() const;

		// Get the token sprite
		Sprite::Sprite& getTokenSprite() { return *token; }

	private:
		// Move sprite to relative postion
		void moveSpriteToSide(Sprite::Sprite&);

		// Update the attack icon animations
		void updateAttacks();

		Stats stats;
		int numAttacks;	// Unsure if to be used
		std::vector<Attack> attacks;
		SpecialAbility ability;
		Sprite::Sprite *token;
		Sprite::Sprite *glow;
		Sprite::AnimatedSprite *sprite;
		Sprite::AnimatedSprite *arrowSprite;
		Board::Board *boardPtr;	// Pointer to the player's board
	};

};

#endif