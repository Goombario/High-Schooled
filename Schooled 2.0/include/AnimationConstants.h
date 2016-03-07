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
	};
}


#endif