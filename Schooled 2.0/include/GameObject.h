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
		void firstOrder();
		void secondOrder();

		void impulse(Vector2 const& force);

	private:
		double mass;
		Vector2 position;
		Vector2 velocity;
		Vector2 acceleration;
	};
}


#endif