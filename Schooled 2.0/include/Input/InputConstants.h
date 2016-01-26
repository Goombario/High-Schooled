#ifndef INPUT_CONSTANTS_H
#define INPUT_CONSTANTS_H

namespace InputMapping
{
	enum Action
	{
		ATTACK,
		EXIT_GAME,
	};

	enum State
	{
		MOVE_LEFT,
		MOVE_RIGHT,
		MOVE_UP,
		MOVE_DOWN,
	};

	enum Range
	{
		RANGE_ONE,
		RANGE_TWO,
	};
}

#endif