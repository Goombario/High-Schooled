#include "Level.h"

#include "Image.h"
#include "Character.h"
#include "Fizzle\Fizzle.h"
#include <iostream>

namespace Level
{
	Level::Level(std::string mapFile)
	{
		if (tmxparser::parseFromFile(mapFile, &map, "res/textures") != tmxparser::kSuccess)
		{
			std::cerr << "Error: tmxparser::parseFromFile in Level::Level()" << std::endl;
		}
		

		// Initialize all images
		// Initialize Tilesets
		for (auto it = map.tilesetCollection.begin(); it != map.tilesetCollection.end(); ++it)
		{
			(*it).image.handle = FzlLoadSprite((*it).image.source.c_str(), 
				(*it).tileWidth, 
				(*it).tileHeight);
		}

		// Initialize Image layers
		for (auto it = map.imageLayerCollection.begin(); it != map.imageLayerCollection.end(); ++it)
		{
			(*it).image.handle = FzlLoadSprite((*it).image.source.c_str(), 
				(*it).widthInTiles*map.tileWidth, 
				(*it).heightInTiles*map.tileHeight);
		}
	}

	Level::~Level()
	{
		// Delete all used sprites
		for (auto it = map.tilesetCollection.begin(); it != map.tilesetCollection.end(); ++it)
		{
			FzlDeleteSprite((*it).image.handle);
		}
		for (auto it = map.imageLayerCollection.begin(); it != map.imageLayerCollection.end(); ++it)
		{
			FzlDeleteSprite((*it).image.handle);
		}
	}
}