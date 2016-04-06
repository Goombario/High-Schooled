#include "Tutorial.h"
#include "Sprite.h"
#include "Schooled.h"
#include "tinyxml2.h"

using namespace tinyxml2;

namespace Tutorial
{
	Tutorial::Tutorial()
	{
		ready = false;
		slideNum = 0;
		slides.push_back(Sprite::AnimatedSprite());
	}

	Tutorial::Tutorial(const char* playerName, Side s)
	{
		ready = false;
		slideNum = 0;
		const char* sideName;

		// Get the position of the slides based on the side
		if (s == Side::LEFT)
		{
			setPos(Vector::Vector2(
				schooled::SCREEN_WIDTH_PX / 4, 
				schooled::SCREEN_HEIGHT_PX / 2));
			sideName = "Left";
		}
		else
		{
			setPos(Vector::Vector2(
				(schooled::SCREEN_WIDTH_PX / 4) * 3,
				schooled::SCREEN_HEIGHT_PX / 2));
			sideName = "Right";
		}

		// Load tutorial data from XML file
		tinyxml2::XMLDocument data;
		CheckXMLResult(data.LoadFile("../Schooled/TutorialData.xml"));
		XMLNode *pRoot = data.RootElement();
		if (pRoot == nullptr)
		{
			std::cerr << "ERROR: Loading Tutorial data file" << std::endl;
			exit(-2);
		}

		XMLElement *slideElement = pRoot->FirstChildElement("Slide");
		if (CheckIfNull(slideElement, "No slide exists")) exit(-2);

		// Go through all the slides and add them to the vector
		
		while (slideElement != nullptr)
		{	
			Sprite::AnimatedSprite tempSprite;
			XMLElement *spriteElement;

			if (slideElement->FirstChildElement(playerName) != nullptr)
			{
				spriteElement = slideElement->FirstChildElement(playerName);
			}
			else
			{
				spriteElement = slideElement->FirstChildElement(sideName);
			}

			tempSprite = Sprite::AnimatedSprite(spriteElement, slideElement->FirstChildElement("Animation"));
			tempSprite.setPos(getPos());
			slides.push_back(tempSprite);
			slideElement = slideElement->NextSiblingElement("Slide");
		}
	}

	void Tutorial::setReady(bool isReady)
	{
		if (isReady && slideNum == slides.size() - 2)
		{
			ready = true;
			slideNum = slides.size() - 1;
		}
		/*else if (isReady)
		{
			ready = false;
			slideNum = slides.size() - 2;
		}*/
		else if (!isReady && slideNum == slides.size() - 1)
		{
			ready = false;
			slideNum = slides.size() - 2;
		}
		else
		{
			ready = false;
		}
	}

	void Tutorial::moveSlide(int change)
	{
		// If out of bounds of normal slides, don't move
		if (slideNum + change >= slides.size() - 1 ||
			slideNum + change < 0)
			return;

		slideNum += change;

		// Reset the animation
		slides.at(slideNum).changeAnimation(Animation::AnimationEnum::IDLE);
	}

	void Tutorial::draw() const
	{
		slides.at(slideNum).draw();
	}

	void Tutorial::update()
	{
		slides.at(slideNum).update();
	}
}