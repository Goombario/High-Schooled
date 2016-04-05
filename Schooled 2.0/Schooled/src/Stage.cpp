#include "Stage.h"
#include "Player.h"
#include "Sprite.h"
#include "GameEngine.h"
#include "Schooled.h"

#include <string>
#include "tinyxml2.h"
using namespace tinyxml2;

// HUD class
namespace Stage
{
	HUD::HUD()
	{
		player = nullptr;
		display = new Sprite::Sprite();
		HPBar = new Sprite::AnimatedSprite();
		SPBar = new Sprite::Sprite();
		APBar = new Sprite::Sprite();
		sideBar = new Sprite::Sprite();
		downBar = new Sprite::Sprite();
	}

	HUD::HUD(Player::Player const& p, XMLElement *HUDData)
		: player(&p)
	{
		Vector::Vector2 HPPos, SPPos, APPos;
		const char *sideBarName, *hpName, *spName;

		if (player->getSide() == Side::LEFT)
		{
			if (CheckIfNull(HUDData->FirstChildElement("HUDLeft"), "HUD: HUDLeft")) exit(-2);
			display = new Sprite::Sprite(HUDData->FirstChildElement("HUDLeft"));

			side = 1;

			// Set the HUD position
			setPos(Vector::Vector2(
				display->getFrameWidth() / 2.0, 
				schooled::SCREEN_HEIGHT_PX - (display->getFrameHeight() / 2.0)));

			HPPos = Vector::Vector2(85, 40);
			SPPos = Vector::Vector2(85, -10);
			APPos = Vector::Vector2(-80, -65);
			iconOffset = Vector::Vector2(-20, display->getFrameHeight() - schooled::SCREEN_HEIGHT_PX - 30);
			hpName = "HPBarLeft";
			spName = "SPBarLeft";
			sideBarName = "SideBarLeft";
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

			HPPos = Vector::Vector2(-85, 40);
			SPPos = Vector::Vector2(-85, -10);
			APPos = Vector::Vector2(85, -65);
			iconOffset = Vector::Vector2(-display->getFrameWidth() + 20, display->getFrameHeight() - schooled::SCREEN_HEIGHT_PX - 30);
			hpName = "HPBarRight";
			spName = "SPBarRight";
			sideBarName = "SideBarRight";
		}

		// Load the HP bar
		if (CheckIfNull(HUDData->FirstChildElement(hpName), "HUD: HPBar")) exit(-2);
		if (CheckIfNull(HUDData->FirstChildElement("HPAnimation"), "HUD: HPAnimation")) exit(-2);
		HPBar = new Sprite::AnimatedSprite(HUDData->FirstChildElement(hpName), HUDData->FirstChildElement("HPAnimation"));
		HPBar->setPos(HPPos);

		// Load the SP bar
		if (CheckIfNull(HUDData->FirstChildElement(spName), "HUD: SPBar")) exit(-2);
		SPBar = new Sprite::Sprite(HUDData->FirstChildElement(spName));
		SPBar->setPos(SPPos);

		// Load the AP bar
		if (CheckIfNull(HUDData->FirstChildElement("APBar"), "HUD: APBar")) exit(-2);
		APBar = new Sprite::Sprite(HUDData->FirstChildElement("APBar"));
		APBar->setPos(APPos);

		// Load the Down bar
		if (CheckIfNull(HUDData->FirstChildElement("DownBar"), "HUD: DownBar")) exit(-2);
		downBar = new Sprite::Sprite(HUDData->FirstChildElement("DownBar"));

		// Load the Side bar
		if (CheckIfNull(HUDData->FirstChildElement(sideBarName), "HUD: SideBar")) exit(-2);
		sideBar = new Sprite::Sprite(HUDData->FirstChildElement(sideBarName));
	}

	HUD::HUD(HUD const& other)
	{
		display = new Sprite::Sprite();
		*display = *other.display;

		HPBar = new Sprite::AnimatedSprite();
		*HPBar = *other.HPBar;

		SPBar = new Sprite::Sprite();
		*SPBar = *other.SPBar;

		sideBar = new Sprite::Sprite();
		*sideBar = *other.sideBar;

		downBar = new Sprite::Sprite();
		*downBar = *other.downBar;

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
		*APBar = *other.APBar;
		*sideBar = *other.sideBar;
		*downBar = *other.downBar;

		iconOffset = other.iconOffset;
		side = other.side;

		return (*this);
	}

	HUD::~HUD()
	{
		delete display;
		delete HPBar;
		delete SPBar;
		delete APBar;
		delete downBar;
		delete sideBar;

		display = nullptr;
		HPBar = nullptr;
		SPBar = nullptr;
	}

	void HUD::draw() const
	{
		// Draw all the HP background bars
		for (int i = 0; i < player->getMaxHP(); i++)
		{
			Vector::Vector2 tempVec(HPBar->getPos() +
				(*this).getPos() +
				Vector::Vector2((i * (HPBar->getFrameWidth() - 20) * side), 0));
			sideBar->drawAt(tempVec);
		}

		// Draw all the HP bars
		for (int i = 0; i < player->getCurrentHP(); i++)
		{
			Vector::Vector2 tempVec(HPBar->getPos() +
				(*this).getPos() +
				Vector::Vector2(i * (HPBar->getFrameWidth() - 20) * side, 0));
			HPBar->drawAt(tempVec);
		}

		// Draw all the SP background bars
		for (int i = 0; i < player->getMaxSP(); i++)
		{
			Vector::Vector2 tempVec(SPBar->getPos() +
				(*this).getPos() +
				Vector::Vector2((i * (SPBar->getFrameWidth() - 20) * side), 0));
			sideBar->drawAt(tempVec);
		}

		// Draw all the SP bars
		for (int i = 0; i < player->getCurrentSP(); i++)
		{
			Vector::Vector2 tempVec(
				SPBar->getPos() +
				(*this).getPos() +
				Vector::Vector2(i * (SPBar->getFrameWidth() - 20) * side, 0));
			SPBar->drawAt(tempVec);
		}

		// Draw all the AP background bars
		for (int i = 1; i <= player->getMaxAP(); i++)
		{
			Vector::Vector2 tempVec(APBar->getPos() +
				(*this).getPos() +
				Vector::Vector2(side * APBar->getFrameWidth(), i * -(APBar->getFrameHeight() - 20)));
			downBar->drawAt(tempVec);
		}

		// Draw all the AP bars
		for (int i = 1; i <= player->getCurrentAP(); i++)
		{
			Vector::Vector2 tempVec(
				APBar->getPos() +
				(*this).getPos() +
				Vector::Vector2(side * APBar->getFrameWidth(), i * -(APBar->getFrameHeight() - 20)));
			APBar->drawAt(tempVec);
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
			(*it).icon.drawNoGlowAt(tempVec);
			counter++;
		}
	}

	void HUD::update()
	{
		HPBar->update();
	}

	void HUD::updateHPColour()
	{
		if (player->getCurrentHP() < player->getMaxHP() / 4)
		{
			HPBar->changeAnimation(Animation::AnimationEnum::HP_LOW);
		}
		else if (player->getCurrentHP() < player->getMaxHP() / 2)
		{
			HPBar->changeAnimation(Animation::AnimationEnum::HP_MED);
		}
		else
		{
			HPBar->changeAnimation(Animation::AnimationEnum::IDLE);
		}
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

	void Stage::updateHPColour()
	{
		p1HUD.updateHPColour();
		p2HUD.updateHPColour();
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