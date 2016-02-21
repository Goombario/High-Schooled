#ifndef INPUT_CONSTANTS_H
#define INPUT_CONSTANTS_H

#include <map>
#include <string>

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

	static const std::map<std::string, Range> RangeLookup
	{
		{ "RANGE_ONE", RANGE_ONE },
		{ "RANGE_TWO", RANGE_TWO },
	};

	static const std::map<std::string, Action> ActionLookup
	{
		{ "ATTACK", ATTACK },
		{ "EXIT_GAME", EXIT_GAME },
	};

	static const std::map<std::string, State> StateLookup
	{
		{ "MOVE_LEFT", MOVE_LEFT },
		{ "MOVE_RIGHT", MOVE_RIGHT },
		{ "MOVE_UP", MOVE_UP },
		{ "MOVE_DOWN", MOVE_DOWN },
	};

}

#endif