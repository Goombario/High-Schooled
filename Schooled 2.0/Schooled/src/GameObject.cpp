#include "GameObject.h"
#include "Schooled.h"

namespace GameObject
{
	GameObject::GameObject()
	{

	}

	void GameObject::firstOrder(Vector2 const& velocity)
	{
		position += (velocity * (1 / schooled::FRAMERATE));
	}
}