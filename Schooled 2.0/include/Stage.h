#ifndef STAGE_H
#define STAGE_H

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
		const Player::Player *player;
		Sprite::Sprite *display;
		Sprite::Sprite *HPBar;
		Sprite::Sprite *SPBar;
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

		// Get the token sprite
		Sprite::Sprite& getSprite() { return *token; }

	private:
		Sprite::Sprite *background;
		Sprite::Sprite *token;
		const Player::Player *player1, *player2;
		HUD p1HUD, p2HUD;
	};
}

#endif