#include "Path.h"
#include "Schooled.h"

namespace Path
{
	Path::Path()
		: GameObject()
	{
		timeToTarget = 0.0;
		delay = 0.0;
	}

	Path::Path(Vector::Vector2 const& target, Vector::Vector2 const& dest, double t, double d)
		: GameObject(), timeToTarget(t), delay(d)
	{
		// Get the information for the needed velocity
		setPos(target);
		Vector::Vector2 dist = target - dest;
		Vector::Vector2 vel(
			(dist.getX() * -1.0) / timeToTarget,
			(dist.getY() * -1.0) / timeToTarget);
		setVelocity(vel);
		setAcceleration(Vector::Vector2(0, 0));

		origTarget = target;
		(*this).dest = dest;
	}

	void Path::moveTime()
	{
		if (delay > 0.0)
		{
			delay -= 1.0 / schooled::FRAMERATE;
			return;
		}

		// If there is time left in the movement
		if (timeToTarget > 0.0)
		{
			timeToTarget -= 1.0 / schooled::FRAMERATE;

			// If it's finished
			if (timeToTarget < 0.0)
			{
				timeToTarget = 0.0;
			}
		}
	}

	void Path::update(GameObject& target)
	{

		//moveTime();
		target.setPos(dest);
		delay = 0.0;
		timeToTarget = 0.0;

		if (!isActive())
		{
			setVelocity(Vector::Vector2(0, 0));
		}
		firstOrder();

		//target.setVelocity(getVelocity());
		//target.setPos(getPos());
	}

	bool Path::isActive() const
	{
		return (timeToTarget > 0.0);
	}

	Quadratic::Quadratic()
	{

	}

	Quadratic::Quadratic(double y1, double y2, double peak, double time)
	{
		tranX = time / 2.0;
		tranY = peak - y1;
		scaleX = tranX * 2.0;
		scaleY = peak - y1;
	}

	double Quadratic::solve(double X)
	{
		double Y, inside;
		inside = (1.0 / scaleX) * (X - tranX);
		Y = scaleY * (inside * inside) + tranY;
		return Y;
	}

	ProjectilePath::ProjectilePath()
		: Path()
	{

	}

	ProjectilePath::ProjectilePath(Vector::Vector2 const& target, Vector::Vector2 const& dest, double t,
		double max, double d)
		: Path(target, dest, t, d)
	{
		// Set the velocity and acceleration based on the maximum height
		if (dest.getY() == target.getY())
		{
			maxHeight = dest.getY() + max;
			Quadratic formula(origTarget.getY(), dest.getY(), maxHeight, timeToTarget);
			setVelocity(Vector::Vector2(getVelocity().getX(), formula.solve(0)));
		}
		else
		{
			maxHeight = 0.0;	// PLACEHOLDER
		}

		gravity = schooled::GRAVITY;
	}

	ProjectilePath::ProjectilePath(Path const& p, double max)
		: Path(p), maxHeight(max)
	{
		// Set the velocity and acceleration based on the maximum height

		maxHeight = dest.getY() + maxHeight;
		Quadratic formula(origTarget.getY(), dest.getY(), maxHeight, timeToTarget);
		setVelocity(Vector::Vector2(getVelocity().getX(), formula.solve(timeToTarget)));

		gravity = schooled::GRAVITY;
	}

	void ProjectilePath::update(GameObject& target)
	{
		setAcceleration(getAcceleration() += Vector::Vector2(0, -gravity));

		moveTime();
		if (!isActive())
		{
			setVelocity(Vector::Vector2(0, 0));
			setAcceleration(Vector::Vector2(0, 0));
		}

		euler();
		target.setPos(getPos());
		target.setVelocity(getVelocity());
		target.setAcceleration(getAcceleration());
	}
}