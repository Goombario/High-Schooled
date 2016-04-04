#include "Menu.h"
#include "Sprite.h"
#include "GameEngine.h"
#include "Schooled.h"

#include "tinyxml2.h"
using namespace tinyxml2;

// Generic Menu class
namespace Menu
{
	Menu::Menu(const char* menuName)
	{
		setPos(Vector::Vector2(schooled::SCREEN_WIDTH_PX / 2.0, schooled::SCREEN_HEIGHT_PX / 2.0));
		selectionNum = 0;

		XMLElement const* menuData = getMenuElement(menuName);
		if (menuData == nullptr)
		{
			std::cerr << "ERROR: Loading menuData: "
				<< XML_ERROR_FILE_READ_ERROR << std::endl;
			exit(-2);
		}

		if (CheckIfNull(menuData->FirstChildElement("Background"), "Menu: Background")) exit(-2);
		background = new Sprite::Sprite(menuData->FirstChildElement("Background"));

		if (CheckIfNull(menuData->FirstChildElement("Menu"), "Menu: Menu sprite")) exit(-2);
		menuSprite = new Sprite::Sprite(menuData->FirstChildElement("Menu"));
	}

	Menu::~Menu()
	{
		delete background;
		delete menuSprite;
	}

	XMLElement const* Menu::getMenuElement(const char* menuName) const
	{
		// Load menu data from player file
		XMLDocument data;
		CheckXMLResult(data.LoadFile("../Schooled/MenuData.xml"));
		XMLNode *pRoot = data.RootElement();
		if (pRoot == nullptr)
		{
			std::cerr << "ERROR: Loading menu data file: "
				<< XML_ERROR_FILE_READ_ERROR << std::endl;
			exit(-2);
		}

		XMLElement *menuData;
		menuData = pRoot->FirstChildElement("Menu");

		// Check if menu data loaded
		std::string menuDataName = menuData->Attribute("name");
		while (menuDataName != menuName)
		{
			menuData = menuData->NextSiblingElement();
			if (menuData == nullptr)
			{
				return nullptr;
			}
			menuDataName = menuData->Attribute("name");
		}

		return menuData;
	}
}

// Main menu
namespace Menu
{
	MainMenu::MainMenu(const char* menuName)
		: Menu(menuName)
	{
		XMLElement const* menuData = getMenuElement(menuName);
		if (menuData == nullptr)
		{
			std::cerr << "ERROR: Loading menuData: "
				<< XML_ERROR_FILE_READ_ERROR << std::endl;
			exit(-2);
		}

		if (CheckIfNull(menuData->FirstChildElement("Highlight"), "Main Menu: Highlight Sprite")) exit(-2);
		if (CheckIfNull(menuData->FirstChildElement("HighlightAnimation"), "Main Menu: Highlight Animation")) exit(-2);
		highlight = new Sprite::AnimatedSprite(menuData->FirstChildElement("Highlight"), menuData->FirstChildElement("HighlightAnimation"));
		highlight->setPos(getPos());

		maxSize = 3;
	}

	MainMenu::~MainMenu()
	{
		delete highlight;
	}

	void MainMenu::draw() const
	{
		background->draw();
		menuSprite->draw();
		highlight->draw();
	}

	void MainMenu::update() 
	{
		highlight->update();
	}

	bool MainMenu::moveSelectionNum(int change)
	{
		if (getSelectionNum() + change >= static_cast<int>(maxSize) ||
			getSelectionNum() + change < 0)
			return false;

		setSelectionNum(getSelectionNum() + change);

		// Change the highlight
		switch (getSelectionNum())
		{
		case 0:
			highlight->changeAnimation(Animation::AnimationEnum::IDLE);
			highlight->setPos(getPos());
			break;
		case 1:
			highlight->changeAnimation(Animation::AnimationEnum::CREDITS);
			highlight->setPos(getPos());
			break;
		case 2:
			highlight->changeAnimation(Animation::AnimationEnum::EXIT);
			highlight->setPos(getPos());
			break;
		}

		return true;
	}
}

// Character Selection Menu
namespace Menu
{
	CharMenu::CharMenu(const char* menuName, Side s)
		: Menu(menuName)
	{
		if (s == Side::LEFT)
		{
			setPos(Vector::Vector2(
				schooled::SCREEN_WIDTH_PX / 4,
				schooled::SCREEN_HEIGHT_PX / 2));
		}
		else
		{
			setPos(Vector::Vector2(
				(schooled::SCREEN_WIDTH_PX / 4) * 3,
				schooled::SCREEN_HEIGHT_PX / 2));
		}

		menuSprite->setPos(getPos());
	}
}