#ifndef PROJECTILE_H
#define PROJECTILE_H

#include "BattleObject.h"
#include "tinyxml2.h"
#include "Sprite.h"

namespace Vector
{
	class Vector2;
}

namespace Player
{
	class Player;
}

namespace Projectile
{
	// The base class of a projectile object:
	// An object that travels a set distance before terminating.
	class Projectile : public BattleObject::BattleObject
	{
	public:
		Projectile();
		Projectile(tinyxml2::XMLElement const*);

		// Initializes the projectile values for throwing
		void init(
			Player::Player const& player, 
			Player::Player const& enemy, 
			Vector::Vector2 const& tilePos = Vector::Vector2(0, 0));

		void draw() const;
		void update();

		//bool collidesWith(BattleObject const*);

	private:
		// Load projectile data from projectile name
		void loadData(const char*);

		Sprite::Sprite sprite;
		bool hasGravity;
		bool collides;
		double delay;
		bool hasTarget;
		double timeElapsed;
		double timeToTarget;
		Vector::Vector2 offset;
	};
}

#endif