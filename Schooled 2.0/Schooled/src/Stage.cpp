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
		display = new Sprite::Sprite();
		HPBar = new Sprite::Sprite();
		SPBar = new Sprite::Sprite();
	}

	HUD::HUD(Player::Player const& p, XMLElement *HUDData)
		: player(&p)
	{
		Vector::Vector2 HPPos, SPPos;

		if (player->getSide() == Side::LEFT)
		{
			if (CheckIfNull(HUDData->FirstChildElement("HUDLeft"), "HUD: HUDLeft")) exit(-2);
			display = new Sprite::Sprite(HUDData->FirstChildElement("HUDLeft"));

			side = 1;

			// Set the HUD position
			setPos(Vector::Vector2(
				display->getFrameWidth() / 2.0, 
				schooled::SCREEN_HEIGHT_PX - (display->getFrameHeight() / 2.0)));

			HPPos = Vector::Vector2(-20, 13);
			SPPos = Vector::Vector2(-20, 3);
			iconOffset = Vector::Vector2(0, -display->getFrameHeight());
		}
		else
		{
			if (CheckIfNull(HUDData->FirstChildElement("HUDRight"), "HUD: HUDRight")) exit(-2);
			display = new Sprite::Sprite(HUDData->FirstChildElement("HUDRight"));

			side = -1;

			// Set the HUD position
			setPos(Vector::Vector2(
				schooled::SCREEN_WIDTH_PX - (display->getFrameWidth() / 2.0),
				schooled::SCREEN_HEIGHT_PX - (display->getFrameHeight() / 2.0)));

			HPPos = Vector::Vector2(20, 13);
			SPPos = Vector::Vector2(20, 3);
			iconOffset = Vector::Vector2(-display->getFrameWidth(), -display->getFrameHeight());
		}

		// Load the HP bar
		if (CheckIfNull(HUDData->FirstChildElement("HPBar"), "HUD: HPBar")) exit(-2);
		HPBar = new Sprite::Sprite(HUDData->FirstChildElement("HPBar"));
		HPBar->setPos(HPPos);

		// Load the SP bar
		if (CheckIfNull(HUDData->FirstChildElement("SPBar"), "HUD: SPBar")) exit(-2);
		SPBar = new Sprite::Sprite(HUDData->FirstChildElement("SPBar"));
		SPBar->setPos(SPPos);
	}

	HUD::HUD(HUD const& other)
	{
		display = new Sprite::Sprite();
		*display = *other.display;

		HPBar = new Sprite::Sprite();
		*HPBar = *other.HPBar;

		SPBar = new Sprite::Sprite();
		*SPBar = *other.SPBar;

		iconOffset = other.iconOffset;
		side = other.side;
	}

	HUD& HUD::operator=(HUD const& other)
	{
		if (this == &other) return *this;
		GameObject::operator=(other);
		player = other.player;

		// UNSURE IF MEMORY LEAK
		*display = *other.display;
		*HPBar = *other.HPBar;
		*SPBar = *other.SPBar;

		iconOffset = other.iconOffset;
		side = other.side;

		return (*this);
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
		// Draw all the HP bars
		for (int i = 0; i < player->getCurrentHP(); i++)
		{
			Vector::Vector2 tempVec(HPBar->getPos() +
				(*this).getPos() +
				Vector::Vector2(i * HPBar->getFrameWidth() * side, 0));
			HPBar->drawAt(tempVec);
		}

		// Draw all the SP bars
		for (int i = 0; i < player->getCurrentSP(); i++)
		{
			Vector::Vector2 tempVec(
				SPBar->getPos() +
				(*this).getPos() +
				Vector::Vector2(i * SPBar->getFrameWidth() * side, 0));
			SPBar->drawAt(tempVec);
		}

		// Draw the HUD
		display->drawAt((*this).getPos());
		
		// Draw Attack Icons
		int counter = 0;
		for (auto it = player->attacks.begin(); it != player->attacks.end(); it++)
		{
			Vector::Vector2 tempVec(
				(*this).getPos() + iconOffset +
				Vector::Vector2(counter * 90, 0));
			(*it).icon.drawAt(tempVec);
			counter++;
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

		background = new Sprite::Sprite(stageElement);
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

		// Load the DARKNESS
		if (CheckIfNull(stageData->FirstChildElement("Darkness"), "Stage: Darkness")) exit(-2);
		darkness = new Sprite::Sprite(stageData->FirstChildElement("Darkness"));
		darkness->move(0, 0, false);
		setDark(false);
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
		if (hasDarkness)
		{
			darkness->draw();
		}
		boardSprite->draw();
	}

	void Stage::drawHUD()
	{
		p1HUD.draw();
		p2HUD.draw();
	}

	void Stage::setDark(bool hasDarkness)
	{
		(*this).hasDarkness = hasDarkness;
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