#ifndef STAGE_H
#define STAGE_H

#include "BattleConstants.h"
#include "GameObject.h"
#include "Vector2.h"
#include <vector>

namespace Sprite
{
	class Sprite;
	class AnimatedSprite;
}

namespace Player
{
	class Icon;
	class Player;
}

namespace tinyxml2
{
	class XMLElement;
}

namespace Stage
{
	class HUD : public GameObject::GameObject
	{
	public:
		HUD();
		HUD(Player::Player const&, tinyxml2::XMLElement*);
		~HUD();

		void draw() const;

		void update();

	private:
		const Player::Player *player;
		Sprite::Sprite *display;
		Sprite::Sprite *HPBar;
		Sprite::Sprite *SPBar;
		Vector::Vector2 offset;
		int side;
	};

	// Holds the stage elements ex. music, background elements, HUD
	class Stage
	{
	public:
		// FUTURE: Load data from file for images
		Stage(const char* stageName, 
			Player::Player const*, Player::Player const*);
		~Stage();

		// Draw functions
		void drawBackground();
		void drawHUD();

		// Update the HUD 
		void update();

		// Swap the active board
		void setActiveBoard(Side);

	private:
		Sprite::Sprite *background;
		Sprite::Sprite *boardSprite;
		Sprite::AnimatedSprite *p1BoardHighlight, *p2BoardHighlight;
		const Player::Player *player1, *player2;
		HUD p1HUD, p2HUD;
	};
}

#endif