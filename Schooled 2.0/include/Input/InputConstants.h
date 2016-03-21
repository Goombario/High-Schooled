#ifndef INPUT_CONSTANTS_H
#define INPUT_CONSTANTS_H

#include <map>
#include <string>

namespace InputMapping
{
	enum class Action
	{
		BOARD_FORWARD,
		BOARD_BACKWARD,
		BOARD_UP,
		BOARD_DOWN,
		SELECT_POS,
		ATTACK_1,
		ATTACK_2,
		ATTACK_3,
		EXIT_GAME
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
		{ "BOARD_FORWARD", Action::BOARD_FORWARD },
		{ "BOARD_BACKWARD", Action::BOARD_BACKWARD },
		{ "BOARD_UP", Action::BOARD_UP },
		{ "BOARD_DOWN", Action::BOARD_DOWN },
		{ "SELECT_POS", Action::SELECT_POS },
		{ "ATTACK_1", Action::ATTACK_1 },
		{ "ATTACK_2", Action::ATTACK_2 },
		{ "ATTACK_3", Action::ATTACK_3 },
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