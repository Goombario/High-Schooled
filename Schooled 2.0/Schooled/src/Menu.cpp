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

	}


	Menu::~Menu()
	{
		delete background;

	}

	void Menu::draw() const
	{
		background->drawAt(getPos());
	}

	void Menu::update()
	{
		
	}
}