#ifndef ANIMATION_H
#define ANIMATION_H

#include <vector>
#include <map>
#include <string>
#include <iostream>
#include "AnimationConstants.h"

namespace Animation
{
	struct Frame
	{
		double duration;
	};

	struct Animation
	{
		std::string name;
		bool loop;
		int firstFrame;
		std::vector<Frame> frames;
	};

	class AnimationData
	{
	public:
		// Loads the data from the given file
		AnimationData();
		AnimationData(std::string const& dataPath, int const& numCol);
		~AnimationData();

		// Get the animation from the identifier
		inline Animation const& getAnimation(AnimationEnum const&) const;
		//inline Animation const& getAnimation(int const&) const;
		
		// Get the numbers of columns
		inline int const getNumCol() const { return numCol; }
	private:
		std::map<AnimationEnum, Animation> animationMap;
		int numCol;
	};

	Animation const& AnimationData::getAnimation(AnimationEnum const& a) const
	{
		if (animationMap.find(a) != animationMap.end())
		{
			return animationMap.at(a);
		}
		std::cerr << "Error: Animation not found." << std::endl;
		exit(-2);
	}
}



#endif