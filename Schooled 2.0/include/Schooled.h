#ifndef SCHOOLED_H
#define SCHOOLED_H

#include <Windows.h>
#include <string>
#include <vector>
using std::string;
using std::vector;

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

	int const SCREEN_HEIGHT_PX = 400;
	int const SCREEN_WIDTH_PX = 960;
	int const TILE_SIZE = 16;
	int const TILE_SIZE_CENTER = TILE_SIZE / 2;

	const vector<string> controlOptions = { "Classic", "Double-Tap", "Classic Lefty", "Double-Tap Lefty" };

	// Return the setting that the key is attached to
	string getSetting(string);

	// Get the path to the resources
	std::string getResourcePath(const std::string &subDir = "");
}

// Structures
struct Tile
{
	bool isPassable;
	int tileInt;
	char character;
	int colorCode;
};

struct Stats
{
	int HP; //hit points
	int EN;	//endurance
	int STR; //strength
};

// Operators for manipulating COORDS
bool operator ==(COORD a, COORD b);
bool operator !=(COORD a, COORD b);
COORD operator +(COORD a, COORD b);
COORD operator -(COORD a, COORD b);

#endif