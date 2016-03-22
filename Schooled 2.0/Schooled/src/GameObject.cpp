#include "GameObject.h"
#include "Schooled.h"

namespace GameObject
{
	GameObject::GameObject()
	{
		mass = 0.0;
		position = Vector2(0, 0);
		velocity = Vector2(0, 0);
		acceleration = Vector2(0, 0);
		prevVelocity = Vector2(0, 0);
		prevAcceleration = Vector2(0, 0);
	}

	GameObject::GameObject(double m, Vector2 p, Vector2 v, Vector2 a)
		: mass(m),
		position(p),
		acceleration(a)
	{
		setPrevVelocity(Vector2(0, 0));
		setPrevAcceleration(Vector2(0, 0));
	}

	void GameObject::firstOrder()
	{
		setPos(getPos() + (velocity * static_cast<double>(1.0 / schooled::FRAMERATE)));
	}

	Vector2 GameObject::firstOrder(Vector2 const& lower, Vector2 const& upper) const
	{
		double dt = static_cast<double>(1.0 / schooled::FRAMERATE);
		return (lower + (upper * dt));
	}

	void GameObject::secondOrder()
	{
		double dt = static_cast<double>(1.0 / schooled::FRAMERATE);
		setPos(getPos() + getPrevVelocity() * dt + getPrevAcceleration() * (dt * dt));

		// Update variables
		setPrevVelocity(getVelocity());
		setPrevAcceleration(getAcceleration());
		setVelocity(firstOrder(getVelocity(), getAcceleration()));
	}

	void GameObject::euler()
	{
		double dt = static_cast<double>(1.0 / schooled::FRAMERATE);
		setPos(getPos() + (getVelocity() * dt) + (0.5 * getAcceleration() * (dt * dt)));

		// Update variables
		setPrevVelocity(getVelocity());
		setPrevAcceleration(getAcceleration());
		setVelocity(firstOrder(getVelocity(), getAcceleration()));
	}

	void GameObject::adamsBashford()
	{
		double dt = static_cast<double>(1.0 / schooled::FRAMERATE);
		setPos(getPos() + 0.5 * (3 * (getVelocity() + getAcceleration() * dt) - getPrevVelocity()) * dt);

		// Update variables
		setPrevVelocity(getVelocity());
		setPrevAcceleration(getAcceleration());
		setVelocity(firstOrder(getVelocity(), getAcceleration()));
	}

	void GameObject::impulse(Vector2 const& force)
	{
		acceleration.setX(force.getX() / mass);
		acceleration.setY(force.getY() / mass);
	}
}