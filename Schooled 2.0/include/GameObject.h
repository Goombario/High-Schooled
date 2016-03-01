#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include "Vector2.h"
using Vector::Vector2;

namespace GameObject
{
	class GameObject
	{
	public:
		GameObject();
		void firstOrder(Vector2 const& velocity);

	private:
		double mass;
		Vector2 position;
	};
}


#endif