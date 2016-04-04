#ifndef STAGE_MENU_H
#define STAGE_MENU_H

#include "Menu.h"

namespace Sprite
{
	class Sprite;
	class AnimatedSprite;
}

namespace Menu
{
	// The stage changing menu
	class StageMenu : public Menu
	{
	public:
		StageMenu(const char* menuName);
		~StageMenu();

		// Inherited overloads
		void draw() const;
		void update();

	private:
		Sprite::Sprite *background;
		Sprite::Sprite *menuSprite;
	};
}

#endif