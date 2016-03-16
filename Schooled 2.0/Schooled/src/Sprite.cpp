#include "Sprite.h"
#include "Fizzle\Fizzle.h"
#include "Schooled.h"
#include "tinyxml2.h"
#include "GameEngine.h"

// Sprite
namespace Sprite
{
	Sprite::Sprite()
	{
		posX = 0.0f;
		posY = 0.0f;
		angle = 0.0f;
		scaleX = 1.0f;
		scaleY = 1.0f;
		image.handle = 0;
		image.frameHeight = 0;
		image.frameWidth = 0;
	}

	Sprite::Sprite(Image::Image const& i) :
		image(i)
	{
		posX = 0.0f;
		posY = 0.0f;
		angle = 0.0f;
		scaleX = 1.0f;
		scaleY = 1.0f;
	}

	Sprite::Sprite(tinyxml2::XMLElement const* element) : Sprite()
	{
		unsigned int frameWidth;
		unsigned int frameHeight;
		std::string spriteName = element->Attribute("name");
		tinyxml2::CheckXMLResult(element->QueryUnsignedAttribute("frameWidth", &frameWidth));
		tinyxml2::CheckXMLResult(element->QueryUnsignedAttribute("frameHeight", &frameHeight));
		Image::Image spriteImage = GameEngine::getImageManager()->loadImage(
			schooled::getResourcePath("img") + spriteName, frameWidth, frameHeight);
		
		this->setImage(spriteImage);
	}

	void Sprite::shift(float x, float y)
	{
		posX += x;
		posY += y;
	}

	void Sprite::move(float x, float y, bool centered)
	{
		posX = x;
		posY = y;

		if (!centered)
		{
			posX = posX + (image.frameWidth / 2);
			posY = posY + (image.frameHeight / 2);
		}
	}

	void Sprite::draw()
	{
		// If the scales have been tipped, draw the sprite scaled
		if (scaleX != 1.0f || scaleY != 1.0f || schooled::SCALE != 1.0f)
		{
			FzlDrawSpriteScaled(image.handle, 
				(posX * scaleX * schooled::SCALE),
				(posY * scaleY * schooled::SCALE), 
				angle, 
				scaleX * schooled::SCALE, scaleY * schooled::SCALE);
		}
		else
		{
			FzlDrawSprite(image.handle, posX, posY, angle);
		}
	}

	void Sprite::drawAt(float x, float y)
	{
		// If the scales have been tipped, draw the sprite scaled
		if (scaleX != 1.0f || scaleY != 1.0f || schooled::SCALE != 1.0f)
		{
			FzlDrawSpriteScaled(image.handle,
				(x * scaleX * schooled::SCALE),
				(y * scaleY * schooled::SCALE),
				angle,
				scaleX * schooled::SCALE, scaleY * schooled::SCALE);
		}
		else
		{
			FzlDrawSprite(image.handle, x, y, angle);
		}
	}

	void Sprite::setAngle(float a)
	{
		angle = a;
	}

	void Sprite::rotate(float a)
	{
		angle += a;
	}

}

// Animated Sprite
namespace Sprite
{

	AnimatedSprite::AnimatedSprite(Image::Image const& i, Animation::AnimationData const& sheet)
		: Sprite(i), data(sheet)
	{
		col = 0;
		row = 0;
		numCol = data.getNumCol();
		time = 0.0;

		pushAnimation(Animation::AnimationEnum::IDLE);
	}

	AnimatedSprite::AnimatedSprite(tinyxml2::XMLElement const* imageData, tinyxml2::XMLElement const* animationData)
		: Sprite(imageData)
	{
		std::string animationName = animationData->Attribute("name");
		data = Animation::AnimationData(schooled::getResourcePath("img") + "Image_Data/" + animationName);
		col = 0;
		row = 0; 
		numCol = data.getNumCol();
		time = 0.0;
	}

	void AnimatedSprite::draw()
	{
		// If the scales have tipped, draw the sprite scaled.
		if (scaleX != 1.0f || scaleY != 1.0f || schooled::SCALE != 1.0f)
		{
			FzlDrawAnimatedSpriteScaled(image.handle, row, col, posX, posY, angle,
				scaleX + schooled::SCALE, scaleY + schooled::SCALE);
		}
		else
		{
			FzlDrawAnimatedSprite(image.handle, row, col, posX, posY, angle);
		}
	}

	void AnimatedSprite::drawAt(float x, float y)
	{
		// If the scales have tipped, draw the sprite scaled.
		if (scaleX != 1.0f || scaleY != 1.0f || schooled::SCALE != 1.0f)
		{
			FzlDrawAnimatedSpriteScaled(image.handle, row, col, x, y, angle,
				scaleX + schooled::SCALE, scaleY + schooled::SCALE);
		}
		else
		{
			FzlDrawAnimatedSprite(image.handle, row, col, x, y, angle);
		}
	}

	void AnimatedSprite::update()
	{
		//time += FzlGetDeltaTime();	//Broken currently
		time += (1.0 / schooled::FRAMERATE);	// Locked framerate

		

		// Checks if the time elapsed since the last frame drawn is large enough to advance
		// To the next frame
		if (animationList.back().frames[col].duration <= time)
		{
			//std::cout << "Time: " << time << std::endl;	// For testing purposes
			time -= animationList.back().frames[col].duration;
			col++;

			// If at the end of the animation
			if (col >= animationList.back().frames.size())
			{
				// If the animation loops
				if (animationList.back().loop)
				{
					col = 0;
				}
				else
				{
					animationList.pop_back();
					if (animationList.size() == 0)
					{
						pushAnimation(Animation::AnimationEnum::IDLE);
					}
				}
			}
		}
	}

	void AnimatedSprite::pushAnimation(Animation::AnimationEnum a)
	{
		Animation::Animation tempA = data.getAnimation(a);
		row = (tempA.firstFrame / numCol);	// Gets the row based on the first frame position
		col = (tempA.firstFrame % numCol);	// Gets the column based on the first frame postion	(Mostly for debug)
		animationList.push_back(tempA);
	}

	void AnimatedSprite::popAnimation()
	{
		animationList.pop_back();
	}

	void AnimatedSprite::changeAnimation(Animation::AnimationEnum a)
	{
		popAnimation();
		pushAnimation(a);
		time = 0.0;
	}

	Animation::AnimationEnum AnimatedSprite::getCurrentAnimation()
	{
		return Animation::AnimationLookup.at(animationList.back().name);
	}
}