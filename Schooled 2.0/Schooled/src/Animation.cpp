#include "Animation.h"
#include "tinyxml2.h"

using namespace tinyxml2;

namespace Animation
{
	AnimationData::AnimationData()
	{
		numCol = 0;
	}
	AnimationData::AnimationData(std::string const& dataPath)
	{
		// Open the XML Document
		XMLDocument data;
		CheckXMLResult(data.LoadFile(dataPath.c_str()));

		// Choose the root
		XMLNode *root = data.RootElement();
		if (root == nullptr)
		{
			std::cerr << "ERROR: Loading animation data file: " << dataPath << std::endl;
			exit(-1);
		}
		CheckXMLResult(root->ToElement()->QueryIntAttribute("numCol", &numCol));

		// Retrieve the animations
		XMLElement *animElement;
		animElement = root->FirstChildElement();
		while (animElement != nullptr)
		{
			// Load all animation data into a temporary file
			Animation temp;
			temp.name = animElement->Attribute("name");
			CheckXMLResult(animElement->QueryBoolAttribute("loop", &temp.loop));
			CheckXMLResult(animElement->FirstChildElement("FirstFrame")->QueryIntText(&temp.firstFrame));

			// Get all frame data
			XMLElement *frameElement;
			frameElement = animElement->FirstChildElement("Frame");
			while (frameElement != nullptr)
			{
				Frame tempFrame;
				CheckXMLResult(frameElement->QueryDoubleText(&tempFrame.duration));
				temp.frames.push_back(tempFrame);
				frameElement = frameElement->NextSiblingElement();
			}

			// Put the animation into the map, and load the next animation
			if (AnimationLookup.find(temp.name) == AnimationLookup.end())
			{
				std::cerr << "Error: Animation " << temp.name << " not found in animation lookup" << std::endl;
				exit(-3);
			}
			AnimationEnum tempEnum = AnimationLookup.at(temp.name);
			animationMap[tempEnum] = temp;
			animElement = animElement->NextSiblingElement();
		}
	}

	AnimationData::~AnimationData()
	{
		animationMap.clear();
	}
}