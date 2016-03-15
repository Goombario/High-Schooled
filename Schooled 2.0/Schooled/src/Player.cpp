#include "Player.h"
#include "Sprite.h"
#include "Board.h"
#include "Image.h"
#include "Animation.h"
#include "Schooled.h"
#include "GameEngine.h"
#include "tinyxml2.h"

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
	}

	Player::Player(const char* playerName, Board::Board* playerBoard)
	{
		setActing(false);

		boardPtr = playerBoard;

		// Load player data from player file
		tinyxml2::XMLDocument data;
		CheckXMLResult(data.LoadFile("../Schooled/PlayerData.xml"));
		XMLNode *pRoot = data.RootElement();
		if (pRoot == nullptr)
		{
			std::cerr << "ERROR: Loading Player data file: " 
				<< XML_ERROR_FILE_READ_ERROR << std::endl;
			exit(-2);
		}

		// Choose the first Player data
		XMLElement *playerData;
		playerData = pRoot->FirstChildElement();

		// Check if the player data loaded
		std::string dataName = playerData->Attribute("name");
		while (dataName != playerName)
		{
			playerData = playerData->NextSiblingElement();
			if (playerData == nullptr)
			{
				std::cerr << "ERROR: Loading playerData: "
					<< XML_ERROR_FILE_READ_ERROR << std::endl;
				exit(-2);
			}
			dataName = playerData->Attribute("name");
		}

		// Load images and animation
		Image::ImageManager *imageManager = GameEngine::getImageManager();
		unsigned int frameWidth, frameHeight;
		XMLElement *spriteData;
		spriteData = playerData->FirstChildElement("Sprite");
		if (spriteData == nullptr)
		{
			std::cerr << "ERROR: Loading Player data file: Sprite. "
				<< XML_ERROR_FILE_READ_ERROR << std::endl;
			exit(-2);
		}

		// Load the image data
		std::string spriteName = spriteData->Attribute("name");
		CheckXMLResult(spriteData->QueryUnsignedAttribute("frameWidth", &frameWidth));
		CheckXMLResult(spriteData->QueryUnsignedAttribute("frameHeight", &frameHeight));
		Image::Image spriteImage = imageManager->loadImage(
			schooled::getResourcePath("img") + spriteName, frameWidth, frameHeight);

		// Load the animation data
		std::string animationName = spriteData->NextSiblingElement()->Attribute("name");
		Animation::AnimationData animationData
			(schooled::getResourcePath("img") + "Image_Data/" + animationName);

		// Set up the sprite
		(*this).sprite = new Sprite::AnimatedSprite(spriteImage, animationData);
		moveSpriteToSide();

		// Loading the token data
		spriteData = playerData->FirstChildElement("Token");
		if (spriteData == nullptr)
		{
			std::cerr << "ERROR: Loading Player data file: Token "
				<< XML_ERROR_FILE_READ_ERROR << std::endl;
			exit(-2);
		}

		std::string imageName = spriteData->Attribute("name");
		CheckXMLResult(spriteData->QueryUnsignedAttribute("width", &frameWidth));
		CheckXMLResult(spriteData->QueryUnsignedAttribute("height", &frameHeight));
		Image::Image tempImage = GameEngine::getImageManager()->loadImage(
			schooled::getResourcePath("img") + imageName,
			frameWidth, frameHeight);
		token = new Sprite::Sprite(tempImage);

		// Load statistics
		XMLElement *statsData = playerData->FirstChildElement("Stats");
		if (statsData == nullptr)
		{
			std::cerr << "ERROR: Loading Player data file: Stats "
				<< XML_ERROR_FILE_READ_ERROR << std::endl;
			exit(-2);
		}
		CheckXMLResult(statsData->FirstChildElement("MaxHP")->QueryIntText(&stats.maxHP));
		CheckXMLResult(statsData->FirstChildElement("MaxSP")->QueryIntText(&stats.maxSP));
		CheckXMLResult(statsData->FirstChildElement("MaxAP")->QueryIntText(&stats.maxAP));
		stats.currentHP = stats.maxHP;
		stats.currentSP = 0;
		stats.currentAP = 0;
		stats.lockedAP = 0;
		
		// Load all attacks
		XMLElement *allAttackData = playerData->FirstChildElement("Attacks");
		if (allAttackData == nullptr)
		{
			std::cerr << "ERROR: Loading Player data file: Attacks "
				<< XML_ERROR_FILE_READ_ERROR << std::endl;
			exit(-2);
		}

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

			(*this).attacks.push_back(tempAttack);
			(*this).numAttacks++;
			attackData = attackData->NextSiblingElement();
		}

		// Load the special data
		XMLElement *specialData = playerData->FirstChildElement("Special");
		if (specialData == nullptr)
		{
			std::cerr << "ERROR: Loading Player data file: Special "
				<< XML_ERROR_FILE_READ_ERROR << std::endl;
			exit(-2);
		}

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
	}

	Player::Player(Player const& source)
	{
		this->sprite = source.sprite;	// Unknown if good
		this->boardPtr = source.boardPtr;
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
		}

		// Decrease the special meter
		(*this).stats.currentSP -= (*this).stats.maxSP;
	}

	void Player::move(Direction d)
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

		// Move the character on the grid
		boardPtr->setPlayerLocation(boardPtr->getPlayerlocation() + change);
		boardPtr->removeToken(boardPtr->getPlayerlocation());
		moveSpriteToSide();
		stats.currentAP = stats.maxAP - stats.lockedAP - boardPtr->updatePath();

		std::cout << "Current AP: " << stats.currentAP << std::endl;
	}

	void Player::moveSpriteToSide()
	{
		float initX;
		float colPos;
		if (boardPtr->getSide() == Side::LEFT)
		{
			colPos = static_cast<float>(boardPtr->getPlayerlocation().X);
			initX = Board::OFFSET_X;
		}
		else
		{
			int offsetRight = Stage::BOARD_WIDTH - 1;
			colPos = static_cast<float>(offsetRight - boardPtr->getPlayerlocation().X);
			initX = Board::CENTER_X + Board::OFFSET_X;
		}
		float rowPos = static_cast<float>(boardPtr->getPlayerlocation().Y);
		sprite->move((initX + 
			(colPos * Board::ROW_WIDTH) +	// The width of the column times the number of columns to move
			(rowPos * Board::ROW_OFFSET)) * schooled::SCALE,	// The row offset due to the perspective
			(Board::OFFSET_Y - (rowPos * Board::ROW_HEIGHT)) * schooled::SCALE, false);
	}

	void Player::startTurn()
	{
		for (auto it = attacks.begin(); it != attacks.end(); it++)
		{
			if ((*it).currentCooldown > 0)
				(*it).currentCooldown--;
		}

		boardPtr->setPlayerFirstPos(boardPtr->getPlayerlocation());

		stats.currentAP = stats.maxAP;
		stats.lockedAP = 0;
	}

	void Player::update()
	{
		sprite->update();

		// Check if the player is still acting
		setActing(sprite->getCurrentAnimation() != Animation::AnimationEnum::IDLE);
	}

	void Player::draw() const
	{
		sprite->draw();
	}

	Side Player::getSide() const { return boardPtr->getSide(); }
}