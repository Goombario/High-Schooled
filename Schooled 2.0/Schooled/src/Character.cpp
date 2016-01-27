#include "Character.h"
#include "Sprite.h"

namespace Character
{
	Character::Character() 
	{
		state = 0;
	}

	Character::Character(const Sprite::AnimatedSprite &s) : sprite(s)
	{
		state = 0;
	}

	void Character::draw()
	{
		sprite.draw();
	}

	void Character::update()
	{
		sprite.update(state);
	}

	void Character::changeState(int newState)
	{
		state = newState;
	}

	int Character::getState() const { return state; }

	void Character::moveSprite(float x, float y, bool centered)
	{
		sprite.move(x, y, centered);
	}

	void Character::shiftSprite(float x, float y)
	{
		sprite.shift(x, y);
	}

	void Character::setSpriteAngle(float a)
	{
		sprite.setAngle(a);
	}

	void Character::rotateSprite(float a)
	{
		sprite.rotate(a);
	}

}