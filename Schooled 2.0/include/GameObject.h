#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include "Vector2.h"
using Vector::Vector2;

namespace GameObject
{
	class GameObject
	{
	public:
		GameObject();
		GameObject(double mass, Vector2 position, Vector2 velocity, Vector2 acceleration);

		void firstOrder();
		Vector2 firstOrder(Vector2 const& lower, Vector2 const& upper) const;
		void secondOrder();
		void euler();
		void adamsBashford();

		void impulse(Vector2 const& force);

		Vector2 getPos() const { return position; }
		Vector2 getVelocity() const { return velocity; }
		Vector2 getAcceleration() const { return acceleration; }

		void setPos(Vector2 const& newPos) { position = newPos; }
		void setVelocity(Vector2 const& newVel) { velocity = newVel; }
		void setAcceleration(Vector2 const& newAcc) { acceleration = newAcc; }

		// Virtual functions
		virtual void update() {}
		virtual void draw() {}
	
	private:
		Vector2 getPrevVelocity() const { return prevVelocity; }
		Vector2 getPrevAcceleration() const { return prevAcceleration; }
		void setPrevVelocity(Vector2 const& newPrevVelocity) { prevVelocity = newPrevVelocity; }
		void setPrevAcceleration(Vector2 const& newPrevAcceleration) { prevAcceleration = newPrevAcceleration; }

		double mass;
		Vector2 position;
		Vector2 velocity;
		Vector2 prevVelocity;
		Vector2 acceleration;
		Vector2 prevAcceleration;
	};
}


#endif