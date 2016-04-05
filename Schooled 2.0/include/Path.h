#ifndef PATH_H
#define PATH_H

#include "GameObject.h"
#include <vector>

namespace Vector
{
	class Vector2;
}

namespace Path
{
	// Class that applies a vector over a set time
	class Path : public GameObject::GameObject
	{
	public:
		Path();
		Path(Vector::Vector2 const& target, Vector::Vector2 const& dest, 
			double timeToTarget, double delay = 0.0);

		// Update the variables every frame
		virtual void update(GameObject& target);

		// if the path is still active
		bool isActive() const;

		// update the path times.
		void moveTime();

	protected:
		Vector::Vector2 origTarget, dest;
		double timeToTarget;
		double delay;

	};

	class Quadratic
	{
	public:
		Quadratic();
		Quadratic(double y1, double y2, double peak, double time);
		
		// Returns the value of Y at t	// 
		double solve(double t);
	private: 
		double tranX, tranY, scaleX, scaleY;
	};

	class ProjectilePath : public Path
	{
	public:
		ProjectilePath();
		ProjectilePath(Vector::Vector2 const& target, Vector::Vector2 const& dest, 
			double timeToTarget, double maxHeight, double delay = 0.0);
		ProjectilePath(Path const& p, double maxHeight);

		void update(GameObject& target);

	private:
		// 

		double gravity;
		double maxHeight;
	};
}

#endif