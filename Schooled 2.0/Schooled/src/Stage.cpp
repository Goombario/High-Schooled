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
	HUD::HUD()
	{
		player = nullptr;
		display = nullptr;
		HPBar = nullptr;
		SPBar = nullptr;
	}
	HUD::HUD(Player::Player const& p, XMLElement *HUDData)
		: player(&p)
	{
		if (player->getSide() == Side::LEFT)
		{
			if (CheckIfNull(HUDData->FirstChildElement("HUDLeft"), "HUD: HUDLeft")) exit(-2);
			display = new Sprite::Sprite(HUDData->FirstChildElement("HUDLeft"));

			// Set the HUD position
			setPos(Vector::Vector2(
				display->getFrameWidth() / 2.0, 
				display->getFrameWidth() / 2.0));
		}
		else
		{
			if (CheckIfNull(HUDData->FirstChildElement("HUDRight"), "HUD: HUDRight")) exit(-2);
			display = new Sprite::Sprite(HUDData->FirstChildElement("HUDRight"));

			// Set the HUD position
			setPos(Vector::Vector2(
				schooled::SCREEN_WIDTH_PX - (display->getFrameWidth() / 2.0), 
				display->getFrameWidth() / 2.0));
		}

		// Load the HP bar
		if (CheckIfNull(HUDData->FirstChildElement("HPBar"), "HUD: HPBar")) exit(-2);
		HPBar = new Sprite::Sprite(HUDData->FirstChildElement("HPBar"));
		HPBar->setPos(Vector::Vector2(10, 10));

		// Load the SP bar
		if (CheckIfNull(HUDData->FirstChildElement("SPBar"), "HUD: SPBar")) exit(-2);
		SPBar = new Sprite::Sprite(HUDData->FirstChildElement("SPBar"));
		SPBar->setPos(Vector::Vector2(10, 10));
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

	void HUD::draw() const
	{
		for (int i = 0; i < player->getCurrentHP(); i++)
		{
			HPBar->drawAt(
				HPBar->getPos() +
				(*this).getPos() +
				Vector::Vector2(i * HPBar->getFrameWidth() * side, 0));
		}

		for (int i = 0; i < player->getCurrentSP(); i++)
		{
			SPBar->drawAt(
				SPBar->getPos() +
				(*this).getPos() +
				Vector::Vector2(i * SPBar->getFrameWidth() * side, 0));
		}

		display->draw();

		int counter = 0;
		// Draw Attack Icons
		for (auto it = player->attacks.begin(); it != player->attacks.end(); it++)
		{
			(*it).icon.drawAt(
				(*this).getPos() + 
				Vector::Vector2(display->getFrameWidth() * side + counter * 80, 0));
		}
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
		player2(p2)
	{
		Image::Image tempImage;

		// Load player data from player file
		tinyxml2::XMLDocument data;
		CheckXMLResult(data.LoadFile("../Schooled/StageData.xml"));
		XMLNode *pRoot = data.RootElement();
		if (pRoot == nullptr)
		{
			std::cerr << "ERROR: Loading Stage data file: "
				<< XML_ERROR_FILE_READ_ERROR << std::endl;
			exit(-2);
		}

		// Load the HUD
		XMLElement *HUDData;
		HUDData = pRoot->FirstChildElement("HUD");
		p1HUD = HUD(*p1, HUDData);
		p2HUD = HUD(*p2, HUDData);

		XMLElement *stageData;
		stageData = pRoot->FirstChildElement("Stage");

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

		// Load the board highlights
		if (CheckIfNull(stageData->FirstChildElement("Highlight"), "Stage: Highlight")) exit(-2);
		if (CheckIfNull(stageData->FirstChildElement("HighlightAnimation"), "Stage: HighlightAnimation")) exit(-2);

		p1BoardHighlight = new Sprite::AnimatedSprite(
			stageData->FirstChildElement("Highlight"), stageData->FirstChildElement("HighlightAnimation"));
		p1BoardHighlight->setPos(p1->getBoard()->getPos());

		p2BoardHighlight = new Sprite::AnimatedSprite(
			stageData->FirstChildElement("Highlight"), stageData->FirstChildElement("HighlightAnimation"));
		p2BoardHighlight->setPos(p2->getBoard()->getPos());
	}

	Stage::~Stage()
	{
		delete background;
		delete boardSprite;
		delete p1BoardHighlight;
		delete p2BoardHighlight;

		background = nullptr;
		boardSprite = nullptr;
	}

	void Stage::drawBackground()
	{
		background->draw();
		p1BoardHighlight->draw();
		p2BoardHighlight->draw();
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

	void Stage::setActiveBoard(Side s)
	{
		if (s == Side::LEFT)
		{
			p1BoardHighlight->changeAnimation(Animation::AnimationEnum::BLUE_PULSE);
			p2BoardHighlight->changeAnimation(Animation::AnimationEnum::IDLE);
		}
		else
		{
			p1BoardHighlight->changeAnimation(Animation::AnimationEnum::IDLE);
			p2BoardHighlight->changeAnimation(Animation::AnimationEnum::RED_PULSE);
		}
	}
}