#include "Sprite.h"
#include "Fizzle\Fizzle.h"

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

		width = 0;
		height = 0;
		handle = -1;
	}

	Sprite::Sprite(FzlSpriteHandle sHandle, int w, int h) :
		handle(sHandle),
		width(w),
		height(h)
	{
		posX = 0.0f;
		posY = 0.0f;
		angle = 0.0f;
		scaleX = 0.0f;
		scaleY = 0.0f;
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
			posX = posX + (width / 2);
			posY = posY + (height / 2);
		}
	}

	void Sprite::draw()
	{
		if (scaleX != 0 || scaleY != 0)
		{
			FzlDrawSpriteScaled(handle, posX, posY, angle, scaleX, scaleY);
		}
		else
		{
			FzlDrawSprite(handle, posX, posY, angle);
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

	AnimatedSprite::AnimatedSprite()
	{
		animation = 0;
		frame = 0;
		numCol = 0;
		numRow = 0;
	}
	AnimatedSprite::AnimatedSprite(int spriteHandle, int width, int height, int col, int row)
		: Sprite(spriteHandle, width, height), numCol(col), numRow(row)
	{
		animation = 0;
		frame = 0;
	}

	void AnimatedSprite::draw()
	{
		if (scaleX != 0 || scaleY != 0)
		{
			FzlDrawAnimatedSpriteScaled(handle, animation, frame, posX, posY, angle, scaleX, scaleY);
		}
		else
		{
			FzlDrawAnimatedSprite(handle, animation, frame, posX, posY, angle);
		}
	}

	void AnimatedSprite::update(int state)
	{
		// If the frame has exceeded the limit, return to zero.
		if (frame < numCol)
		{
			frame++;
		}
		else
		{
			frame = 0;
		}

		// If the state has changed
		if (state != animation)
		{
			changeAnimation(state, frame);
		}
	}

	void AnimatedSprite::changeAnimation(int a, int f)
	{
		animation = a;
		frame = f;
	}
}