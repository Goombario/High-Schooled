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
	}

	Projectile::Projectile(tinyxml2::XMLElement const* projElement)
	{
		const char* projName = projElement->Attribute("name");
		if (projName == NULL)
		{
			std::cerr << "Error: Projectile name not found" << std::endl;
			exit(-2);
		}

		loadData(projName);

		CheckXMLResult(projElement->QueryDoubleAttribute("delay", &delay));
		offset.setX(projElement->DoubleAttribute("offsetX"));
		offset.setY(projElement->DoubleAttribute("offsetY"));

		CheckXMLResult(projElement->QueryBoolAttribute("hasTarget", &hasTarget));
		if (hasTarget)
		{
			CheckXMLResult(projElement->QueryIntAttribute("targetX", &target.X));
			CheckXMLResult(projElement->QueryIntAttribute("targetY", &target.Y));
			CheckXMLResult(projElement->QueryDoubleAttribute("timeToTarget", &target.timeToTarget));
		}
		else
		{
			target.X = -1;
			target.Y = -1;
			target.timeToTarget = 1;
		}
	}

	void Projectile::loadData(const char* projName)
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
		if (CheckIfNull(projData->FirstChildElement("Sprite"), "Projectile: Sprite")) exit(-2);
		sprite = Sprite::Sprite(projData->FirstChildElement("Sprite"));

		CheckXMLResult(projData->FirstChildElement("Properties")->QueryBoolAttribute("hasGravity", &hasGravity));
		CheckXMLResult(projData->FirstChildElement("Mass")->QueryDoubleText(&mass));

		// Impulse
		double x, y;
		CheckXMLResult(projData->FirstChildElement("Impulse")->QueryDoubleAttribute("X", &x));
		CheckXMLResult(projData->FirstChildElement("Impulse")->QueryDoubleAttribute("Y", &y));
		impulse(Vector::Vector2(x, y));

		// Initial Velocity
		CheckXMLResult(projData->FirstChildElement("Velocity")->QueryDoubleAttribute("X", &x));
		CheckXMLResult(projData->FirstChildElement("Velocity")->QueryDoubleAttribute("Y", &y));
		setVelocity(Vector::Vector2(x, y));
	}

	void Projectile::init(Player::Player const& player, Player::Player const& enemy, Vector::Vector2 const& tilePos)
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
			Vector::Vector2 distance = player.getPos() - tilePos + offset;
			Vector::Vector2 velocity((distance.getX() * -1) / target.timeToTarget, 0);
			setVelocity(Vector::Vector2(velocity.getX(), getVelocity().getY()));
		}
	}

	void Projectile::draw() const
	{
		sprite.drawAt(getPos());
	}

	void Projectile::update()
	{
		if (delay > 0)
		{
			delay -= static_cast<double>(1.0 / schooled::FRAMERATE);
			return;
		}

		if (hasGravity)
		{
			setAcceleration(Vector2(getAcceleration().getX(), getAcceleration().getY() - (9.81)));
		}

		euler();

		// If massively out of bounds
		if (getPos().getX() < 0 - schooled::SCREEN_WIDTH_PX ||
			getPos().getX() > schooled::SCREEN_WIDTH_PX * 2 ||
			getPos().getY() < 0 - schooled::SCREEN_HEIGHT_PX ||
			getPos().getY() > schooled::SCREEN_HEIGHT_PX * 2)
		{
			setActing(false);
		}
	}
}