#include "Stage.h"
#include "Player.h"
#include "Sprite.h"
#include "GameEngine.h"
#include "Schooled.h"

namespace Stage
{
	Stage::Stage(const char* backgroundPath, int frameWidth, int frameHeight, 
		Player::Player const* p1, Player::Player const* p2) :
		player1(p1),
		player2(p2)
	{
		Image::Image tempImage = GameEngine::getImageManager()->loadImage(
			schooled::getResourcePath("img") + backgroundPath, 
			frameWidth, frameHeight);
		background = new Sprite::Sprite(tempImage);
		background->move(0, 0, false);

		token = nullptr;
		HUD = nullptr;
	}

	Stage::~Stage()
	{
		delete background;
		delete token;
		delete HUD;

		background = nullptr;
		token = nullptr;
		HUD = nullptr;
	}

	void Stage::drawBackground()
	{
		background->draw();
	}

	void Stage::drawHUD()
	{
		HUD->draw();
	}

	void Stage::update()
	{

	}
}