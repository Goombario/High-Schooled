#ifndef STAGE_H
#define STAGE_H

namespace Sprite
{
	class Sprite;
}

namespace Player
{
	class Player;
}

namespace Stage
{
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
		Sprite::Sprite *HUD;
		const Player::Player *player1, *player2;
	};
}

#endif