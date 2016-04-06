#ifndef PARTICLE_H
#define PARTICLE_H

#include <vector>
#include "Vector2.h"
#include "GameObject.h"
#include "Sprite.h"

namespace Particle
{

	// Particle structure that emitters generate
	struct Particle
	{
		Sprite::Sprite sprite;
		float mass;
		float life;
	};

	// Range structure that holds the distance 
	// the particle can be generated from the origin, 
	// and the degrees of rotation it can have.
	struct Range
	{
		Vector::Vector2 maxDisplacement;
		int minAngle, maxAngle;
	};

	// Base emitter class for particles
	class Emitter : public GameObject::GameObject
	{
	public:
		Emitter();
		Emitter(Range const&);

		// Generate particles based on given parameters
		void generate(int numParticles);

		void draw() const;
		void update();

	private:
		std::vector<Particle> particleList;
		double maxLife, minLife;
		double mass;
		Range range;

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