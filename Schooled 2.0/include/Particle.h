#ifndef PARTICLE_H
#define PARTICLE_H

#include <vector>
#include "Vector2.h"
#include "GameObject.h"
#include "Sprite.h"
#include "Collision.h"

namespace Particle
{

	// Particle structure that emitters generate
	struct Particle : public GameObject::GameObject
	{
		Sprite::Sprite sprite;
		Collision::AABB	boundingBox;
		float mass;
		float life;

		void update();
		void draw() const;
	};

	// Base emitter class for particles
	class Emitter : public GameObject::GameObject
	{
	public:
		Emitter();
		Emitter(const char* emitterName);

		// Generate particles based on given parameters
		void generate(int numParticles);

		void draw() const;
		void update();

		// Test the collision of all active particles against the given box
		void testCollision(Collision::AABB const&);

	private:
		std::vector<Particle> particleList;
		double maxLife, minLife;
		double maxAngle, minAngle;
		double minSpeed, maxSpeed;
		double particleMass;
		bool hasGravity;

		// The list of available sprites for the particles to choose from
		std::vector<Sprite::Sprite> spriteList;
	};

	// Emitter that generates projectiles that have a target
	class TargetEmitter : public Emitter
	{
	public:
		TargetEmitter();

	private:
		
	};
}

#endif