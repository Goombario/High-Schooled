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
		WALK_LEFT,
		IDLE
	};

	static const std::map<std::string, AnimationEnum> AnimationLookup
	{
		{ "ATTACK_1", AnimationEnum::ATTACK_1 },
		{ "ATTACK_2", AnimationEnum::ATTACK_2 },
		{ "ATTACK_3", AnimationEnum::ATTACK_3 },
		{ "WALK_LEFT", AnimationEnum::WALK_LEFT },
		{ "IDLE", AnimationEnum::IDLE }
	};
}


#endif