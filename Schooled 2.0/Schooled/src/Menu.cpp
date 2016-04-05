#include "Menu.h"
#include "Sprite.h"
#include "GameEngine.h"
#include "Schooled.h"

#include <iostream>
#include "tinyxml2.h"
using namespace tinyxml2;

// Generic Menu class
namespace Menu
{
	Menu::Menu(const char* menuName)
	{
		setPos(Vector::Vector2(schooled::SCREEN_WIDTH_PX / 2.0, schooled::SCREEN_HEIGHT_PX / 2.0));
		selectionNum = 0;
		maxSize = 0;

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
				std::cerr << "ERROR: Loading menuData: "
					<< XML_ERROR_FILE_READ_ERROR << std::endl;
				exit(-2);
			}
			menuDataName = menuData->Attribute("name");
		}

		if (CheckIfNull(menuData->FirstChildElement("Background"), "Menu: Background")) exit(-2);
		background = new Sprite::Sprite(menuData->FirstChildElement("Background"));
		background->setPos(getPos());

		if (CheckIfNull(menuData->FirstChildElement("Menu"), "Menu: Menu sprite")) exit(-2);
		menuSprite = new Sprite::Sprite(menuData->FirstChildElement("Menu"));
		menuSprite->setPos(getPos());
	}

	Menu::~Menu()
	{
		delete background;
		delete menuSprite;
	}
}

// Main menu
namespace Menu
{
	MainMenu::MainMenu()
		: Menu("MainMenu")
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
		while (menuDataName != "MainMenu")
		{
			menuData = menuData->NextSiblingElement();
			if (menuData == nullptr)
			{
				std::cerr << "ERROR: Loading menuData: "
					<< XML_ERROR_FILE_READ_ERROR << std::endl;
				exit(-2);
			}
			menuDataName = menuData->Attribute("name");
		}

		if (CheckIfNull(menuData->FirstChildElement("DarkLayer"), "Main Menu: DarkLayer")) exit(-2);
		darkLayer = new Sprite::Sprite(menuData->FirstChildElement("DarkLayer"));
		darkLayer->setPos(getPos());

		if (CheckIfNull(menuData->FirstChildElement("Highlight"), "Main Menu: Highlight Sprite")) exit(-2);
		if (CheckIfNull(menuData->FirstChildElement("HighlightAnimation"), "Main Menu: Highlight Animation")) exit(-2);
		highlight = new Sprite::AnimatedSprite(menuData->FirstChildElement("Highlight"), menuData->FirstChildElement("HighlightAnimation"));
		highlight->setPos(getPos() + Vector::Vector2(10, 52));

		maxSize = 3;
	}

	MainMenu::~MainMenu()
	{
		delete highlight;
	}

	void MainMenu::draw() const
	{
		background->draw();
		darkLayer->draw();
		menuSprite->draw();
		highlight->draw();
	}

	void MainMenu::update() 
	{
		highlight->update();
	}

	void MainMenu::moveSelectionNum(int change)
	{
		if (getSelectionNum() + change >= static_cast<int>(maxSize) ||
			getSelectionNum() + change < 0)
		{
			setSelectionNum(maxSize - 1 - getSelectionNum());
		}
		else
		{
			setSelectionNum(getSelectionNum() + change);
		}

		// Change the highlight
		switch (getSelectionNum())
		{
		case 0:
			highlight->changeAnimation(Animation::AnimationEnum::IDLE);
			highlight->setPos(getPos() + Vector::Vector2(10, 52));
			break;
		case 1:
			highlight->changeAnimation(Animation::AnimationEnum::CREDITS);
			highlight->setPos(getPos() + Vector::Vector2(10, -72));
			break;
		case 2:
			highlight->changeAnimation(Animation::AnimationEnum::EXIT);
			highlight->setPos(getPos() + Vector::Vector2(10, -197));
			break;
		}
	}

	void MainMenu::reset()
	{
		setSelectionNum(0);
	}
}

// Character Selection Menu
namespace Menu
{
	CharMenu::CharMenu(Side s)
		: Menu("CharMenu")
	{
		finished = false;
		Vector::Vector2 spotPos;

		const char* sideName;
		if (s == Side::LEFT)
		{
			setPos(Vector::Vector2(
				schooled::SCREEN_WIDTH_PX / 4,
				schooled::SCREEN_HEIGHT_PX / 2));
			sideName = "MenuLeft";
			spotPos = Vector::Vector2(-12, 57);
		}
		else
		{
			setPos(Vector::Vector2(
				(schooled::SCREEN_WIDTH_PX / 4) * 3,
				schooled::SCREEN_HEIGHT_PX / 2));
			sideName = "MenuRight";
			spotPos = Vector::Vector2(13, 57);
		}

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
		while (menuDataName != "CharMenu")
		{
			menuData = menuData->NextSiblingElement();
			if (menuData == nullptr)
			{
				std::cerr << "ERROR: Loading menuData: "
					<< XML_ERROR_FILE_READ_ERROR << std::endl;
				exit(-2);
			}
			menuDataName = menuData->Attribute("name");
		}

		if (CheckIfNull(menuData->FirstChildElement(sideName), "Char Menu: Menu Sprite")) exit(-2);
		menuSprite = new Sprite::Sprite(menuData->FirstChildElement(sideName));
		menuSprite->setPos(getPos());

		if (CheckIfNull(menuData->FirstChildElement("Spotlight"), "Char Menu: Spotlight Sprite")) exit(-2);
		spotlight = new Sprite::Sprite(menuData->FirstChildElement("Spotlight"));
		spotlight->setPos(getPos() + spotPos);

		if (CheckIfNull(menuData->FirstChildElement("Characters"), "Char Menu: Characters")) exit(-2);
		loadSprites(menuData->FirstChildElement("Characters")->FirstChildElement("Character"));


	}

	CharMenu::~CharMenu()
	{
		delete spotlight;
	}

	void CharMenu::loadSprites(XMLElement const* characterData)
	{
		// Get all player names and spritesheets
		while (characterData != nullptr)
		{
			// Load the character name
			std::string name = characterData->Attribute("name");

			// Load character sprite
			if (CheckIfNull(characterData->FirstChildElement("Sprite"), "Char Menu: Character: Sprite")) exit(-2);
			if (CheckIfNull(characterData->FirstChildElement("Animation"), "Char Menu: Character: Animation")) exit(-2);
			Sprite::AnimatedSprite charSprite(characterData->FirstChildElement("Sprite"), characterData->FirstChildElement("Animation"));

			// Load the text sprite
			if (CheckIfNull(characterData->FirstChildElement("Name"), "Char Menu: Character: Name Sprite")) exit(-2);
			Sprite::Sprite textSprite(characterData->FirstChildElement("Name"));
			textSprite.setPos(getPos() + Vector::Vector2(0, -300));

			// Add everything to lists
			characterNames.push_back(name);
			characterSprites.push_back(charSprite);
			nameSprites.push_back(textSprite);
			maxSize++;

			// Grab the next one
			characterData = characterData->NextSiblingElement("Character");
		}
		
	}

	void CharMenu::draw() const
	{
		menuSprite->draw();
		nameSprites.at(getSelectionNum()).draw();
		characterSprites.at(getSelectionNum()).drawAt(getPos());
		if (finished)
		{
			spotlight->draw();
		}
	}

	void CharMenu::update()
	{
		characterSprites.at(getSelectionNum()).update();
	}

	void CharMenu::moveSelectionNum(int change)
	{
		if (isFinished()) return;
		if (getSelectionNum() + change >= static_cast<int>(maxSize) ||
			getSelectionNum() + change < 0)
		{
			setSelectionNum(maxSize - 1 - getSelectionNum());
		}
		else
		{
			setSelectionNum(getSelectionNum() + change);
		}

		characterSprites.at(getSelectionNum()).changeAnimation(Animation::AnimationEnum::IDLE);
	}

	void CharMenu::setFinished(bool isFinished)
	{
		finished = isFinished;
	}

	void CharMenu::reset()
	{
		setSelectionNum(0);
		setFinished(false);
	}
}

// Stage choosing menu
namespace Menu
{
	StageMenu::StageMenu()
		: Menu("StageMenu")
	{
		finished = false;

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
		while (menuDataName != "StageMenu")
		{
			menuData = menuData->NextSiblingElement();
			if (menuData == nullptr)
			{
				std::cerr << "ERROR: Loading menuData: "
					<< XML_ERROR_FILE_READ_ERROR << std::endl;
				exit(-2);
			}
			menuDataName = menuData->Attribute("name");
		}

		if (CheckIfNull(menuData->FirstChildElement("Stages"), "Stage menu: Stages")) exit(-2);
		if (CheckIfNull(menuData->FirstChildElement("Stages")->FirstChildElement("Stage"), "Stage menu: Stages : Stage")) exit(-2);
		XMLElement *stageData = menuData->FirstChildElement("Stages")->FirstChildElement("Stage");

		// Load each stage data
		while (stageData != nullptr)
		{
			std::string stageName = stageData->Attribute("name");

			if (CheckIfNull(stageData->FirstChildElement("Background"), "Stage menu: Stage: Background")) exit(-2);
			Sprite::Sprite stageSprite(stageData->FirstChildElement("Background"));
			stageSprite.setPos(getPos());

			if (CheckIfNull(stageData->FirstChildElement("Name"), "Stage menu: Stage: NameSprite")) exit(-2);
			Sprite::Sprite nameSprite(stageData->FirstChildElement("Name"));
			nameSprite.setPos(getPos() + Vector::Vector2(0, -305));

			names.push_back(stageName);
			stageSprites.push_back(stageSprite);
			nameSprites.push_back(nameSprite);
			maxSize++;

			stageData = stageData->NextSiblingElement("Stage");
		}
	}

	StageMenu::~StageMenu()
	{

	}

	void StageMenu::draw() const
	{
		stageSprites.at(getSelectionNum()).draw();
		menuSprite->draw();
		nameSprites.at(getSelectionNum()).draw();
	}

	void StageMenu::update()
	{

	}

	void StageMenu::moveSelectionNum(int change)
	{
		if (getSelectionNum() + change >= static_cast<int>(maxSize) ||
			getSelectionNum() + change < 0)
		{
			setSelectionNum(maxSize - 1 - getSelectionNum());
		}
		else
		{
			setSelectionNum(getSelectionNum() + change);
		}
	}

	void StageMenu::reset()
	{
		setSelectionNum(0);
	}
}