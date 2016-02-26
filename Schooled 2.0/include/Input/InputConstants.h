#ifndef INPUT_CONSTANTS_H
#define INPUT_CONSTANTS_H

#include <map>
#include <string>

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

	static const std::map<std::string, Range> RangeLookup
	{
		{ "RANGE_ONE", Range::RANGE_ONE },
		{ "RANGE_TWO", Range::RANGE_TWO },
	};

	static const std::map<std::string, Action> ActionLookup
	{
		{ "ATTACK", Action::ATTACK },
		{ "EXIT_GAME", Action::EXIT_GAME },
	};

	static const std::map<std::string, State> StateLookup
	{
		{ "MOVE_LEFT", State::MOVE_LEFT },
		{ "MOVE_RIGHT", State::MOVE_RIGHT },
		{ "MOVE_UP", State::MOVE_UP },
		{ "MOVE_DOWN", State::MOVE_DOWN },
	};

}

#endif