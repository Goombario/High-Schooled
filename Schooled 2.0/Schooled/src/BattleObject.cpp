#include "BattleObject.h"
#include "Schooled.h"

namespace BattleObject
{
	Path::Path()
		: GameObject()
	{
		timeToTarget = 0.0;
		delay = 0.0;
	}

	Path::Path(GameObject const& target, Vector::Vector2 const& dest, double t, double d)
		: GameObject(), timeToTarget(t), delay(d)
	{
		// Get the information for the needed velocity
		setPos(target.getPos());
		Vector::Vector2 dist = target.getPos() - dest;
		Vector::Vector2 vel(
			(dist.getX() * -1.0) / timeToTarget, 
			(dist.getY() * -1.0 ) / timeToTarget);
		setVelocity(vel);
		setAcceleration(Vector::Vector2(0, 0));
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
		moveTime();
		if (!isActive())
		{
			setVelocity(Vector::Vector2(0, 0));
		}
		firstOrder();

		target.setVelocity(getVelocity());
		target.setPos(getPos());
	}

	bool Path::isActive() const
	{
		return (timeToTarget > 0.0);
	}

	ProjectilePath::ProjectilePath()
		: Path()
	{

	}

	ProjectilePath::ProjectilePath(GameObject const& target, Vector::Vector2 const& dest, double t,
		double maxHeight, double d)
		: Path(target, dest, t, d)
	{
		// Set the velocity and acceleration based on the maximum height
		gravity = 0.0;
	}

	ProjectilePath::ProjectilePath(Path const& p, double maxHeight)
		: Path(p)
	{
		// Set the velocity and acceleration based on 
		gravity = 0.0;
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