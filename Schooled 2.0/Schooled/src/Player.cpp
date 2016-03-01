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

		sprite = nullptr;
		boardPtr = nullptr;
	}

	Player::Player(const char* playerName, Board::Board* playerBoard)
	{
		boardPtr = playerBoard;

		// Load player data from player file
		XMLDocument data;
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
		int frameWidth, frameHeight;
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
		CheckXMLResult(spriteData->QueryIntAttribute("frameWidth", &frameWidth));
		CheckXMLResult(spriteData->QueryIntAttribute("frameHeight", &frameHeight));
		Image::Image spriteImage = imageManager->loadImage(
			schooled::getResourcePath("img") + spriteName, frameWidth, frameHeight);

		// Load the animation data
		std::string animationName = spriteData->NextSiblingElement()->Attribute("name");
		int numCol;
		CheckXMLResult(spriteData->NextSiblingElement()
			->QueryIntAttribute("numCol", &numCol));
		Animation::AnimationData animationData(schooled::getResourcePath("img") + "/Image_Data/" + animationName, numCol);

		// Set up the sprite
		(*this).sprite = new Sprite::AnimatedSprite(spriteImage, animationData);

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

	Player::~Player()
	{
		delete sprite;
		delete boardPtr;
	}

	void Player::attack(Player& enemy, int attackNum)
	{
		Attack *currentAttack = &attacks.at(attackNum);

		// Check each space if it is being attacked,
		// And change the player and board accordingly
		for (int w = 0; w < Stage::BOARD_WIDTH; w++)
		{
			for (int h = 0; h < Stage::BOARD_HEIGHT; h++)
			{
				if (currentAttack->range[w][h])
				{
					int modifiedPos = boardPtr->getPlayerlocation()
						+ currentAttack->range[w][h];

					// If the modified value is greater than the size of the board, wrap
					if (modifiedPos >= Stage::BOARD_HEIGHT 
						* Stage::BOARD_WIDTH)
					{
						modifiedPos = (Stage::BOARD_HEIGHT * Stage::BOARD_WIDTH)
							- modifiedPos;
					}

					// If the player is at that position
					if (enemy.boardPtr->getPlayerlocation() == modifiedPos)
					{
						enemy.changeHealth(currentAttack->damage);
					}
					else
					{
						enemy.boardPtr->placeToken(modifiedPos);
					}
				}
			}
		}

		// Update the SP counter, and use the special if filled
		(*this).stats.currentSP += (enemy.boardPtr->checkMatches());
		if ((*this).stats.currentSP >= (*this).stats.maxSP)
		{
			(*this).useSpecial(enemy);
		}
		
		(*this).stats.currentAP--;
		currentAttack = nullptr;
	}

	void Player::changeHealth(int difference)
	{
		stats.currentHP += difference;
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

		//if (ability.setPosition)
		//{
		//	// ???
		//}
	}
}