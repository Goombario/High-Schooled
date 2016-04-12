#ifndef PROJECTILE_H
#define PROJECTILE_H

#include "BattleObject.h"
#include "BattleConstants.h"
#include "tinyxml2.h"
#include "Sprite.h"
#include "Collision.h"

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
		Projectile(tinyxml2::XMLElement const*, Side);

		// Initializes the projectile values for throwing
		void init(Player::Player const& player, Vector::Vector2 const& target);

		// Getters
		double getDelay() const { return delay; }
		double getTimeToTarget() const { return timeToTarget; }

		void draw() const;
		void update();
		void checkCollision(Collision::AABB const&);

		//bool collidesWith(BattleObject const*);

	private:
		// Load projectile data from projectile name
		void loadData(const char*, Side);

		Sprite::Sprite sprite;
		bool hasGravity;
		bool collides;
		double delay;
		bool hasTarget;
		double timeElapsed;
		double timeToTarget;
		Vector::Vector2 offset;
		Collision::AABB boundingBox;
	};
}

#endif