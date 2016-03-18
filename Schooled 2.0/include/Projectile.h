#ifndef PROJECTILE_H
#define PROJECTILE_H

#include "BattleObject.h"

namespace Sprite
{
	class Sprite;
}

namespace Player
{
	class Player;
}

namespace Vector
{
	class Vector2;
}

namespace Projectile
{
	class Projectile : public BattleObject::BattleObject
	{
	public:
		Projectile();
		Projectile(Vector::Vector2 const& force, 
			Vector::Vector2 const& position, Sprite::Sprite const&);
		~Projectile();

		void draw() const;
		void update();

		bool collidesWith(Player::Player const&);

	private:
		Sprite::Sprite *sprite;
		bool hasGravity;
		bool destroyOnCollision;
	};
}

#endif