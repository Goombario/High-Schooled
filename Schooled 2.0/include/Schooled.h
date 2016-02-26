#ifndef SCHOOLED_H
#define SCHOOLED_H

#include <string>

namespace schooled
{
	// Constant variables
	int const MAP_WIDTH = 60;
	int const MAP_HEIGHT = 20;
	int const SCREEN_WIDTH = 80;
	int const SCREEN_HEIGHT = 25;
	int const FLOOR_WIDTH = 6;
	int const FLOOR_HEIGHT = 8;
	int const ITEM_INDEX_SIZE = 2;
	int const OFFSET = 1;
	double const CAMERA_SPEED = 3;
	float const CAMERA_MAX_ACCELERATION = 2;
	int const FRAMERATE = 60;

	int const SCREEN_HEIGHT_PX = 400;
	int const SCREEN_WIDTH_PX = 960;
	int const TILE_SIZE = 16;
	int const TILE_SIZE_CENTER = TILE_SIZE / 2;

	// Return the setting that the key is attached to
	std::string getSetting(std::string);

	// Get the path to the resources
	std::string getResourcePath(std::string const& subDir = "");
}

#endif