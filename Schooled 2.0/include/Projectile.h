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
	struct Target
	{
		double timeToTarget;
		int X;
		int Y;
	};

	// The base class of a projectile object:
	// An object that travels a set distance before terminating.
	class Projectile : public BattleObject::BattleObject
	{
	public:
		Projectile();
		Projectile(tinyxml2::XMLElement const*);
		~Projectile();

		void init(Player::Player const& player, Player::Player const& enemy);

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
		Target target;
		Vector::Vector2 offset;
	};
}

#endif