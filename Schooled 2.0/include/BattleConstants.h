#ifndef BATTLE_CONSTANTS_H
#define BATTLE_CONSTANTS_H


enum class Direction
{
	UP,
	DOWN,
	FORWARD,
	BACKWARD
};

enum class Side
{
	LEFT,
	RIGHT
};

namespace Stage
{
	int const BOARD_WIDTH = 3;
	int const BOARD_HEIGHT = 3;
}


#endif