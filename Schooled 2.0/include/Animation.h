#ifndef ANIMATION_H
#define ANIMATION_H

#include <vector>
#include <map>
#include <string>
#include <iostream>
#include "AnimationConstants.h"

// The structures and classes required for animation
namespace Animation
{
	// Frame structure holds the duration of the frame
	struct Frame
	{
		double duration;
	};

	// Animation structure holds the data for a single animation
	struct Animation
	{
		std::string name;
		bool loop;
		int firstFrame;
		std::vector<Frame> frames;
	};

	// AnimationData class holds multiple animation data read from a data file (XML)
	class AnimationData
	{
	public:
		// Loads the data from the given file
		AnimationData();
		AnimationData(std::string const& dataPath);
		~AnimationData();

		// Get the animation from the identifier
		inline Animation const& getAnimation(AnimationEnum const&) const;
		
		// Get the numbers of columns given in the sprite sheet
		inline int const getNumCol() const { return numCol; }
	private:
		std::map<AnimationEnum, Animation> animationMap;
		int numCol;
	};
}

// Inline member functions
namespace Animation
{
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