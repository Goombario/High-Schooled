#include "BattleObject.h"
#include "Schooled.h"

namespace BattleObject
{
	Path::Path()
		: GameObject()
	{
		timeToTarget = 0.0;
	}

	Path::Path(GameObject const& target, Vector::Vector2 const& dest, double t)
		: GameObject(), timeToTarget(t)
	{
		// Get the information for the needed velocity
		setPos(target.getPos());
		Vector::Vector2 dist = target.getPos() - dest;
		Vector::Vector2 vel(
			(dist.getX() * -1.0) / timeToTarget, 
			(dist.getY() * -1.0 ) / timeToTarget);
		setVelocity(vel);
	}

	void Path::update(GameObject& target)
	{
		// If there is time left in the movement
		if (timeToTarget > 0.0)
		{
			timeToTarget -= 1.0 / schooled::FRAMERATE;

			// If it's finished
			if (timeToTarget < 0.0)
			{
				timeToTarget = 0.0;
				setVelocity(Vector::Vector2(0, 0));
			}
		}

		target.setVelocity(getVelocity());
	}

	bool Path::isActive() const
	{
		return (timeToTarget > 0.0);
	}

}