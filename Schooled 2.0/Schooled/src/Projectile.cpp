#include "Projectile.h"
#include "Vector2.h"
#include "Sprite.h"
#include "Schooled.h"

namespace Projectile
{
	Projectile::Projectile()
	{
		sprite = nullptr;
	}

	Projectile::Projectile(tinyxml2::XMLElement const* projElement)
	{

	}

	Projectile::~Projectile()
	{
		delete sprite;
	}

	void Projectile::draw() const
	{
		sprite->drawAt(getPos());
	}
	void Projectile::update()
	{
		if (hasGravity)
		{
			setAcceleration(Vector2(getAcceleration().getX(), getAcceleration().getY() - (9.81 / schooled::FRAMERATE)));
		}

		euler();

		// If massively out of bounds
		if (getPos().getX() < 0 - schooled::SCREEN_WIDTH_PX ||
			getPos().getX() > schooled::SCREEN_WIDTH_PX * 2 ||
			getPos().getY() < 0 - schooled::SCREEN_HEIGHT_PX ||
			getPos().getY() > schooled::SCREEN_HEIGHT_PX * 2)
		{
			setActing(false);
		}
	}

}