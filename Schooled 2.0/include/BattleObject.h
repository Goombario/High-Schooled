#ifndef BATTLE_OBJECT_H
#define BATTLE_OBJECT_H

#include "GameObject.h"
#include <vector>

namespace Vector
{
	class Vector2;
}

namespace BattleObject
{
	struct rect
	{
		double width;
		double height;
	};

	// Abstract class that all battle objects draw from
	class BattleObject : public GameObject::GameObject
	{
	public:
		virtual ~BattleObject() {}

		virtual void draw() const = 0;
		virtual void update() = 0;

		inline bool isActing() const { return acting; }
		void setActing(bool a) { acting = a; }

	private:
		bool acting = false;
	};

	// Class that applies a vector over a set time
	class Path : public GameObject::GameObject
	{
	public:
		Path();
		Path(GameObject const& target, Vector::Vector2 const& dest, double timeToTarget);

		// Update the variables every frame
		void update(GameObject& target);

		// if the path is still active
		bool isActive() const;

	private:
		double timeToTarget;
	};
}

#endif