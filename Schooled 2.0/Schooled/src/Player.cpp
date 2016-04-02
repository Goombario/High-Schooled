#include "Player.h"
#include "Sprite.h"
#include "Board.h"
#include "Image.h"
#include "Animation.h"
#include "Schooled.h"
#include "tinyxml2.h"
#include "BattleState.h"
#include "Projectile.h"
#include "Vector2.h"
#include "Path.h"

#include <stdexcept>

using namespace tinyxml2;

// Icon class
namespace Player
{
	Icon::Icon()
	{
		icon = new Sprite::Sprite();
		cooldown = new Sprite::AnimatedSprite();
		glow = new Sprite::AnimatedSprite();
	}

	Icon::Icon(const char* iconName, Side side)
	{
		// Load Icon data from player file
		tinyxml2::XMLDocument data;
		CheckXMLResult(data.LoadFile("../Schooled/IconData.xml"));
		XMLNode *pRoot = data.RootElement();
		if (pRoot == nullptr)
		{
			std::cerr << "ERROR: Loading Icon data file: "
				<< iconName << std::endl;
			exit(-2);
		}

		// Choose the first icon data
		XMLElement *iconData;
		iconData = pRoot->FirstChildElement("Icon");

		// Check if the icon data is available
		std::string dataName = iconData->Attribute("name");
		while (dataName != iconName)
		{
			iconData = iconData->NextSiblingElement();
			if (iconData == nullptr)
			{
				std::cerr << "ERROR: Loading IconData: " << iconName << std::endl;
				exit(-2);
			}
			dataName = iconData->Attribute("name");
		}

		// Load the default Icon data
		XMLElement *sharedData;
		sharedData = pRoot->FirstChildElement("SharedData");
		if (CheckIfNull(sharedData, "Icon: SharedData")) exit(-2);

		// Load the cooldown
		if (CheckIfNull(sharedData->FirstChildElement("Cooldown"),
			"Icon: SharedData : Cooldown")) exit(-2);
		if (CheckIfNull(sharedData->FirstChildElement("CooldownAnimation"),
			"Icon: SharedData : CooldownAnimation")) exit(-2);
		cooldown = new Sprite::AnimatedSprite(
			sharedData->FirstChildElement("Cooldown"),
			sharedData->FirstChildElement("CooldownAnimation"));

		// Check if glow animation exists
		if (CheckIfNull(sharedData->FirstChildElement("GlowAnimation"),
			"Icon: SharedData : GlowAnimation")) exit(-2);

		// Load the icon
		if (side == Side::LEFT)
		{
			if (CheckIfNull(iconData->FirstChildElement("SpriteLeft"), "Icon : SpriteLeft")) exit(-2);
			icon = new Sprite::Sprite(iconData->FirstChildElement("SpriteLeft"));

			if (CheckIfNull(iconData->FirstChildElement("GlowLeft"), "Icon : GlowLeft")) exit(-2);
			glow = new Sprite::AnimatedSprite(
				iconData->FirstChildElement("GlowLeft"),
				sharedData->FirstChildElement("GlowAnimation"));
		}
		else
		{
			if (CheckIfNull(iconData->FirstChildElement("SpriteRight"), "Icon : SpriteRight")) exit(-2);
			icon = new Sprite::Sprite(iconData->FirstChildElement("SpriteRight"));

			if (CheckIfNull(iconData->FirstChildElement("GlowRight"), "Icon : GlowRight")) exit(-2);
			glow = new Sprite::AnimatedSprite(
				iconData->FirstChildElement("GlowRight"),
				sharedData->FirstChildElement("GlowAnimation"));
		}
	}

	Icon::~Icon()
	{
		delete icon;
		delete cooldown;
		delete glow;
	}

	Icon::Icon(Icon const& other)
	{
		icon = new Sprite::Sprite();
		*icon = *other.icon;

		cooldown = new Sprite::AnimatedSprite();
		*cooldown = *other.cooldown;

		glow = new Sprite::AnimatedSprite();
		*glow = *other.glow;
	}

	Icon& Icon::operator=(Icon const& other)
	{
		if (this == &other) return *this;

		// UNSURE IF MEMORY LEAK
		*icon = *other.icon;
		*cooldown = *other.cooldown;
		*glow = *other.glow;

		return *this;
	}

	void Icon::draw() const
	{
		icon->draw();
		glow->draw();
		cooldown->draw();
	}

	void Icon::drawAt(Vector::Vector2 const& newPos) const
	{
		icon->drawAt(newPos);
		glow->drawAt(newPos);
		cooldown->drawAt(newPos);
	}

	void Icon::update()
	{
		cooldown->update();
		glow->update();
	}

	void Icon::setSelected(bool isSelected)
	{
		if (isSelected)
		{
			glow->changeAnimation(Animation::AnimationEnum::PULSE);
		}
		else
		{
			glow->changeAnimation(Animation::AnimationEnum::IDLE);
		}
	}

	void Icon::setCooldown(int coolNum)
	{
		switch (coolNum)
		{
		case 0:
			cooldown->changeAnimation(Animation::AnimationEnum::IDLE);
			break;

		case 1:
			cooldown->changeAnimation(Animation::AnimationEnum::COOLDOWN_1);
			break;

		case 2:
			cooldown->changeAnimation(Animation::AnimationEnum::COOLDOWN_2);
			break;

		default:
			break;
		};
	}
}

// AttackWindow class
namespace Player
{
	AttackWindow::AttackWindow()
	{
		window = nullptr;
	}

	AttackWindow::AttackWindow(Side s)
		: side(s), attackNum(0)
	{
		// Load the window image and set the offset
		Image::Image tempImage;

		if (side == Side::LEFT)
		{
			tempImage = GameEngine::getImageManager()->loadImage(
				schooled::getResourcePath("img") + "AttackL.png", 380, 170);
			iconOffset = Vector::Vector2(-70, 30);
			windowOffset = Vector::Vector2(250, 100);
		}
		else
		{
			tempImage = GameEngine::getImageManager()->loadImage(
				schooled::getResourcePath("img") + "AttackR.png", 380, 170);
			iconOffset = Vector::Vector2(-120, 30);
			windowOffset = Vector::Vector2(-250, 100);
		}

		window = new Sprite::Sprite(tempImage);
	}

	AttackWindow::AttackWindow(AttackWindow const& other)
		: window(nullptr)
	{
		side = other.side;
		attackNum = other.attackNum;
		iconOffset = other.iconOffset;
		windowOffset = other.windowOffset;
		icons = other.icons;

		window = new Sprite::Sprite(*other.window);
	}

	AttackWindow& AttackWindow::operator=(AttackWindow const& other)
	{
		if (this == &other) return (*this);

		side = other.side;
		attackNum = other.attackNum;
		iconOffset = other.iconOffset;
		windowOffset = other.windowOffset;
		icons = other.icons;

		delete window;
		window = new Sprite::Sprite(*other.window);

		return (*this);
	}

	AttackWindow::~AttackWindow()
	{
		delete window;

		// UNSURE IF NEEDED
		for (auto it = icons.begin(); it != icons.end(); it++)
		{
			(*it) = nullptr;
		}
	}

	void AttackWindow::draw() const
	{
		window->draw();

		unsigned int counter = 0;
		for (auto it = icons.begin(); it != icons.end(); it++)
		{
			// Draw icons in order
			(*it)->drawAt(iconOffset + 
				Vector::Vector2(counter * 80, 0));
			counter++;
		}
	}

	void AttackWindow::drawAtPlayer(Vector::Vector2 const& playerPos) const
	{
		window->drawAt(playerPos + windowOffset);

		unsigned int counter = 0;
		for (auto it = icons.begin(); it != icons.end(); it++)
		{
			// Draw icons in order
			(*it)->drawAt(
				playerPos + windowOffset + iconOffset +
				Vector::Vector2(counter * 90, 0));
			counter++;
		}
	}

	void AttackWindow::update()
	{
		for (auto it = icons.begin(); it != icons.end(); it++)
		{
			(*it)->update();
		}
	}

	void AttackWindow::clearActiveIcon()
	{
		for (auto it = icons.begin(); it != icons.end(); it++)
		{
			(*it)->setSelected(false);
		}
	}

	void AttackWindow::reset()
	{
		clearActiveIcon();
		attackNum = 0;
	}

	void AttackWindow::setActiveIconIndex(int index)
	{
		clearActiveIcon();
		for (unsigned int i = 0; i < icons.size(); i++)
		{
			if (index == i)
			{
				icons[i]->setSelected(true);
				return;
			}
		}
	}

	void AttackWindow::moveActiveIconIndex(int difference)
	{
		// If not an increment, don't do anything
		if (difference != -1 && difference != 1) return;

		icons[attackNum]->setSelected(false);

		attackNum += difference;
		if (attackNum >= static_cast<int>(icons.size()))
		{
			attackNum -= icons.size();
		}
		else if (attackNum < 0)
		{
			attackNum += icons.size();
		}

		icons[attackNum]->setSelected(true);
	}
}

namespace Player
{
	Player::Player()
	{
		stats.currentAP = 0;
		stats.currentHP = 0;
		stats.currentSP = 0;
		stats.maxAP = 0;
		stats.maxHP = 0;
		stats.maxSP = 0;
		setActing(false);

		sprite = new Sprite::AnimatedSprite();
		boardPtr = nullptr;
	}

	Player::Player(const char* playerName, Board::Board& playerBoard)
	{
		setActing(false);
		boardPtr = &playerBoard;

		// Load player data from player file
		tinyxml2::XMLDocument data;
		CheckXMLResult(data.LoadFile("../Schooled/PlayerData.xml"));
		XMLNode *pRoot = data.RootElement();
		if (pRoot == nullptr)
		{
			std::cerr << "ERROR: Loading Player data file: " 
				<< playerName << std::endl;
			exit(-2);
		}

		// Load the default player data
		XMLElement *sharedData;
		sharedData = pRoot->FirstChildElement("SharedData");
		if (CheckIfNull(sharedData, "Player: SharedData")) exit(-2);

		// Load the arrow sprite
		if (CheckIfNull(sharedData->FirstChildElement("Arrow"), "Player: Shared: Arrow")) exit(-2);
		arrowSprite = new Sprite::AnimatedSprite(sharedData->FirstChildElement("Arrow"), sharedData->FirstChildElement("ArrowAnimation"));

		// Choose the first Player data
		XMLElement *playerData;
		playerData = pRoot->FirstChildElement("Player");

		// Check if the player data is available
		std::string dataName = playerData->Attribute("name");
		while (dataName != playerName)
		{
			playerData = playerData->NextSiblingElement();
			if (playerData == nullptr)
			{
				std::cerr << "ERROR: Loading playerData: " << playerName << std::endl;
				exit(-2);
			}
			dataName = playerData->Attribute("name");
		}

		// Set up the sprite
		XMLElement *spriteData;
		spriteData = playerData->FirstChildElement("Sprite");
		if (CheckIfNull(spriteData, "Player: Sprite")) exit(-2);
		(*this).sprite = new Sprite::AnimatedSprite(spriteData, spriteData->NextSiblingElement("Animation"));

		// Load statistics
		XMLElement *statsData = playerData->FirstChildElement("Stats");
		if (CheckIfNull(statsData, "Player: Stats")) exit(-2);

		CheckXMLResult(statsData->FirstChildElement("MaxHP")->QueryIntText(&stats.maxHP));
		CheckXMLResult(statsData->FirstChildElement("MaxSP")->QueryIntText(&stats.maxSP));
		CheckXMLResult(statsData->FirstChildElement("MaxAP")->QueryIntText(&stats.maxAP));
		stats.currentHP = stats.maxHP;
		stats.currentSP = 0;
		stats.currentAP = 0;
		stats.lockedAP = 0;
		
		// Load all attacks
		XMLElement *allAttackData = playerData->FirstChildElement("Attacks");
		if (CheckIfNull(allAttackData, "Player: Attacks")) exit(-2);

		// Load each attack and put it in the player object
		(*this).numAttacks = 0;
		XMLElement * attackData = allAttackData->FirstChildElement();
		while (attackData != nullptr)
		{
			Attack tempAttack;
			tempAttack.currentCooldown = 0;
			tempAttack.name = attackData->Attribute("name");
			CheckXMLResult(attackData->FirstChildElement("Cooldown")
				->QueryIntText(&tempAttack.cooldown));
			CheckXMLResult(attackData->FirstChildElement("Static")
				->QueryBoolText(&tempAttack.isStatic));
			CheckXMLResult(attackData->FirstChildElement("Damage")
				->QueryIntText(&tempAttack.damage));

			// RANGE
			std::string rangeString;
			rangeString = attackData->FirstChildElement("Range")->GetText();
			int rangeCounter = 0;

			// Go through the range string, and determine where the ranges go
			for (int it = 0; it != rangeString.size(); it++)
			{
				if (rangeString[it] != ' ' && rangeString[it] != '\n')
				{
					tempAttack.range[rangeCounter % Stage::BOARD_WIDTH]
						[rangeCounter / Stage::BOARD_HEIGHT] = (rangeString[it] == '1');
					rangeCounter++;
				}
			}

			// Icon loading
			if (CheckIfNull(attackData->FirstChildElement("Icon"), "Player: Attack: Icon")) exit(-2);
			tempAttack.icon = Icon(
				attackData->FirstChildElement("Icon")->Attribute("name"), 
				boardPtr->getSide());

			// Projectile loading
			if (!CheckIfNull(attackData->FirstChildElement("Projectile"), "Player: Attack: Projectile"))
			{
				XMLElement *projData = attackData->FirstChildElement("Projectile");
				while (projData != nullptr)
				{
					tempAttack.projectiles.push_back(Projectile::Projectile(projData));
					projData = projData->NextSiblingElement("Projectile");
				}
			}

			(*this).attacks.push_back(tempAttack);
			(*this).numAttacks++;
			attackData = attackData->NextSiblingElement();
		}

		// Load the attack window
		window = AttackWindow(boardPtr->getSide());
		for (auto it = attacks.begin(); it != attacks.end(); it++)
		{
			window.pushIcon(&(*it).icon);
		}

		// Load the special data
		XMLElement *specialData = playerData->FirstChildElement("Special");
		if (CheckIfNull(specialData, "Player: Special")) exit(-2);

		ability.name = specialData->Attribute("name");
		CheckXMLResult(specialData->FirstChildElement("RemoveAllTokens")
			->QueryBoolText(&ability.removesAllTokens));
		CheckXMLResult(specialData->FirstChildElement("RemoveEnemyTokens")
			->QueryBoolText(&ability.removesEnemyTokens));
		CheckXMLResult(specialData->FirstChildElement("ResetCooldowns")
			->QueryBoolText(&ability.resetCooldowns));
		CheckXMLResult(specialData->FirstChildElement("Heal")
			->QueryIntText(&ability.heal));
		CheckXMLResult(specialData->FirstChildElement("Damage")
			->QueryIntText(&ability.damage));

		setPos(boardPtr->getTilePos(boardPtr->getPlayerlocation()) +
			Vector::Vector2(0, sprite->getFrameHeight() / 2.0));
		//moveToSide();
	}

	Player::Player(Player const& source)
		: sprite(nullptr),
		arrowSprite(nullptr),
		boardPtr(nullptr)
	{
		stats = source.stats;
		numAttacks = source.numAttacks;
		attacks = source.attacks;
		activeProjectiles = source.activeProjectiles;
		ability = source.ability;
		window = source.window;
		sprite = new Sprite::AnimatedSprite(*source.sprite);
		arrowSprite = new Sprite::AnimatedSprite(*source.arrowSprite);

		boardPtr = source.boardPtr;
	}

	Player& Player::operator=(Player const& source)
	{
		if (this == &source) return (*this);

		stats = source.stats;
		numAttacks = source.numAttacks;
		attacks = source.attacks;
		activeProjectiles = source.activeProjectiles;
		ability = source.ability;
		window = source.window;

		*sprite = *source.sprite;
		*arrowSprite = *source.arrowSprite;
		boardPtr = source.boardPtr;

		return (*this);
	}

	Player::~Player()
	{
		delete sprite;
		delete arrowSprite;

		//delete all remaining paths
		for (auto it = paths.begin(); it != paths.end(); it++)
		{
			delete (*it);
		}
	}

	bool Player::attack(Player& enemy)
	{
		Attack *currentAttack = &attacks.at(window.getActiveIconIndex());
		
		// Don't attack if the cooldown is still in effect
		if (currentAttack->currentCooldown != 0)
		{
			return false;
		}

		std::vector<COORD> attackPositions = getAttackPattern(*currentAttack);

		if (currentAttack->isStatic)
		{
			unsigned int counter = 0;
			// Fire the projectile
			for (auto et = currentAttack->projectiles.begin(); et != currentAttack->projectiles.end(); et++)
			{
				Projectile::Projectile tempProj((*et));
				tempProj.init((*this), enemy.getBoard()->getTilePos(attackPositions[counter]));
				activeProjectiles.push_back(tempProj);
				counter++;
			}
		}

		for (auto it = attackPositions.begin(); it != attackPositions.end(); it++)
		{
			if (!currentAttack->isStatic)
			{
				for (auto et = currentAttack->projectiles.begin(); et != currentAttack->projectiles.end(); et++)
				{
					Projectile::Projectile tempProj((*et));
					tempProj.init((*this), enemy.getBoard()->getTilePos((*it)));
					activeProjectiles.push_back(tempProj);
				}
			}

			double delay = (!activeProjectiles.empty()) ? activeProjectiles.back().getTimeToTarget() : 0.0;

			// If the player is at that position, deal damage
			// Else, place a token.
			if (enemy.boardPtr->getPlayerlocation() == (*it))
			{
				enemy.changeHealth(-currentAttack->damage);
				enemy.sprite->pushAnimation(Animation::AnimationEnum::HURT);
				
				enemy.sprite->addDelay(delay);
			}
			else
			{	
				enemy.boardPtr->placeToken((*it), delay);
			}
			
			std::cout << std::endl;
		}

		// Update the SP counter, and use the special if filled
		(*this).stats.currentSP += (enemy.boardPtr->checkMatches());
		if ((*this).stats.currentSP >= (*this).stats.maxSP)
		{
			(*this).useSpecial(enemy);
		}
		
		// Update player stats
		currentAttack->currentCooldown = currentAttack->cooldown;
		(*this).sprite->changeAnimation(static_cast<Animation::AnimationEnum>(window.getActiveIconIndex()));
		(*this).stats.lockedAP += (*this).boardPtr->updatePath() + 1;
		(*this).boardPtr->setPlayerFirstPos(boardPtr->getPlayerlocation());
		(*this).boardPtr->updatePath();
		stats.currentAP = stats.maxAP - stats.lockedAP;
		currentAttack = nullptr;
		std::cout << "Current AP: " << stats.currentAP << std::endl;

		updateIconCooldown();
		return true;
	}

	void Player::changeHealth(int difference)
	{
		stats.currentHP += difference;
	}

	void Player::passTurn()
	{

	}

	void Player::useSpecial(Player& enemy)
	{
		// Clear tokens from one or more players
		if (ability.removesAllTokens)
		{
			(*this).boardPtr->clearTokens();
			enemy.boardPtr->clearTokens();
		}
		else if (ability.removesEnemyTokens)
		{
			// Clear enemy tokens from your board
			(*this).boardPtr->clearTokens();
		}

		// Deal damage to the enemy player
		if (ability.damage > 0)
		{
			enemy.changeHealth(ability.damage * -1);
		}

		// Heal the current player
		if (ability.heal > 0)
		{
			(*this).changeHealth(ability.heal);
		}

		// Reset all cooldowns
		if (ability.resetCooldowns)
		{
			for (auto it = (*this).attacks.begin(); 
				it != (*this).attacks.end(); it++)
			{
				(*it).currentCooldown = 0;
			}
			updateIconCooldown();
		}

		// Decrease the special meter
		(*this).stats.currentSP -= (*this).stats.maxSP;

	}

	void Player::move(Direction d)
	{
		// If out of AP, don't move (Failsafe)
		if ((*this).stats.currentAP <= 0) return;

		// If choosing position, don't use this function.
		if (BattleState::BattleState::Instance()->getCurrentState() == BattleState::State::POS_CHOOSE)
		{
			moveArrow(d);
			return;
		}

		COORD change = { 0, 0 };
		Animation::AnimationEnum newAnim;

		// Determine the value to change by
		switch (d)
		{
		case Direction::UP:
			change.Y--;
			newAnim = Animation::AnimationEnum::UP;
			break;
		case Direction::DOWN:
			change.Y++;
			newAnim = Animation::AnimationEnum::DOWN;
			break;
		case Direction::FORWARD:
			change.X++;
			newAnim = Animation::AnimationEnum::FORWARDS;
			break;
		case Direction::BACKWARD:
			change.X--;
			newAnim = Animation::AnimationEnum::BACKWARDS;
			/*newAnim = (getBoard()->getSide() == Side::LEFT) ?
				Animation::AnimationEnum:: WALK_LEFT:
				Animation::AnimationEnum::WALK_RIGHT;*/
			break;
		}

		int boardSize = Stage::BOARD_HEIGHT * Stage::BOARD_WIDTH;

		// If the movement is outside of the grid, don't move
		if (boardPtr->getPlayerlocation().X + change.X >= Stage::BOARD_WIDTH ||
			boardPtr->getPlayerlocation().X + change.X < 0 ||
			boardPtr->getPlayerlocation().Y + change.Y >= Stage::BOARD_HEIGHT ||
			boardPtr->getPlayerlocation().Y + change.Y < 0)
		{
			return;
		}

		// Move the character on the grid
		boardPtr->setPlayerLocation(boardPtr->getPlayerlocation() + change);
		stats.currentAP = stats.maxAP - stats.lockedAP - boardPtr->updatePath();

		boardPtr->destroyToken(boardPtr->getPlayerlocation());

		// Add a path
		Path::Path *tempPath = new Path::Path(
			(*this).getPos(),
			boardPtr->getTilePos(boardPtr->getPlayerlocation()) + 
			Vector::Vector2(0, sprite->getFrameHeight() / 2.0),
			0.25);

		//Path::Path *newPath = new Path::ProjectilePath(tempPath, 100);

		paths.push_back(tempPath);
		sprite->pushAnimation(newAnim);

		std::cout << "Current AP: " << stats.currentAP << std::endl;
	}

	void Player::moveArrow(Direction d)
	{
		// Determine the value to change by
		COORD change = { 0, 0 };
		switch (d)
		{
		case Direction::UP:
			change.Y--;
			break;
		case Direction::DOWN:
			change.Y++;
			break;
		case Direction::FORWARD:
			change.X++;
			break;
		case Direction::BACKWARD:
			change.X--;
			break;
		}

		int boardSize = Stage::BOARD_HEIGHT * Stage::BOARD_WIDTH;

		// If the movement is outside of the grid, don't move
		if (boardPtr->getPlayerlocation().X + change.X >= Stage::BOARD_WIDTH ||
			boardPtr->getPlayerlocation().X + change.X < 0 ||
			boardPtr->getPlayerlocation().Y + change.Y >= Stage::BOARD_HEIGHT ||
			boardPtr->getPlayerlocation().Y + change.Y < 0)
		{
			return;
		}

		// If the movement is to the center tile, don't move
		if (boardPtr->getPlayerlocation().X + change.X == 1 &&
			boardPtr->getPlayerlocation().Y + change.Y == 1)
		{
			return;
		}

		boardPtr->setPlayerLocation(boardPtr->getPlayerlocation() + change);
		setPos(boardPtr->getTilePos(boardPtr->getPlayerlocation()) +
			Vector::Vector2(0, sprite->getFrameHeight() / 2.0));
		//moveToSide();
	}

	void Player::moveToSide()
	{
		float initX;
		float wPos;
		if (boardPtr->getSide() == Side::LEFT)
		{
			wPos = static_cast<float>(boardPtr->getPlayerlocation().X);
			initX = Board::OFFSET_X + Board::ROW_OFFSET;
		}
		else
		{
			int offsetRight = Stage::BOARD_WIDTH - 1;
			wPos = static_cast<float>(offsetRight - boardPtr->getPlayerlocation().X);
			initX = Board::CENTER_X + Board::OFFSET_X;
		}
		float hPos = static_cast<float>(boardPtr->getPlayerlocation().Y - 1);
		setPos(Vector::Vector2((initX +
			((wPos + 1) * Board::ROW_WIDTH) +	// The width of the column times the number of columns to move (+1 for alignment)
			(hPos * Board::ROW_OFFSET)) * schooled::SCALE,	// The row offset due to the perspective
			(Board::OFFSET_Y - (hPos * Board::ROW_HEIGHT)) * schooled::SCALE));
	}

	void Player::startTurn()
	{
		for (auto it = attacks.begin(); it != attacks.end(); it++)
		{
			if ((*it).currentCooldown > 0)
				(*it).currentCooldown--;
		}
		updateIconCooldown();

		boardPtr->setPlayerFirstPos(boardPtr->getPlayerlocation());

		stats.currentAP = stats.maxAP;
		stats.lockedAP = 0;
	}

	void Player::moveSelectedAttack(int i, Player& enemy)
	{
		window.moveActiveIconIndex(i);
		enemy.boardPtr->clearTiles();
		enemy.boardPtr->setSelectedTiles(getAttackPattern(window.getActiveIconIndex()));
	}

	void Player::initAttackMenu(Player& enemy)
	{
		window.clearActiveIcon();
		window.setActiveIconIndex(0);
		enemy.boardPtr->clearTiles();
		enemy.boardPtr->setSelectedTiles(getAttackPattern(window.getActiveIconIndex()));
	}

	void Player::clearAttackMenu(Player& enemy)
	{
		window.reset();
		enemy.boardPtr->clearTiles();
	}

	std::vector<COORD> const Player::getAttackPattern(Attack const& currentAttack) const
	{
		std::vector<COORD> points;

		for (int h = 0; h < Stage::BOARD_HEIGHT; h++)
		{
			for (int w = 0; w < Stage::BOARD_WIDTH; w++)
			{
				//std::cout << currentAttack.range[h][w];
				if (currentAttack.range[h][w])
				{
					COORD pos;
					// If the attack changes with player position,
					if (!currentAttack.isStatic)
					{
						pos = boardPtr->getPlayerlocation()
							+ COORD{ h, w };

						// If the modified value is greater 
						// than the size of the board, wrap
						if (pos.X >= Stage::BOARD_WIDTH)
						{
							pos.X -= Stage::BOARD_WIDTH;
						}

						if (pos.Y >= Stage::BOARD_HEIGHT)
						{
							pos.Y -= Stage::BOARD_HEIGHT;
						}
					}
					else
					{	// The attack pattern doesn't change
						pos = COORD{ h, w };
					}

					points.push_back(pos);
				}
			}
		}
		return points;
	}

	std::vector<COORD> const Player::getAttackPattern(unsigned int index) const
	{
		if (index >= attacks.size())
		{
			throw std::exception("Error: Out of bounds");
		}
		return getAttackPattern(attacks.at(index));
	}

	void Player::update()
	{
		// Update current path
		if (!paths.empty())
		{
			paths.back()->update((*this));
			if (!paths.back()->isActive())
			{
				delete paths.back();
				paths.pop_back();
				sprite->popAnimation();
			}

		}
		else
		{
			(*this).firstOrder();
		}

		// Update the sprites
		sprite->update();
		arrowSprite->update();
		window.update();

		// Check if the player is still acting
		setActing(
			sprite->getCurrentAnimation() != Animation::AnimationEnum::IDLE ||
			!paths.empty());

		// Update all active projectiles
		std::vector<Projectile::Projectile> tempProj;
		for (auto it = activeProjectiles.begin(); it != activeProjectiles.end(); it++)
		{
			(*it).update();

			if ((*it).isActing())
			{
				tempProj.push_back((*it));
			}
		}
		if (tempProj.size() != activeProjectiles.size()) 
		{ 
			activeProjectiles = tempProj; 
		}
		
	}

	void Player::draw() const
	{
		// If currently selected, draw glow
		if (BattleState::BattleState::Instance()->getCurrentSide() == boardPtr->getSide())
		{
			switch (BattleState::BattleState::Instance()->getCurrentState())
			{
			case BattleState::State::ATTACK_CHOOSE:
				window.drawAtPlayer(getPos());
				break;

			case BattleState::State::POS_CHOOSE:
				arrowSprite->drawAt(getPos() + Vector::Vector2(0, 40));
				break;

			default:
				break;
			}
		}

		// Choosing which sprites to draw based on the current state of the battle
		switch (BattleState::BattleState::Instance()->getCurrentState())
		{
		case BattleState::State::POS_CHOOSE:
			//arrowSprite->drawAt(getPos() + Vector::Vector2(-20, 40));
			break;

		default:
			sprite->drawAt(getPos());
			break;
		}

		// Draw all active projectiles
		for (auto it = activeProjectiles.begin(); it != activeProjectiles.end(); it++)
		{
			(*it).draw();
		}
	}

	void Player::updateIconCooldown()
	{
		for (auto it = attacks.begin(); it != attacks.end(); it++)
		{
			(*it).icon.setCooldown((*it).currentCooldown);
		}
	}

	Side Player::getSide() const { return boardPtr->getSide(); }
}