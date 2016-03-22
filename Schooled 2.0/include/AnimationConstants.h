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
		WALK_LEFT,
		IDLE,
		IDLE_BORED,
		TOKEN_PLACE,
		COOLDOWN_1,
		COOLDOWN_2,
		BACKWARDS,
		FORWARDS,
		UP,
		DOWN,
		HURT
	};

	static const std::map<std::string, AnimationEnum> AnimationLookup
	{
		{ "ATTACK_1", AnimationEnum::ATTACK_1 },
		{ "ATTACK_2", AnimationEnum::ATTACK_2 },
		{ "ATTACK_3", AnimationEnum::ATTACK_3 },
		{ "ATTACK_SPECIAL", AnimationEnum::ATTACK_SPECIAL },
		{ "WALK_LEFT", AnimationEnum::WALK_LEFT },
		{ "IDLE", AnimationEnum::IDLE },
		{ "IDLE_BORED", AnimationEnum::IDLE_BORED },
		{ "TOKEN_PLACE", AnimationEnum::TOKEN_PLACE },
		{ "COOLDOWN_1", AnimationEnum::COOLDOWN_1 },
		{ "COOLDOWN_2", AnimationEnum::COOLDOWN_2 },
		{ "BACKWARDS", AnimationEnum::BACKWARDS },
		{ "FORWARDS", AnimationEnum::FORWARDS },
		{ "UP", AnimationEnum::UP },
		{ "DOWN", AnimationEnum::DOWN },
		{ "HURT", AnimationEnum::HURT }
	};
}


#endif