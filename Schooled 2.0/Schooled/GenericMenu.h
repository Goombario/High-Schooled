#ifndef GENERIC_MENU_H
#define GENERIC_MENU_H

namespace Sprite
{
	class Sprite;
	class AnimatedSprite;
}

namespace Menu
{
	
	// Holds the stage elements ex. music, background elements, HUD
	class GenericMenu
	{
	public:
		// FUTURE: Load data from file for images
		GenericMenu(const char* menuName);
		~GenericMenu();

		// Draw functions
		void drawBackground();
		void drawMenu();

		// Update the menu
		void update();

	private:
		Sprite::Sprite *background;
		Sprite::Sprite *menuSprite;
	};
}

#endif