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
		menuData = pRoot->FirstChildElement();

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

		XMLElement *bgElement = menuData->FirstChildElement("Background");
		if (bgElement == nullptr)
		{
			std::cerr << "ERROR: Loading Menu data file: Background "
				<< XML_ERROR_FILE_READ_ERROR << std::endl;
			exit(-2);
		}

		std::string imageName;
		unsigned int frameWidth;
		unsigned int frameHeight;
		imageName = bgElement->Attribute("name");
		CheckXMLResult(bgElement->QueryUnsignedAttribute("width", &frameWidth));
		CheckXMLResult(bgElement->QueryUnsignedAttribute("height", &frameHeight));
		tempImage = GameEngine::getImageManager()->loadImage(
			schooled::getResourcePath("img") + imageName,
			frameWidth, frameHeight);
		background = new Sprite::Sprite(tempImage);
		background->move(0, 0, false);

		XMLElement *menuElement = menuData->FirstChildElement("Menu");
		if (CheckIfNull(menuElement, "Menu: Overlay") != XML_SUCCESS) exit(-2);
		menuSprite = new Sprite::Sprite(menuElement);
		menuSprite->move(0, 0, false);

		XMLElement *menuLightElement = menuData->FirstChildElement("Highlight");
		XMLElement *menuAnimationElement = menuData->FirstChildElement("HighlightAnimation");
		if (CheckIfNull(menuLightElement, "Menu: Sprite") != XML_SUCCESS) exit(-2);
		if (CheckIfNull(menuAnimationElement, "Menu: Animation") != XML_SUCCESS) exit(-2);
		menuAnimSprite = new Sprite::AnimatedSprite(menuLightElement, menuAnimationElement);
		menuAnimSprite->move(0, 0, false);
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

	}

	void GenericMenu::drawMenu()
	{
		menuSprite->draw();
		menuAnimSprite->draw();
	}

	void GenericMenu::update()
	{
		//p1HUD.update();
		//p2HUD.update();
	}
}