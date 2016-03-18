#ifndef PARTICLE_H
#define PARTICLE_H

#include <vector>
#include "Vector2.h"
#include "GameObject.h"

namespace Sprite
{
	class Sprite;
}

namespace Particle
{
	struct Particle
	{
		Vector::Vector2 position;
		Vector::Vector2 velocity;
		Vector::Vector2 acceleration;
		Vector::Vector2 force;
		float mass;
		float life;
	};

	class Emitter : public GameObject::GameObject
	{
	public:
		Emitter();

	private:
		std::vector<Particle> particleList;
	};
}

#endif