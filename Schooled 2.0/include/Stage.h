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
		Stage(const char* backgroundPath, int frameWidth, int frameHeight, 
			Player::Player const&, Player::Player const&);
		~Stage();

		// Draw functions
		void drawBackground();
		void drawHUD();

		// Update the HUD 
		void update();

	private:
		Sprite::Sprite *background;
	};
}

#endif