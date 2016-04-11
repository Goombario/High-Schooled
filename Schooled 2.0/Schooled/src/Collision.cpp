#include "Collision.h"

namespace Collision
{
	AABB::AABB()
	{
		
	}

	AABB::AABB(Vector::Vector2 const& p, double w, double h)
		: pos(p)
	{
		max = Vector::Vector2(w / 2.0, h / 2.0);
		min = Vector::Vector2(-w / 2.0, -h / 2.0);
	}

	void AABB::setPos(Vector::Vector2 const& newPos)
	{
		pos = newPos;
	}

	Collision AABB::testAABB(AABB const& box1)
	{
		Collision testCollision;

		// Set the overlap
		testCollision.overlap.setX(
			((*this).pos.getX() + (*this).max.getX()) -
			(box1.pos.getX() + box1.min.getX()));

		testCollision.overlap.setY(
			((*this).pos.getY() + (*this).max.getY()) -
			(box1.pos.getY() + box1.min.getY()));

		// Check if the algorithm passes
		testCollision.status = 
			(testCollision.overlap.getX() >= 0.0
			&&
			(box1.pos.getY() + box1.max.getY()) -
			((*this).pos.getY() + (*this).min.getY()) >= 0.0);

		return testCollision;
	}
}