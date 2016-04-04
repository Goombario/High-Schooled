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
		MENU_SELECT,
		MENU_BACK,
		MENU_UP,
		MENU_DOWN,
		MENU_P1_LEFT,
		MENU_P2_LEFT,
		MENU_P1_RIGHT,
		MENU_P2_RIGHT,
		P1_ADVANCE,
		P2_ADVANCE,
		P1_BACK,
		P2_BACK,
		P1_READY,
		P2_READY,
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
		{"MENU_SELECT", Action::MENU_SELECT },
		{"MENU_BACK", Action::MENU_BACK },
		{"MENU_UP", Action::MENU_UP },
		{"MENU_DOWN", Action::MENU_DOWN },
		{ "MENU_P1_LEFT", Action::MENU_P1_LEFT },
		{ "MENU_P2_LEFT", Action::MENU_P2_LEFT },
		{ "MENU_P1_RIGHT", Action::MENU_P1_RIGHT },
		{ "MENU_P2_RIGHT", Action::MENU_P2_RIGHT },
		{ "P1_ADVANCE", Action::P1_ADVANCE },
		{ "P2_ADVANCE", Action::P2_ADVANCE },
		{ "P1_BACK", Action::P1_BACK },
		{ "P2_BACK", Action::P2_BACK },
		{ "P1_READY", Action::P1_READY },
		{ "P2_READY", Action::P2_READY },
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