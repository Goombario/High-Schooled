#include "Particle.h"
#include "tinyxml2.h"
#include "Schooled.h"

#include <string>

using namespace tinyxml2;

// Particle struct
namespace Particle
{
	void Particle::draw() const
	{
		sprite.drawAt(getPos());
	}

	void Particle::update()
	{
		euler();
		boundingBox.setPos(getPos());
		life -= static_cast<float>(1.0 / schooled::FRAMERATE);
	}
}

// Emitter class
namespace Particle
{
	Emitter::Emitter()
	{
		maxLife = 0;
		minLife = 0;
		maxAngle = 0;
		minAngle = 0;
		minSpeed = 0;
		maxSpeed = 0;
		particleMass = 0;
		hasGravity = false;
		colType = CollideType::NONE;
	}

	Emitter::Emitter(const char* emitterName)
	{
		tinyxml2::XMLDocument data;
		CheckXMLResult(data.LoadFile("../Schooled/EmitterData.xml"));
		XMLNode *pRoot = data.RootElement();
		if (pRoot == nullptr)
		{
			std::cerr << "ERROR: Loading Emitter data file: "
				<< emitterName << std::endl;
			exit(-2);
		}

		// Choose the first Emitter data
		XMLElement *emitterData;
		emitterData = pRoot->FirstChildElement("Emitter");

		// Check if the emitter data is available
		std::string dataName = emitterData->Attribute("name");
		while (dataName != emitterName)
		{
			emitterData = emitterData->NextSiblingElement();
			if (emitterData == nullptr)
			{
				std::cerr << "ERROR: Loading emitterData: " << emitterName << std::endl;
				exit(-2);
			}
			dataName = emitterData->Attribute("name");
		}

		// Load the particle data
		if (CheckIfNull(emitterData->FirstChildElement("Particle"), "Emitter: Particle")) exit(-2);
		CheckXMLResult(emitterData->FirstChildElement("Particle")->QueryDoubleAttribute("mass", &mass));
		CheckXMLResult(emitterData->FirstChildElement("Particle")->QueryDoubleAttribute("minLife", &minLife));
		CheckXMLResult(emitterData->FirstChildElement("Particle")->QueryDoubleAttribute("maxLife", &maxLife));
		CheckXMLResult(emitterData->FirstChildElement("Particle")->QueryDoubleAttribute("minSpeed", &minSpeed));
		CheckXMLResult(emitterData->FirstChildElement("Particle")->QueryDoubleAttribute("maxSpeed", &maxSpeed));

		// Load the available angles
		if (CheckIfNull(emitterData->FirstChildElement("Angle"), "Emitter: Angle")) exit(-2);
		CheckXMLResult(emitterData->FirstChildElement("Angle")->QueryDoubleAttribute("min", &minAngle));
		CheckXMLResult(emitterData->FirstChildElement("Angle")->QueryDoubleAttribute("max", &maxAngle));

		// Load the properties
		if (CheckIfNull(emitterData->FirstChildElement("Properties"), "Emitter: Properties")) exit(-2);
		CheckXMLResult(emitterData->FirstChildElement("Properties")->QueryBoolAttribute("hasGravity", &hasGravity));

		// Choose the collision type
		std::string collideTypeString = emitterData->FirstChildElement("Properties")->Attribute("collideType");
		if (collideTypeString == "BOUNCE") colType = CollideType::BOUNCE;
		else if (collideTypeString == "ERASE") colType = CollideType::ERASE;
		else colType = CollideType::NONE;
		

		// Load all sprites
		XMLElement *spriteData = emitterData->FirstChildElement("Sprite");
		while (spriteData != nullptr)
		{
			spriteList.push_back(Sprite::Sprite(spriteData));
			spriteData = spriteData->NextSiblingElement("Sprite");
		}
	}

	void Emitter::draw() const
	{
		for (auto it = particleList.begin(); it != particleList.end(); it++)
		{
			(*it).draw();
		}
	}

	void Emitter::update()
	{
		std::vector<Particle> tempParticles;
		for (auto it = particleList.begin(); it != particleList.end(); it++)
		{
			if (hasGravity)
			{
				(*it).setAcceleration(Vector2(
					(*it).getAcceleration().getX(), 
					(*it).getAcceleration().getY() - (schooled::GRAVITY)));
			}
			(*it).update();

			if ((*it).life > 0.0)
			{
				tempParticles.push_back((*it));
			}
		}

		if (tempParticles.size() != particleList.size())
		{
			particleList = tempParticles;
		}
	}

	void Emitter::generate(int numParticles)
	{
		for (int i = 0; i < numParticles; i++)
		{
			for (auto it = spriteList.begin(); it != spriteList.end(); it++)
			{
				Particle tempParticle;
				tempParticle.sprite = (*it);
				tempParticle.boundingBox = Collision::AABB(tempParticle.getPos(),
					tempParticle.sprite.getFrameWidth(),
					tempParticle.sprite.getFrameHeight());
				tempParticle.life = static_cast<float>((rand() % static_cast<int>(maxLife - minLife)) + minLife) / 10.0f;
				tempParticle.mass = static_cast<float>(mass);

				// Rotate the velocity
				double angle = (rand() % static_cast<int>(maxAngle - minAngle)) + minAngle;
				tempParticle.setVelocity(Vector::Vector2((rand() % static_cast<int>(maxSpeed - minSpeed)) + minSpeed, 0.0));
				tempParticle.setVelocity(tempParticle.getVelocity().getRotation(angle));
				tempParticle.setPos(getPos());

				particleList.push_back(tempParticle);
			}
		}
	}

	void Emitter::testCollision(Collision::AABB const& box1)
	{
		if (colType == CollideType::NONE) return;

		// Test all particles to see if they hit the object
		Collision::Collision result;
		std::vector<Particle> tempList;
		for (auto it = particleList.begin(); it != particleList.end(); it++)
		{
			result = (*it).boundingBox.testAABB(box1);
			tempList.push_back((*it));
			// If the object is colliding and overlapping
			if (result.status && (result.overlap.getX() != 0.0 || result.overlap.getY() != 0.0))
			{
				// Correct the position
				result = (*it).boundingBox.testAABB(box1);
				Vector::Vector2 correction = -1 * result.overlap.getProjection((*it).getVelocity());

				(*it).setPos((*it).getPos() - correction);

				if (result.overlap.getX() == 0.0 && result.overlap.getY() == 0.0)
				{
					return;
				}
				

				switch (colType)
				{
				case CollideType::BOUNCE:
					(*it).setVelocity(Vector::Vector2((*it).getVelocity().getX(), -(*it).getVelocity().getY() / 2.0));
					break;
				case CollideType::ERASE:
					tempList.pop_back();
					break;
				default:
					break;
				}
			}
		}
		if (tempList.size() != particleList.size())
		{
			particleList = tempList;
		}
	}
}