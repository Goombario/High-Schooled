#include "GameObject.h"
#include "Schooled.h"

namespace GameObject
{
	GameObject::GameObject()
	{

	}

	void GameObject::firstOrder()
	{
		position += (velocity * (1 / schooled::FRAMERATE));
	}

	void GameObject::secondOrder()
	{
		position = position + (acceleration * (1 / schooled::FRAMERATE) * (1 / schooled::FRAMERATE));
	}

	void GameObject::impulse(Vector2 const& force)
	{
		acceleration.setX(force.getX() / mass);
		acceleration.setY(force.getY() / mass);
	}
}