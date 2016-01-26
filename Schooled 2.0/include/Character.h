#ifndef CHARACTER_H
#define CHARACTER_H

#include "Sprite.h"

namespace Character
{
	// Class to hold a character that can hold a sprite and have multiple states.
	class Character
	{
	public:
		Character();
		Character(Sprite::AnimatedSprite&);

		// Draws the character's sprite to the screen
		void draw();

		// Updates the character's animation
		void update();

		// Changes the state to the given new state
		void changeState(int newState);

		// Returns the current state
		int getState() const;

		/* Move the sprite to position x, y on the screen, with the option to be centred there
		* @param float x - A float representing the pixel along the X axis to move to
		* @param float y - A float representing the pixel along the Y axis to move to
		* @param bool centered - A boolean value representing the desire to be centered within oneself.
		*		 defaults to true.
		*/
		void moveSprite(float x, float y, bool centered = true);

		/* Shift the sprite by x pixels right and y pixels down
		* @param float x - A float representing the number of pixels to be moved along the x axis
		* @param float y - A float representing the number of pixels to be moved along the y axis
		*/
		void shiftSprite(float x, float y);

		// Set the sprite angle to the given float angle
		void setSpriteAngle(float angle);

		// Set the sprite angle to the current angle plus the new angle
		void rotateSprite(float angle);

	private:
		int state;	// 0 standing, 1 walking, 2 attacking...
		Sprite::AnimatedSprite sprite;
	};
}

#endif