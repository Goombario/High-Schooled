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

		int frameWidth = 96;
		int frameHeight = 32;
		float distanceToHP = 38.0f;
		offsetY = schooled::SCREEN_HEIGHT_PX - 9;
		

		float HUDOffsetX;
		if (player->getSide() == Side::LEFT)
		{
			path = schooled::getResourcePath("img") + "HUD_Left.png";
			HUDOffsetX = 0.0f;
			offsetX = distanceToHP;	// The distance from the edge to the HP bar
			side = 1;
		}
		else
		{
			path = schooled::getResourcePath("img") + "HUD_Right.png";
			HUDOffsetX = static_cast<float>(schooled::SCREEN_WIDTH_PX - frameWidth);
			offsetX = schooled::SCREEN_WIDTH_PX - distanceToHP;
			side = -1;
		}

		// Load the HUD
		tempImage = GameEngine::getImageManager()->loadImage(path, frameWidth, frameHeight);
		display = new Sprite::Sprite(tempImage);
		display->move(HUDOffsetX, static_cast<float>(schooled::SCREEN_HEIGHT_PX - frameHeight), 0);

		// Load the HP bar
		tempImage = GameEngine::getImageManager()->loadImage(schooled::getResourcePath("img") + "HPBar.png", 10, 5);
		HPBar = new Sprite::Sprite(tempImage);

		// Load the SP bar
		tempImage = GameEngine::getImageManager()->loadImage(schooled::getResourcePath("img") + "SPBar.png", 7, 5);
		SPBar = new Sprite::Sprite(tempImage);
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
		for (int i = 0; i < player->getCurrentHP(); i++)
		{
			HPBar->drawAt(offsetX + (i * HPBar->getFrameWidth() * side), offsetY);
		}

		for (int i = 0; i < player->getCurrentSP(); i++)
		{
			SPBar->drawAt(offsetX + 1 + (i * SPBar->getFrameWidth() * side), offsetY - SPBar->getFrameHeight());
		}

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

		XMLElement *boardElement = stageData->FirstChildElement("Board");
		if (CheckIfNull(boardElement, "Stage: Board") != XML_SUCCESS) exit(-2);
		boardSprite = new Sprite::Sprite(boardElement);
		boardSprite->move(0, 0, false);
	}

	Stage::~Stage()
	{
		delete background;
		delete boardSprite;

		background = nullptr;
		boardSprite = nullptr;
	}

	void Stage::drawBackground()
	{
		background->draw();
		boardSprite->draw();
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