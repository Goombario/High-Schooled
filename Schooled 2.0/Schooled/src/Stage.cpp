#include "Stage.h"
#include "Player.h"
#include "Sprite.h"
#include "GameEngine.h"
#include "Schooled.h"

#include "tinyxml2.h"
using namespace tinyxml2;

namespace Stage
{
	Stage::Stage(const char* stageName, 
		Player::Player const* p1, Player::Player const* p2) :
		player1(p1),
		player2(p2)
	{
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
		Image::Image tempImage;
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

		HUD = nullptr;
	}

	Stage::~Stage()
	{
		delete background;
		delete token;
		delete HUD;

		background = nullptr;
		token = nullptr;
		HUD = nullptr;
	}

	void Stage::drawBackground()
	{
		background->draw();
	}

	void Stage::drawHUD()
	{
		HUD->draw();
	}

	void Stage::update()
	{

	}
}