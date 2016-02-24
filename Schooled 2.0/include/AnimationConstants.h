#ifndef ANIMATION_CONSTANTS_H
#define ANIMATION_CONSTANTS_H

#include <map>
#include <string>

namespace Animation
{
	enum AnimationEnum
	{
		ATTACK_1,
		ATTACK_2,
		ATTACK_3,
		WALK_LEFT,
		IDLE
	};

	static const std::map<std::string, AnimationEnum> AnimationLookup
	{
		{ "ATTACK_1", ATTACK_1 },
		{ "ATTACK_2", ATTACK_2 },
		{ "ATTACK_3", ATTACK_3 },
		{ "WALK_LEFT", WALK_LEFT },
		{ "IDLE", IDLE }
	};
}


#endif