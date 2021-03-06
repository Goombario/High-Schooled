#ifndef PLAYER_H
#define PLAYER_H

#include "BattleConstants.h"
#include "BattleObject.h"
#include "Particle.h"
#include <string>
#include <vector>
#include <map>
#include "Schooled.h"
#include "Board.h"
#include "Vector2.h"
#include "Collision.h"

// Forward Declaration

namespace Sprite
{
	class Sprite;
	class AnimatedSprite;
}

namespace Stage
{
	class HUD;
}

namespace Projectile
{
	class Projectile;
}

namespace Path
{
	class Path;
}

namespace Player
{
	enum class CollideWith
	{
		NONE,
		ENEMY_PLAYER,
		ENEMY_CENTER
	};

	// Helper class that holds icon data
	class Icon : public GameObject::GameObject
	{
	public:
		Icon();
		Icon(const char* iconName, Side);

		Icon(Icon const&);
		Icon& operator=(Icon const&);
		~Icon();

		void draw() const;
		void drawNoGlowAt(Vector::Vector2 const&) const;
		void drawAt(Vector::Vector2 const&) const;
		void update();

		// Setters
		void setSelected(bool);
		void setCooldown(int);

	private:
		Sprite::Sprite *icon;
		Sprite::AnimatedSprite *cooldown;
		Sprite::AnimatedSprite *glow;
	};

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
		Icon icon;
		std::vector<Projectile::Projectile> projectiles;
	};

	// Helper struct holds special ability data
	struct SpecialAbility
	{
		std::string name;
		bool removesAllTokens;	
		bool removesEnemyTokens;	// Both options for testing purposes
		bool resetCooldowns;
		bool continuous; // If the emitter constantly emits particles
		double delay;	// How long between special particles
		double currentDelay;
		int heal;	// How much the player is healed
		int damage;	// How much the enemy player is damaged
		Particle::Emitter stage1;
		Particle::Emitter stage2;
		CollideWith collideType;
	};

	// Helper stuct that holds the attack window
	class AttackWindow : public GameObject::GameObject
	{
	public:
		AttackWindow();
		AttackWindow(Side);

		AttackWindow(AttackWindow const&);
		AttackWindow& operator=(AttackWindow const&);
		~AttackWindow();

		void draw() const;
		void drawAtPlayer(Vector::Vector2 const&) const;
		void update();

		// Reset the selected to default
		void reset();

		// Get and set currently active icon index
		inline int getActiveIconIndex() const { return attackNum; }
		void setActiveIconIndex(int i);
		void clearActiveIcon();

		// Takes in a number to move the index by
		void moveActiveIconIndex(int difference);

		// Add icon to the list
		void pushIcon(Icon* newIcon) { icons.push_back(newIcon); };

	private:
		Side side;
		int attackNum;
		Sprite::Sprite *window;
		Vector::Vector2 iconOffset;
		Vector::Vector2 windowOffset;
		std::vector<Icon*> icons;
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

		friend class Stage::HUD;

		// Getters
		inline int getCurrentHP() const { return stats.currentHP; }
		inline int getCurrentAP() const { return stats.currentAP; }
		inline int getCurrentSP() const { return stats.currentSP; }
		inline int getMaxHP() const { return stats.maxHP; }
		inline int getMaxSP() const { return stats.maxSP; }
		inline int getMaxAP() const { return stats.maxAP; }
		Side getSide() const;

		// Changes health of enemy player, tokens on enemy board
		// And changes your cooldown and current AP
		bool attack(Player& enemy);

		// Modifies the player's current health
		void changeHealth(int);

		// Use your special ability. May affect the enemy player/board.
		bool canUseSpecial() const { return (stats.currentSP >= stats.maxSP); }
		bool isUsingSpecial() const { return usingSpecial; }
		void useSpecial(Player&);

		// Move the character on the board.
		// Sets AP to the path of least resistance. (Board updatePath)
		void move(Direction);
		void moveArrow(Direction);

		// Choose to end your turn.
		void passTurn();

		// Set cooldowns for the first player
		void firstTurn();

		// Resets cooldowns
		void startTurn();

		// Finish planning phase by moving player to arrow location
		void endChoosing();

		// Update the player's animations
		void update();
		void updateProjectiles(Player const&);

		// Draw the player to the screen
		void draw() const;
		void drawAttackWindow() const;
		void drawProjectiles() const;

		// Modify the attack menu
		void initAttackMenu(Player&);
		void moveSelectedAttack(int, Player&);
		void clearAttackMenu(Player&);
		void clearBoardTiles() { boardPtr->clearTiles(); }

		// Find out the attack pattern based on the current player position
		std::vector<COORD> const getAttackPattern(Attack const&) const;
		std::vector<COORD> const getAttackPattern(unsigned int) const;

		// Get the board
		Board::Board const* getBoard() const { return boardPtr; }

	private:
		// Move sprite to relative postion
		//void moveSpriteToSide(Sprite::Sprite&);
		void moveToSide();

		// Update attack icon cooldown
		void updateIconCooldown();

		bool usingSpecial;
		Stats stats;
		int numAttacks;	// Unsure if to be used
		std::vector<Attack> attacks;
		std::vector<Projectile::Projectile> activeProjectiles;
		std::vector<Path::Path*> paths;
		std::vector<COORD> boardPath;
		SpecialAbility ability;
		AttackWindow window;
		Sprite::Sprite *shadow;
		Sprite::AnimatedSprite *sprite;
		Sprite::AnimatedSprite *arrowSprite;
		Board::Board *boardPtr;	// Pointer to the player's board
		Collision::AABB boundingBox;
	};
	
}

#endif