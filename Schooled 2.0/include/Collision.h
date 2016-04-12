#ifndef COLLISION_H
#define COLLISION_H

#include "Vector2.h"

namespace Collision
{
	const bool DEBUG_DRAW = true;

	struct Collision
	{
		bool status;
		Vector::Vector2 overlap;
	};

	class AABB
	{
	public:
		AABB();
		AABB(Vector::Vector2 const& pos, double width, double height);

		// Get and set the position of the box
		void setPos(Vector::Vector2 const& newPos);
		Vector::Vector2 const& getPos() { return pos; }

		// Test the collision of this box against another box
		Collision testAABB(AABB const& box1);

	private:
		Vector::Vector2 max, min, pos;
	};
}


#endif