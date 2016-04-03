#ifndef STAGE_MENU_H
#define STAGE_MENU_H

#include "GenericMenu.h"

namespace Sprite
{
	class Sprite;
	class AnimatedSprite;
}

namespace Menu
{

	// Holds the stage elements ex. music, background elements, HUD
	class StageMenu : public GenericMenu //Public inheritance from Package base class
	{
	public:
		// FUTURE: Load data from file for images
		StageMenu(const char* menuName);
		~StageMenu();

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