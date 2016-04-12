#ifndef TUTORIAL_H
#define TUTORIAL_H

#include "GameObject.h"
#include "BattleConstants.h"
#include <vector>

namespace Sprite
{
	class Sprite;
	class AnimatedSprite;
}

namespace Tutorial
{
	class Tutorial : public GameObject::GameObject
	{
	public:
		Tutorial();
		Tutorial(const char*, Side);

		// Returns if the player is finished with the tutorial
		bool isReady() const { return ready; }
		void setReady(bool isReady = true);

		// Move the slides by the given modifier, and reset the animations
		void moveSlide(int);

		//Draw and update
		void draw() const;
		void update();
		
	private:
		bool ready;
		unsigned int slideNum;
		std::vector<Sprite::AnimatedSprite> slides;
	};
}

#endif