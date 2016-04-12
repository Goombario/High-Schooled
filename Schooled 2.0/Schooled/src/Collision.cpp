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
		double x_max0 = (*this).pos.getX() + (*this).max.getX();
		double x_min0 = (*this).pos.getX() + (*this).min.getX();
		double y_max0 = (*this).pos.getY() + (*this).max.getY();
		double y_min0 = (*this).pos.getY() + (*this).min.getY();

		double x_max1 = box1.pos.getX() + box1.max.getX();
		double x_min1 = box1.pos.getX() + box1.min.getX();
		double y_max1 = box1.pos.getY() + box1.max.getY();
		double y_min1 = box1.pos.getY() + box1.min.getY();

		testCollision.overlap.setX(x_max0 - x_min1);
		testCollision.overlap.setY(y_max0 - y_min1);

		// Check if the algorithm passes
		testCollision.status = (
			testCollision.overlap.getX() >= 0.0 && x_max1 - x_min0 >= 0.0 &&
			testCollision.overlap.getY() >= 0.0 && y_max1 - y_min0 >= 0.0);

		return testCollision;
	}
}