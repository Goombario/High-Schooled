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
		scaleX = 0.0f;
		scaleY = 0.0f;
	}

	Sprite::Sprite(Image::Image const& i) :
		image(i)
	{
		posX = 0.0f;
		posY = 0.0f;
		angle = 0.0f;
		scaleX = 0.0f;
		scaleY = 0.0f;
	}

	Sprite::Sprite(Sprite const& source) :
		posX(source.posX),
		posY(source.posY),
		angle(source.angle),
		scaleX(source.scaleX),
		scaleY(source.scaleY),
		image(source.image)
	{

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
		if (scaleX != 0 || scaleY != 0)
		{
			FzlDrawSpriteScaled(image.handle, posX, posY, angle, scaleX, scaleY);
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

	AnimatedSprite::AnimatedSprite() : Sprite()
	{
		col = 0;
		row = 0;
		numCol = 0;
		time = 0.0;
	}

	AnimatedSprite::AnimatedSprite(Image::Image const& i, Animation::AnimationData const& sheet)
		: Sprite(i), data(sheet)
	{
		col = 0;
		row = 0;
		time = 0.0;
		numCol = data.getNumCol();
	}

	AnimatedSprite::AnimatedSprite(AnimatedSprite const& source) :
		Sprite(source),
		animationList(source.animationList),
		data(source.data),
		row(source.row),
		col(source.col),
		numCol(source.numCol),
		time(source.time)
	{

	}

	AnimatedSprite::~AnimatedSprite()
	{
		animationList.clear();
	}

	void AnimatedSprite::draw()
	{
		if (scaleX != 0 || scaleY != 0)
		{
			FzlDrawAnimatedSpriteScaled(image.handle, row, col, posX, posY, angle, scaleX, scaleY);
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

		std::cout << "Time: " << time << std::endl;
		while (animationList.back().frames[col].duration <= time)
		{
			time -= animationList.back().frames[col].duration;
			col++;

			// If at the end of the animation
			if (col >= animationList.back().frames.size())
			{
				if (animationList.back().loop)
				{
					col = 0;
				}
				else
				{
					animationList.pop_back();
					if (animationList.size() == 0)
					{
						pushAnimation(Animation::IDLE);
					}
				}
			}
		}
	}

	void AnimatedSprite::pushAnimation(Animation::AnimationEnum a)
	{
		Animation::Animation tempA = data.getAnimation(a);
		row = (tempA.firstFrame % numCol);
		col = (tempA.firstFrame / numCol);
		animationList.push_back(tempA);
	}

	void AnimatedSprite::changeAnimation(Animation::AnimationEnum a)
	{
		animationList.clear();
		pushAnimation(a);
	}
}