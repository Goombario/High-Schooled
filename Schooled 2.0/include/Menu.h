#ifndef GENERIC_MENU_H
#define GENERIC_MENU_H

#include "GameObject.h"
#include "BattleConstants.h"
#include <string>
#include <vector>

namespace Sprite
{
	class Sprite;
	class AnimatedSprite;
}

namespace tinyxml2
{
	class XMLElement;
}

namespace Menu
{
	class Menu : public GameObject::GameObject
	{
	public:
		// FUTURE: Load data from file for images
		Menu(const char* menuName);
		virtual ~Menu();

		virtual void draw() const;
		virtual void update() = 0;

		// Get the XMLElement for the menu
		tinyxml2::XMLElement const* getMenuElement(const char* menuName) const;

		// Move the menu selection by the number given
		// Returns false if the operation failed
		virtual bool moveSelectionNum(int change) = 0;

		// Get the selection number
		int getSelectionNum() const { return selectionNum; }

		// Reset the selection number
		void reset() { selectionNum = 0; }

	private:
		int selectionNum;

	protected:
		Sprite::Sprite *background;
		Sprite::Sprite *menuSprite;

		// Set the selection number
		void setSelectionNum(int newNum) { selectionNum = newNum; }
	};

	class MainMenu : public Menu
	{
	public:
		MainMenu(const char* menuName);
		~MainMenu();

		// Move the menu selection by the number given
		// Returns false if the operation failed
		bool moveSelectionNum(int change);

		// Inherited overloads
		void draw() const;
		void update();

	private:
		unsigned int maxSize;
		Sprite::AnimatedSprite *highlight;
	};

	// Character selection menu for a single character
	class CharMenu : public Menu
	{
	public:
		CharMenu(const char* menuName, Side);
		~CharMenu();

		// Move the menu selection by the number given
		// Returns false if the operation failed
		bool moveSelectionNum(int change);

		// Inherited overloads
		void draw() const;
		void update();

		void moveSelection(int change);

	private:
	};

	// The stage changing menu
	class StageMenu : public Menu
	{
	public:
		StageMenu(const char* menuName);
		~StageMenu();

		// Move the menu selection by the number given
		// Returns false if the operation failed
		bool moveSelectionNum(int change);

		// Inherited overloads
		void draw() const;
		void update();

	private:
	};
}

#endif