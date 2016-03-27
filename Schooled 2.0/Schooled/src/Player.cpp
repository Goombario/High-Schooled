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

using namespace tinyxml2;

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

		sprite = nullptr;
		token = nullptr;
		boardPtr = nullptr;

		attackWindow.window = nullptr;
		attackWindow.reticule = nullptr;
	}

	Player::Player(const char* playerName, Board::Board& playerBoard)
	{
		setActing(false);
		boardPtr = &playerBoard;

		// Load the attack window
		Image::Image tempImage;

		if (boardPtr->getSide() == Side::LEFT)
		{
			tempImage = GameEngine::getImageManager()->loadImage(
				schooled::getResourcePath("img") + "AttackL.png", 380, 170);
		}
		else
		{
			tempImage = GameEngine::getImageManager()->loadImage(
				schooled::getResourcePath("img") + "AttackR.png", 380, 170);
		}
		attackWindow.window = new Sprite::Sprite(tempImage);
		attackWindow.reticule = nullptr;


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

		// Loading the token data
		if (CheckIfNull(playerData->FirstChildElement("Token"), "Player: Token")) exit(-2);
		token = new Sprite::Sprite(playerData->FirstChildElement("Token"));

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
			tempAttack.icon = new Sprite::AnimatedSprite(
				attackData->FirstChildElement("Icon"), sharedData->FirstChildElement("IconAnimation"));

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

		moveToSide();
	}

	Player::Player(Player const& source) :
		sprite(source.sprite),
		boardPtr(source.boardPtr)
	{

	}

	Player& Player::operator=(Player const& source)
	{
		this->sprite = source.sprite;	// Unknown if good
		this->boardPtr = source.boardPtr;
		return (*this);
	}

	Player::~Player()
	{
		delete sprite;
		delete token;
		delete arrowSprite;
		delete attackWindow.window;
		delete attackWindow.reticule;

		for (auto it = attacks.begin(); it != attacks.end(); it++)
		{
			delete (*it).icon;
		}
	}

	void Player::attack(Player& enemy, int attackNum)
	{
		Attack *currentAttack = &attacks.at(attackNum);

		// Don't attack if the cooldown is still in effect
		if (currentAttack->currentCooldown != 0)
		{
			return;
		}

		// Check each space if it is being attacked,
		// And change the player and board accordingly
		std::cout << "Attack: " << std::endl;
		for (int h = 0; h < Stage::BOARD_HEIGHT; h++)
		{
			for (int w = 0; w < Stage::BOARD_WIDTH; w++)
			{
				std::cout << currentAttack->range[h][w];
				if (currentAttack->range[h][w])
				{
					COORD pos;
					// If the attack changes with player position,
					if (!currentAttack->isStatic)
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

					// Fire the projectile
					for (auto it = currentAttack->projectiles.begin(); it != currentAttack->projectiles.end(); it++)
					{
						Projectile::Projectile tempProj((*it));
						tempProj.init((*this), enemy, enemy.getBoard()->getTilePos(pos));
						activeProjectiles.push_back(tempProj);
					}

					// If the player is at that position, deal damage
					// Else, place a token.
					if (enemy.boardPtr->getPlayerlocation() == pos)
					{

						enemy.changeHealth(-currentAttack->damage);
					}
					else
					{	
						enemy.boardPtr->placeToken(pos);
					}
				}
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
		(*this).sprite->changeAnimation(static_cast<Animation::AnimationEnum>(attackNum));
		(*this).stats.lockedAP += (*this).boardPtr->updatePath() + 1;
		(*this).boardPtr->setPlayerFirstPos(boardPtr->getPlayerlocation());
		(*this).boardPtr->updatePath();
		stats.currentAP = stats.maxAP - stats.lockedAP;
		currentAttack = nullptr;
		std::cout << "Current AP: " << stats.currentAP << std::endl;

		updateIconCooldown();
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
		// If choosing position, don't use this function.
		if (BattleState::BattleState::Instance()->getCurrentState() == BattleState::State::POS_CHOOSE)
		{
			moveArrow(d);
			return;
		}

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

		// Move the character on the grid
		boardPtr->setPlayerLocation(boardPtr->getPlayerlocation() + change);
		boardPtr->removeToken(boardPtr->getPlayerlocation());
		stats.currentAP = stats.maxAP - stats.lockedAP - boardPtr->updatePath();
		moveToSide();
		/*moveSpriteToSide(*sprite);
		moveSpriteToSide(*glow);*/

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
		moveToSide();
		/*moveSpriteToSide(*arrowSprite);
		moveSpriteToSide(*sprite);
		moveSpriteToSide(*glow);*/
	}

	//void Player::moveSpriteToSide(Sprite::Sprite& s)
	//{
	//	float initX;
	//	float wPos;
	//	if (boardPtr->getSide() == Side::LEFT)
	//	{
	//		wPos = static_cast<float>(boardPtr->getPlayerlocation().X);
	//		initX = Board::OFFSET_X + Board::ROW_OFFSET;
	//	}
	//	else
	//	{
	//		int offsetRight = Stage::BOARD_WIDTH - 1;
	//		wPos = static_cast<float>(offsetRight - boardPtr->getPlayerlocation().X);
	//		initX = Board::CENTER_X + Board::OFFSET_X;
	//	}
	//	float hPos = static_cast<float>(boardPtr->getPlayerlocation().Y - 1);
	//	s.move((initX + 
	//		((wPos + 1) * Board::ROW_WIDTH) +	// The width of the column times the number of columns to move (+1 for alignment)
	//		(hPos * Board::ROW_OFFSET)) * schooled::SCALE,	// The row offset due to the perspective
	//		(Board::OFFSET_Y - (hPos * Board::ROW_HEIGHT)) * schooled::SCALE);
	//}

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

	void Player::update()
	{
		sprite->update();
		arrowSprite->update();

		// Check if the player is still acting
		setActing(sprite->getCurrentAnimation() != Animation::AnimationEnum::IDLE);

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
			//glow->draw();
			//glow->drawAt(getPos());
		}

		// Choosing which sprites to draw based on the current state of the battle
		switch (BattleState::BattleState::Instance()->getCurrentState())
		{
		case BattleState::State::POS_CHOOSE:
			//arrowSprite->draw();
			arrowSprite->drawAt(getPos() + Vector::Vector2(-10, 20));
			break;

		case BattleState::State::ATTACK_CHOOSE:
			attackWindow.window->draw();
			attackWindow.reticule->draw();

		default:
			//sprite->draw();
			sprite->drawAt(getPos());
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
			switch ((*it).currentCooldown)
			{
			case 0:
				(*it).icon->changeAnimation(Animation::AnimationEnum::IDLE);
				break;
			case 1:
				(*it).icon->changeAnimation(Animation::AnimationEnum::COOLDOWN_1);
				break;
			case 2:
				(*it).icon->changeAnimation(Animation::AnimationEnum::COOLDOWN_2);
				break;
			}
		}
	}

	Side Player::getSide() const { return boardPtr->getSide(); }
}