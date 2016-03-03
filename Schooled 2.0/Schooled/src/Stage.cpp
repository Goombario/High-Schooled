#include "Stage.h"
#include "Player.h"
#include "Sprite.h"
#include "GameEngine.h"
#include "Schooled.h"

namespace Stage
{
	Stage::Stage(const char* backgroundPath, int frameWidth, int frameHeight, Player::Player const& player1, Player::Player const& player2)
	{
		Image::Image tempImage = GameEngine::getImageManager()->loadImage(
			schooled::getResourcePath("img") + backgroundPath, 
			frameWidth, frameHeight);
		background = new Sprite::Sprite(tempImage);
		background->move(0, 0, false);
	}

	Stage::~Stage()
	{
		delete background;
		background = nullptr;
	}

	void Stage::drawBackground()
	{
		background->draw();
	}

	void Stage::drawHUD()
	{

	}

	void Stage::update()
	{

	}
}