#ifndef PROJECTILE_H
#define PROJECTILE_H

#include "GameObject.h"

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
	class Projectile : public GameObject::GameObject
	{
	public:
		Projectile();
		Projectile(Vector::Vector2 const& force, 
			Vector::Vector2 const& position, Sprite::Sprite const&);
		~Projectile();

		bool collidesWith(Player::Player const&);

	private:
		Sprite::Sprite *sprite;
		bool hasGravity;
		bool destroyOnCollision;
	};
}

#endif