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

		virtual void draw() const = 0;
		virtual void update() = 0;

		// Move the menu selection by the number given
		// Returns false if the operation failed
		virtual bool moveSelectionNum(int change) = 0;

		// Get the selection number
		int getSelectionNum() const { return selectionNum; }

		// Reset the selection number and other menu items
		virtual void reset() = 0;

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
		MainMenu();
		~MainMenu();

		// Move the menu selection by the number given
		// Returns false if the operation failed
		bool moveSelectionNum(int change);

		// Reset the cursor to default
		void reset();

		// Inherited overloads
		void draw() const;
		void update();

	private:
		unsigned int maxSize;
		Sprite::AnimatedSprite *highlight;
		Sprite::Sprite *darkLayer;
	};

	// Character selection menu for a single character
	class CharMenu : public Menu
	{
	public:
		CharMenu(Side);
		~CharMenu();

		// Move the menu selection by the number given
		// Returns false if the operation failed
		bool moveSelectionNum(int change);

		// Inherited overloads
		void draw() const;
		void update();

		// Move the selection
		void moveSelection(int change);

		// Reset the character location and the spotlight 
		void reset();

		// get the chosen name
		std::string getChosenName() const { return characterNames.at(getSelectionNum()); }

		// Set the menu to finished
		void setFinished(bool isFinished = true);
		bool isFinished() const { return finished; }

	private:
		bool finished;
		unsigned int maxSize;
		std::vector<Sprite::Sprite> nameSprites;
		std::vector<Sprite::AnimatedSprite> characterSprites;
		std::vector<std::string> characterNames;
		Sprite::Sprite *spotlight;

		// Load all character and name sprites
		void loadSprites(tinyxml2::XMLElement const*);
	};

	//// The stage changing menu
	//class StageMenu : public Menu
	//{
	//public:
	//	StageMenu(const char* menuName);
	//	~StageMenu();

	//	// Move the menu selection by the number given
	//	// Returns false if the operation failed
	//	bool moveSelectionNum(int change);

	//	// Inherited overloads
	//	void draw() const;
	//	void update();

	//private:
	//};
}

#endif