#ifndef SPRITE_H
#define SPRITE_H

#include <string>

namespace Sprite
{
	typedef int FzlSpriteHandle;

	class Sprite
	{
	public:
		Sprite();
		Sprite(FzlSpriteHandle spriteHandle, int width, int height);

		/* Shift the sprite by x pixels right and y pixels down
		* @param float x - A float representing the number of pixels to be moved along the x axis
		* @param float y - A float representing the number of pixels to be moved along the y axis
		*/
		void shift(float x, float y);

		/* Move the sprite to position x, y on the screen, with the option to be centred there
		* @param float x - A float representing the pixel along the X axis to move to
		* @param float y - A float representing the pixel along the Y axis to move to
		* @param bool centered - A boolean value representing the desire to be centered within oneself.
		*		 defaults to true.
		*/
		void move(float x, float y, bool centered = true);	

		// Set the current angle.
		void setAngle(float angle);

		// Increase the rotation of the angle by the given float.
		void rotate(float angle);

		// Set the current scale
		inline void setScaleX(float newX) { scaleX = newX; }
		inline void setScaleY(float newY) { scaleY = newY; }

		// Draw the sprite to the screen.
		void draw();	

		inline void setSpriteHandle(int newHandle) { handle = newHandle; }

		// GETTERS
		float getX() const { return posX; }
		float getY() const { return posY; }
		float getAngle() const { return angle; }

	protected:
		float posX, posY, angle;
		int width, height;
		float scaleX, scaleY;
		FzlSpriteHandle handle;
	};

	class AnimatedSprite : public Sprite
	{
	public:
		AnimatedSprite();
		AnimatedSprite(int spriteHandle, int width, int height, int numCol, int numRow);

		// Draw the sprite to the screen.
		void draw();

		/* 
		* Advance the frame. If the frame reaches the end, repeat. 
		*If the state has changed, change the animation to given state.
		*/
		void update(int state);	

		// Change the animation to the given animation and frame.
		void changeAnimation(int a, int f = 0);	

		// GETTERS
		int getFrame() { return frame; }
		int getAnimation() { return animation; }
		//int getMaxFrame() { return maxFrame; }

	private:
		int animation, frame;
		int numCol, numRow;
	};
}

#endif