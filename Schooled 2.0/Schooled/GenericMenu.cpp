#include "GenericMenu.h"
#include "Sprite.h"
#include "GameEngine.h"
#include "Schooled.h"

#include "tinyxml2.h"
using namespace tinyxml2;

// Generic Menu class
namespace Menu
{
	GenericMenu::GenericMenu(const char* menuName)
	{

		Image::Image tempImage;

		// Load player data from player file
		XMLDocument data;
		CheckXMLResult(data.LoadFile("../Schooled/MenuData.xml"));
		XMLNode *pRoot = data.RootElement();
		if (pRoot == nullptr)
		{
			std::cerr << "ERROR: Loading Stage data file: "
				<< XML_ERROR_FILE_READ_ERROR << std::endl;
			exit(-2);
		}

		XMLElement *menuData;
		menuData = pRoot->FirstChildElement();

		// Check if stage data loaded
		std::string menuDataName = menuData->Attribute("name");
		while (menuDataName != menuName)
		{
			menuData = menuData->NextSiblingElement();
			if (menuData == nullptr)
			{
				std::cerr << "ERROR: Loading stageData: "
					<< XML_ERROR_FILE_READ_ERROR << std::endl;
				exit(-2);
			}
			menuDataName = menuData->Attribute("name");
		}

		XMLElement *menuElement = menuData->FirstChildElement("Background");
		if (menuElement == nullptr)
		{
			std::cerr << "ERROR: Loading Stage data file: Background "
				<< XML_ERROR_FILE_READ_ERROR << std::endl;
			exit(-2);
		}

		std::string imageName;
		unsigned int frameWidth;
		unsigned int frameHeight;
		imageName = menuElement->Attribute("name");
		CheckXMLResult(menuElement->QueryUnsignedAttribute("width", &frameWidth));
		CheckXMLResult(menuElement->QueryUnsignedAttribute("height", &frameHeight));
		tempImage = GameEngine::getImageManager()->loadImage(
			schooled::getResourcePath("img") + imageName,
			frameWidth, frameHeight);
		background = new Sprite::Sprite(tempImage);
		background->move(0, 0, false);

		XMLElement *boardElement = menuData->FirstChildElement("Board");
		if (CheckIfNull(boardElement, "Stage: Board") != XML_SUCCESS) exit(-2);
		menuSprite = new Sprite::Sprite(boardElement);
		menuSprite->move(0, 0, false);
	}

	GenericMenu::~GenericMenu()
	{
		delete background;
		delete menuSprite;

		background = nullptr;
		menuSprite = nullptr;
	}

	void GenericMenu::drawBackground()
	{
		background->draw();
		menuSprite->draw();
	}

	void GenericMenu::drawMenu()
	{
		//p1HUD.draw();
		//p2HUD.draw();
	}

	void GenericMenu::update()
	{
		//p1HUD.update();
		//p2HUD.update();
	}
}