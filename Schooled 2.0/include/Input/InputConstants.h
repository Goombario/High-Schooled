#ifndef INPUT_CONSTANTS_H
#define INPUT_CONSTANTS_H

namespace InputMapping
{
	enum class Action
	{
		ATTACK,
		EXIT_GAME,
	};

	enum class State
	{
		MOVE_LEFT,
		MOVE_RIGHT,
		MOVE_UP,
		MOVE_DOWN,
	};

	enum class Range
	{
		RANGE_ONE,
		RANGE_TWO,
	};

	enum class RawInputAxis
	{
		RAW_INPUT_AXIS_MOUSE_X,
		RAW_INPUT_AXIS_MOUSE_Y,
	};
}

#endif