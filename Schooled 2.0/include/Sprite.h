#ifndef SPRITE_H
#define SPRITE_H

#include "Image.h"
#include "Animation.h"

namespace tinyxml2
{
	class XMLElement;
}

namespace Sprite
{
	typedef int FzlSpriteHandle;

	class Sprite
	{
	public:
		Sprite();
		Sprite(Image::Image const&);
		Sprite(tinyxml2::XMLElement const*);

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

		// Set the image
		inline void setImage(Image::Image const& newImage) { image = newImage; }

		// Increase the rotation of the angle by the given float.
		void rotate(float angle);

		// Set the current scale
		inline void setScaleX(float newX) { scaleX = newX; }
		inline void setScaleY(float newY) { scaleY = newY; }

		// Draw the sprite to the screen.
		void draw();	
		void drawAt(float x, float y);

		// GETTERS
		inline float getX() const { return posX; }
		inline float getY() const { return posY; }
		inline float getAngle() const { return angle; }
		inline int getFrameWidth() const { return image.frameWidth; }
		inline int getFrameHeight() const { return image.frameHeight; }

	protected:
		float posX, posY, angle;
		float scaleX, scaleY;
		Image::Image image;
	};

	class AnimatedSprite : public Sprite
	{
	public:
		AnimatedSprite(Image::Image const&, Animation::AnimationData const&);
		AnimatedSprite(tinyxml2::XMLElement const* imageData, tinyxml2::XMLElement const* animationData);

		// Draw the sprite to the screen.
		void draw();
		void drawAt(float x, float y);
		 
		// Updates the animation.
		void update();

		// Change the current animation to the given animation.
		void changeAnimation(Animation::AnimationEnum a);

		// Add an animation to the list
		void pushAnimation(Animation::AnimationEnum a);

		// Stop the current animation and remove it from the list
		void popAnimation();

		// Get the current animation
		Animation::AnimationEnum getCurrentAnimation();

	private:
		std::vector<Animation::Animation> animationList;
		Animation::AnimationData data;
		unsigned int row, col;	// One animation per row
		unsigned int numCol;		// The maximum number of columns
		double time;
	};
}

#endif