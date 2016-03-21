#ifndef PROJECTILE_H
#define PROJECTILE_H

#include "BattleObject.h"
#include "tinyxml2.h"

namespace Sprite
{
	class Sprite;
}

namespace Vector
{
	class Vector2;
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
		~Projectile();

		void init();

		void draw() const;
		void update();

		//bool collidesWith(BattleObject const*);

	private:
		Sprite::Sprite *sprite;
		bool hasGravity;
		bool collides;
		double delay;
		Vector::Vector2 offset;
	};
}

#endif