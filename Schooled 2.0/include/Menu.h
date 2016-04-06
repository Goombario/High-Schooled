#ifndef GENERIC_MENU_H
#define GENERIC_MENU_H

#include "GameObject.h"

namespace Sprite
{
	class Sprite;
	class AnimatedSprite;
}

namespace Menu
{
	class Menu : public GameObject::GameObject
	{
	public:
		// FUTURE: Load data from file for images
		Menu(const char* menuName);
		~Menu();

		// Move the menu selection by the number given
		void moveSelection(int);
		int getSelection() const { return selectionNum; }

		virtual void draw() const;
		virtual void update();

	private:
		int selectionNum;
		Sprite::Sprite *background;
	};
}

#endif