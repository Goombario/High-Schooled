#ifndef ANIMATION_CONSTANTS_H
#define ANIMATION_CONSTANTS_H

#include <map>
#include <string>

namespace Animation
{
	enum class AnimationEnum
	{
		ATTACK_1,
		ATTACK_2,
		ATTACK_3,
		ATTACK_SPECIAL,
		IDLE,
		IDLE_BORED,
		TOKEN_EMPTY,
		TOKEN_PLACE,
		TOKEN_GHOST,
		COOLDOWN_1,
		COOLDOWN_2,
		BACKWARDS,
		FORWARDS,
		UP,
		DOWN,
		HURT,
		BLOCKED,
		PULSE,
		RED_PULSE,
		BLUE_PULSE,
		HP_GREEN_PULSE,
		HP_RED_PULSE,
		SP_BLUE_PULSE,
		SP_RED_PULSE,
		AP_VIOLET_PULSE
	};

	static const std::map<std::string, AnimationEnum> AnimationLookup
	{
		{ "ATTACK_1", AnimationEnum::ATTACK_1 },
		{ "ATTACK_2", AnimationEnum::ATTACK_2 },
		{ "ATTACK_3", AnimationEnum::ATTACK_3 },
		{ "ATTACK_SPECIAL", AnimationEnum::ATTACK_SPECIAL },
		{ "IDLE", AnimationEnum::IDLE },
		{ "IDLE_BORED", AnimationEnum::IDLE_BORED },
		{ "TOKEN_EMPTY", AnimationEnum::TOKEN_EMPTY },
		{ "TOKEN_PLACE", AnimationEnum::TOKEN_PLACE },
		{ "TOKEN_GHOST", AnimationEnum::TOKEN_GHOST },
		{ "COOLDOWN_1", AnimationEnum::COOLDOWN_1 },
		{ "COOLDOWN_2", AnimationEnum::COOLDOWN_2 },
		{ "BACKWARDS", AnimationEnum::BACKWARDS },
		{ "FORWARDS", AnimationEnum::FORWARDS },
		{ "UP", AnimationEnum::UP },
		{ "DOWN", AnimationEnum::DOWN },
		{ "HURT", AnimationEnum::HURT },
		{ "BLOCKED", AnimationEnum::BLOCKED },
		{ "PULSE", AnimationEnum::PULSE },
		{ "RED_PULSE", AnimationEnum::RED_PULSE },
		{ "BLUE_PULSE", AnimationEnum::BLUE_PULSE },
		{ "HP_GREEN_PULSE", AnimationEnum::HP_GREEN_PULSE },
		{ "HP_RED_PULSE", AnimationEnum::HP_RED_PULSE },
		{ "SP_BLUE_PULSE", AnimationEnum::SP_BLUE_PULSE },
		{ "SP_RED_PULSE", AnimationEnum::SP_RED_PULSE },
		{ "AP_VIOLET_PULSE", AnimationEnum::AP_VIOLET_PULSE }
	};
}


#endif