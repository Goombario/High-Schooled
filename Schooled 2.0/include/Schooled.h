#ifndef SCHOOLED_H
#define SCHOOLED_H

#include <string>

namespace schooled
{
	// Constant variables
	int const FRAMERATE = 60;
	float const SCALE = 1.0f;
	double const GRAVITY = 49.05;
	int const SCREEN_HEIGHT_PX = 720;
	int const SCREEN_WIDTH_PX = 1280;

	// Return the setting that the key is attached to
	std::string getSetting(std::string);

	// Get the path to the resources
	std::string getResourcePath(std::string const& subDir = "");
}

#endif