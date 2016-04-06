#ifndef MAIN_MENU_H
#define MAIN_MENU_H

#include "Menu.h"

namespace Sprite
{
	class Sprite;
	class AnimatedSprite;
}

namespace Menu
{
	// Main menu class
	class MainMenu : public Menu
	{
	public:
		MainMenu(const char* menuName);
		~MainMenu();

		// Inherited overloads
		void draw() const;
		void update();

	private:
		Sprite::Sprite *background;
		Sprite::Sprite *menuSprite;
	};
}

#endif