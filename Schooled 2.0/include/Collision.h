#ifndef COLLISION_H
#define COLLISION_H

#include "Vector2.h"

namespace Collision
{
	struct Box
	{
		double width;
		double length;
	};

	struct Collision
	{
		bool status;
		Vector::Vector2 overlap;
	};

	class AABB
	{

	};
}


#endif