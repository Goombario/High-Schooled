#ifndef CHAR_MENU_H
#define CHAR_MENU_H

#include "Menu.h"

namespace Sprite
{
	class Sprite;
	class AnimatedSprite;
}

namespace Menu
{
	// Character selection menu
	class CharMenu : public Menu
	{
	public:
		CharMenu(const char* menuName);
		~CharMenu();

		// Inherited overloads
		void draw() const;
		void update();

	private:
		Sprite::Sprite *background;
		Sprite::Sprite *menuSprite;
	};
}

#endif