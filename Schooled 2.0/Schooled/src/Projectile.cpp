#include "Projectile.h"
#include "Vector2.h"
#include "Sprite.h"
#include "Schooled.h"
#include "Player.h"
#include <iostream>
#include <Windows.h>

using namespace tinyxml2;

namespace Projectile
{
	Projectile::Projectile()
	{
		sprite = nullptr;
		setActing(true);
	}

	Projectile::Projectile(tinyxml2::XMLElement const* projElement, Side s)
	{
		timeElapsed = 0.0;

		const char* projName = projElement->Attribute("name");
		if (projName == NULL)
		{
			std::cerr << "Error: Projectile name not found" << std::endl;
			exit(-2);
		}

		loadData(projName, s);

		CheckXMLResult(projElement->QueryDoubleAttribute("delay", &delay));
		offset.setX(projElement->DoubleAttribute("offsetX"));
		offset.setY(projElement->DoubleAttribute("offsetY"));

		CheckXMLResult(projElement->QueryBoolAttribute("hasTarget", &hasTarget));
		if (hasTarget)
		{
			CheckXMLResult(projElement->QueryDoubleAttribute("timeToTarget", &timeToTarget));
		}
		else
		{
			timeToTarget = 1;
		}

		boundingBox = Collision::AABB(getPos(), sprite.getFrameWidth(), sprite.getFrameHeight());
		setActing(true);
	}

	void Projectile::loadData(const char* projName, Side s)
	{
		// Load the projectile data file
		tinyxml2::XMLDocument data;
		CheckXMLResult(data.LoadFile("../Schooled/Projectiles.xml"));
		XMLNode *pRoot = data.RootElement();
		if (pRoot == nullptr)
		{
			std::cerr << "ERROR: Loading Projectile data file: "
				<< projName << std::endl;
			exit(-2);
		}

		// Choose the first Projectile data
		XMLElement *projData;
		projData = pRoot->FirstChildElement("Projectile");

		// Check if the projectile data is available
		std::string dataName = projData->Attribute("name");
		while (dataName != projName)
		{
			projData = projData->NextSiblingElement();
			if (projData == nullptr)
			{
				std::cerr << "ERROR: Projectile not found: " << projName << std::endl;
				exit(-2);
			}
			dataName = projData->Attribute("name");
		}

		// Load sprite data
		const char* spriteSide = (s == Side::LEFT) ? "SpriteLeft" : "SpriteRight";
		if (CheckIfNull(projData->FirstChildElement(spriteSide), "Projectile: Sprite")) exit(-2);
		sprite = Sprite::Sprite(projData->FirstChildElement(spriteSide));

		// Properties
		CheckXMLResult(projData->FirstChildElement("Properties")->QueryBoolAttribute("hasGravity", &hasGravity));
		if (projData->FirstChildElement("Properties")->Attribute("hasCollision"))
			CheckXMLResult(projData->FirstChildElement("Properties")->QueryBoolAttribute("hasCollision", &collides));
		CheckXMLResult(projData->FirstChildElement("Mass")->QueryDoubleText(&mass));

		// Initial Velocity
		double x, y;
		CheckXMLResult(projData->FirstChildElement("Velocity")->QueryDoubleAttribute("X", &x));
		CheckXMLResult(projData->FirstChildElement("Velocity")->QueryDoubleAttribute("Y", &y));
		setVelocity(Vector::Vector2(x, y));

		// Impulse
		CheckXMLResult(projData->FirstChildElement("Impulse")->QueryDoubleAttribute("X", &x));
		CheckXMLResult(projData->FirstChildElement("Impulse")->QueryDoubleAttribute("Y", &y));
		impulse(Vector::Vector2(x, y));
	}

	void Projectile::init(Player::Player const& player, Vector::Vector2 const& tilePos)
	{
		if (player.getSide() == Side::LEFT)
		{
			(*this).setPos(player.getPos() + offset);

		}
		else
		{
			(*this).setPos(player.getPos() + Vector::Vector2(offset.getX() * -1, offset.getY()));
			(*this).setVelocity(Vector::Vector2(
				getVelocity().getX() * -1, getVelocity().getY()));
		}

		if (hasTarget)
		{
			Vector::Vector2 distance = player.getPos() - tilePos;
			Vector::Vector2 velocity;
			if (hasGravity)
			{
				velocity = Vector::Vector2((distance.getX() * -1.0) / timeToTarget, getVelocity().getY());
			}
			else
			{
				velocity = Vector::Vector2(
					(distance.getX() * -1.0) / timeToTarget, 
					(distance.getY() * -1.0) / timeToTarget);
			}
			setVelocity(velocity);
		}
	}

	void Projectile::draw() const
	{
		if (delay == 0)
		{
			sprite.drawAt(getPos());
		}
	}

	void Projectile::update()
	{
		if (delay > 0)
		{
			delay -= static_cast<double>(1.0 / schooled::FRAMERATE);
			return;
		}

		timeElapsed += static_cast<double>(1.0 / schooled::FRAMERATE);
		if (timeElapsed >= timeToTarget)
		{
			//std::cout << "Reached Target" << std::endl;
			setActing(false);
		}

		if (hasGravity)
		{
			setAcceleration(Vector2(getAcceleration().getX(), getAcceleration().getY() - (schooled::GRAVITY)));
		}

		euler();
		boundingBox.setPos(getPos());

		// If massively out of bounds
		if (getPos().getX() < 0 - schooled::SCREEN_WIDTH_PX ||
			getPos().getX() > schooled::SCREEN_WIDTH_PX * 2 ||
			getPos().getY() < 0 - schooled::SCREEN_HEIGHT_PX ||
			getPos().getY() > schooled::SCREEN_HEIGHT_PX * 2)
		{
			setActing(false);
		}
	}

	void Projectile::checkCollision(Collision::AABB const& box1)
	{
		if (!collides) return;

		// Test if it hits the object
		Collision::Collision result = boundingBox.testAABB(box1);

		// If the object is colliding and overlapping
		if (result.status && (result.overlap.getX() != 0.0 || result.overlap.getY() != 0.0))
		{
			Vector::Vector2 correction = -1 * result.overlap.getProjection(getVelocity());
			setPos(getPos() - correction);
			setVelocity(-1 * getVelocity());
		}
		// If the object is just colliding
		else if (result.status)
		{
			setVelocity(-1 * getVelocity());
		}
	}
}