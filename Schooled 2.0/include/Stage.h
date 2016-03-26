#ifndef STAGE_H
#define STAGE_H

#include "BattleConstants.h"
#include <vector>

namespace Sprite
{
	class Sprite;
	class AnimatedSprite;
}

namespace Player
{
	class Player;
}

namespace Stage
{
	class HUD
	{
	public:
		HUD(Player::Player const&);
		~HUD();

		void draw();

		void update();

	private:

		// Attack Icon management
		void moveIcons();

		const Player::Player *player;
		Sprite::Sprite *display;
		Sprite::Sprite *HPBar;
		Sprite::Sprite *SPBar;
		std::vector<Sprite::AnimatedSprite> icons;
		
		float offsetX;	// The distance from the edge to the HP bar
		float offsetY;	// The bottom of the HP bar
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