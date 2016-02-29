#include "Player.h"
#include "Sprite.h"
#include "Board.h"

namespace Player
{
	Player::Player()
	{
		stats.currentAP = 0;
		stats.currentHP = 0;
		stats.currentSP = 0;
		stats.maxAP = 0;
		stats.maxHP = 0;
		stats.maxSP = 0;

		sprite = nullptr;
		boardPtr = nullptr;
	}

	Player::Player(Player const& source)
	{
		this->sprite = source.sprite;	// Unknown if good
		this->boardPtr = source.boardPtr;
	}

	Player::~Player()
	{
		delete sprite;
		delete boardPtr;
	}


}