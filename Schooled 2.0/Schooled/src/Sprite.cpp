#include "Sprite.h"
#include "Fizzle\Fizzle.h"
#include "Schooled.h"

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

	void AnimatedSprite::update()
	{
		//time += FzlGetDeltaTime();	//Broken currently
		time += (1.0 / schooled::FRAMERATE);	// Locked framerate

		

		// Checks if the time elapsed since the last frame drawn is large enough to advance
		// To the next frame
		if (animationList.back().frames[col].duration <= time)
		{
			std::cout << "Time: " << time << std::endl;	// For testing purposes
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
}