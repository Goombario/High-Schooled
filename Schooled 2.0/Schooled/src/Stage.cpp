#include "Stage.h"
#include "Player.h"
#include "Sprite.h"
#include "GameEngine.h"
#include "Schooled.h"

#include "tinyxml2.h"
using namespace tinyxml2;

// HUD class
namespace Stage
{

	HUD::HUD(Player::Player const& p)
		: player(&p)
	{
		Image::Image tempImage;
		std::string path;
		float offsetX;
		int frameWidth = 96;
		int frameHeight = 32;
		int HPOffsetX;
		int HPOffsetY = schooled::SCREEN_HEIGHT_PX - 11;
		if (player->getSide() == Side::LEFT)
		{
			path = schooled::getResourcePath("img") + "HUD_Left.png";
			offsetX = 0.0f;
			HPOffsetX = 34;
		}
		else
		{
			path = schooled::getResourcePath("img") + "HUD_Right.png";
			offsetX = static_cast<float>(schooled::SCREEN_WIDTH_PX - frameWidth);
			HPOffsetX = schooled::SCREEN_WIDTH_PX - 34 - 50;
		}

		tempImage = GameEngine::getImageManager()->loadImage(path, frameWidth, frameHeight);
		display = new Sprite::Sprite(tempImage);
		display->move(offsetX, static_cast<float>(schooled::SCREEN_HEIGHT_PX - frameHeight), 0);

		tempImage = GameEngine::getImageManager()->loadImage(schooled::getResourcePath("img") + "HPBar.png", 50, 5);
		HPBar = new Sprite::Sprite(tempImage);
		HPBar->move(static_cast<float>(HPOffsetX), static_cast<float>(HPOffsetY), false);

		SPBar = nullptr;
	}

	HUD::~HUD()
	{
		delete display;
		delete HPBar;
		delete SPBar;

		display = nullptr;
		HPBar = nullptr;
		SPBar = nullptr;
	}

	void HUD::draw()
	{
		HPBar->draw();
		//SPBar->draw();
		display->draw();
	}

	void HUD::update()
	{
		
	}
	
}

// Stage class
namespace Stage
{
	Stage::Stage(const char* stageName, 
		Player::Player const* p1, Player::Player const* p2) :
		player1(p1),
		player2(p2),
		p1HUD(*player1),
		p2HUD(*player2)
	{
		
		Image::Image tempImage;

		// Load player data from player file
		XMLDocument data;
		CheckXMLResult(data.LoadFile("../Schooled/StageData.xml"));
		XMLNode *pRoot = data.RootElement();
		if (pRoot == nullptr)
		{
			std::cerr << "ERROR: Loading Stage data file: "
				<< XML_ERROR_FILE_READ_ERROR << std::endl;
			exit(-2);
		}

		XMLElement *stageData;
		stageData = pRoot->FirstChildElement();

		// Check if stage data loaded
		std::string stageDataName = stageData->Attribute("name");
		while (stageDataName != stageName)
		{
			stageData = stageData->NextSiblingElement();
			if (stageData == nullptr)
			{
				std::cerr << "ERROR: Loading stageData: "
					<< XML_ERROR_FILE_READ_ERROR << std::endl;
				exit(-2);
			}
			stageDataName = stageData->Attribute("name");
		}

		XMLElement *stageElement = stageData->FirstChildElement("Background");
		if (stageElement == nullptr)
		{
			std::cerr << "ERROR: Loading Stage data file: Background "
				<< XML_ERROR_FILE_READ_ERROR << std::endl;
			exit(-2);
		}

		std::string imageName;
		unsigned int frameWidth;
		unsigned int frameHeight;
		imageName = stageElement->Attribute("name");
		CheckXMLResult(stageElement->QueryUnsignedAttribute("width", &frameWidth));
		CheckXMLResult(stageElement->QueryUnsignedAttribute("height", &frameHeight));
		tempImage = GameEngine::getImageManager()->loadImage(
			schooled::getResourcePath("img") + imageName, 
			frameWidth, frameHeight);
		background = new Sprite::Sprite(tempImage);
		background->move(0, 0, false);

		stageElement = stageData->FirstChildElement("Token");
		if (stageElement == nullptr)
		{
			std::cerr << "ERROR: Loading Stage data file: Token "
				<< XML_ERROR_FILE_READ_ERROR << std::endl;
			exit(-2);
		}

		imageName = stageElement->Attribute("name");
		CheckXMLResult(stageElement->QueryUnsignedAttribute("width", &frameWidth));
		CheckXMLResult(stageElement->QueryUnsignedAttribute("height", &frameHeight));
		tempImage = GameEngine::getImageManager()->loadImage(
			schooled::getResourcePath("img") + imageName,
			frameWidth, frameHeight);
		token = new Sprite::Sprite(tempImage);
		token->move(0, 0, false);
	}

	Stage::~Stage()
	{
		delete background;
		delete token;

		background = nullptr;
		token = nullptr;
	}

	void Stage::drawBackground()
	{
		background->draw();
	}

	void Stage::drawHUD()
	{
		p1HUD.draw();
		p2HUD.draw();
	}

	void Stage::update()
	{
		p1HUD.update();
		p2HUD.update();
	}
}