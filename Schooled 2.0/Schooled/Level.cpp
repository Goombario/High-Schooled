#include "Level.h"

#include "Image.h"
#include "Character.h"
#include "Schooled.h"
#include "tinyxml2.h"
#include "Fizzle\Fizzle.h"
#include <iostream>

using namespace tinyxml2;

namespace Level
{
	Level::Level()
	{

	}
	Level::Level(std::string const& mapFile, std::string const& dataFile)
	{
		if (tmxparser::parseFromFile(mapFile, &map, schooled::getResourcePath("textures")) != tmxparser::kSuccess)
		{
			std::cerr << "Error: tmxparser::parseFromFile in Level::Level()" << std::endl;
			exit(-1);
		}
		
		// Initialize all images
		// Initialize Tilesets
		for (auto it = map.tilesetCollection.begin(); it != map.tilesetCollection.end(); ++it)
		{
			(*it).image.handle = FzlLoadSprite(((*it).image.source).c_str(),
				(*it).tileWidth, 
				(*it).tileHeight);
		}

		// Initialize Image layers
		// Load XML data file
		tinyxml2::XMLDocument data;
		CheckXMLResult(data.LoadFile(dataFile.c_str()));
		XMLNode *pRoot = data.RootElement();
		if (pRoot == nullptr)
		{
			std::cerr << "ERROR: Loading Level data file: " << XML_ERROR_FILE_READ_ERROR << std::endl;
			exit(-1);
		}

		// Choose the first image data
		XMLElement *imageData;
		if (map.imageLayerCollection.begin() != map.imageLayerCollection.end())
		{
			imageData = pRoot->FirstChildElement();
			if (imageData == nullptr)
			{
				std::cerr << "ERROR: Loading imageData: " << XML_ERROR_FILE_READ_ERROR << std::endl;
				exit(-1);
			}
		}

		for (auto it = map.imageLayerCollection.begin(); it != map.imageLayerCollection.end(); ++it)
		{
			const char * name = nullptr;
			int width, height;

			// Check if the names match
			name = imageData->Attribute("name");
			if (name == nullptr || name != (*it).name)
			{
				std::cerr << "ERROR: XML_ERROR_PARSING_ATTRIBUTE" << std::endl;
				std::cerr << "Result: " << name << std::endl;
				exit(-1);
			}

			CheckXMLResult(imageData->QueryIntAttribute("width", &width));
			CheckXMLResult(imageData->QueryIntAttribute("height", &height));
			(*it).image.handle = FzlLoadSprite((*it).image.source.c_str(), width, height);

			//delete name;	// POSSIBLE MEMORY LEAK
			name = nullptr;

			imageData = pRoot->NextSiblingElement("imageData");
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