#ifndef BATTLE_OBJECT_H
#define BATTLE_OBJECT_H

#include "GameObject.h"

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
		bool acting;
	};
}

#endif