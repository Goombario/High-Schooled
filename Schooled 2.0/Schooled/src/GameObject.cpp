#include "GameObject.h"
#include "Schooled.h"

namespace GameObject
{
	GameObject::GameObject()
	{
		mass = 0.0;
		position = Vector2::Vector2(0, 0);
		velocity = Vector2::Vector2(0, 0);
		acceleration = Vector2::Vector2(0, 0);
		prevVelocity = Vector2::Vector2(0, 0);
		prevAcceleration = Vector2::Vector2(0, 0);
	}

	void GameObject::firstOrder()
	{
		position += (velocity * (1 / schooled::FRAMERATE));
	}

	void GameObject::secondOrder()
	{
		double dt = (1 / schooled::FRAMERATE);
		position += prevVelocity * dt + prevAcceleration * (dt * dt);
	}

	void GameObject::euler()
	{
		double dt = (1 / schooled::FRAMERATE);
		position += getVelocity() * dt + 0.5 * getAcceleration() * (dt * dt);
	}

	void GameObject::adamsBashford()
	{
		double dt = (1 / schooled::FRAMERATE);
		position += 0.5 * (3 * (getVelocity() + getAcceleration() * dt) - getPrevVelocity()) * dt;
	}

	void GameObject::impulse(Vector2 const& force)
	{
		acceleration.setX(force.getX() / mass);
		acceleration.setY(force.getY() / mass);
	}
}