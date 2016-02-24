#include "Level.h"

#include "Image.h"
#include "Character.h"
#include "Schooled.h"
#include "tinyxml2.h"
#include "Fizzle\Fizzle.h"
#include "GameEngine.h"
#include <iostream>

using namespace tinyxml2;

// Camera
namespace Level
{
	Camera::Camera()
	{

	}

	Camera::Camera(Vector::Vector3 const& newPos)
	{
		setCurrentPos(newPos);
	}

	Camera::~Camera()
	{

	}

	void Camera::update()
	{
		// Move the current position towards the destination

	}
	void Camera::setDestination(Vector::Vector3 const& newDestination)
	{
		destination = newDestination;
		transformation = newDestination - currentPos;
	}
}

// Level
namespace Level
{
	Level::Level()
	{

	}

	Level::Level(std::string const& mapFile, std::string const& dataFile)
	{
		Image::ImageManager *iManager = GameEngine::getImageManager();

		// Load the Tiled level
		std::string resPath = schooled::getResourcePath("textures");
		if (tmxparser::parseFromFile(mapFile, &map, resPath) != tmxparser::kSuccess)
		{
			std::cerr << "Error: tmxparser::parseFromFile in Level::Level()" << std::endl;
			exit(-1);
		}
		
		// Initialize all images
		// Initialize Tilesets
		for (auto it = map.tilesetCollection.begin(); it != map.tilesetCollection.end(); ++it)
		{
			iManager->loadImage((resPath + (*it).image.source), (*it).tileWidth, (*it).tileHeight);
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
		imageData = pRoot->FirstChildElement("imageData");

		for (auto it = map.imageLayerCollection.begin(); it != map.imageLayerCollection.end(); ++it)
		{
			if (imageData == nullptr)
			{
				std::cerr << "ERROR: Loading imageData: " << XML_ERROR_FILE_READ_ERROR << std::endl;
				exit(-1);
			}
			const char * name = nullptr;

			// Check if the names match
			name = imageData->Attribute("name");
			if (name == nullptr || name != (*it).name)
			{
				std::cerr << "ERROR: XML_ERROR_PARSING_ATTRIBUTE" << std::endl;
				std::cerr << "Result: " << name << std::endl;
				exit(-1);
			}

			if ((*it).image.source == resPath + "/")
			{
				std::cerr << "Error: image has no source" << std::endl;
				exit(-1);
			}

			CheckXMLResult(imageData->QueryUnsignedAttribute("width", &(*it).image.width));
			CheckXMLResult(imageData->QueryUnsignedAttribute("height", &(*it).image.height));
			iManager->loadImage(resPath + (*it).image.source, (*it).image.width, (*it).image.height);

			LayerInfo temp;
			temp.index = it - map.imageLayerCollection.begin();
			temp.type = "imageLayer";
			if ((*it).propertyMap.find("z") != (*it).propertyMap.end())
			{
				temp.order = stoi((*it).propertyMap["z"]);
			}
			else temp.order = 0;

			if ((*it).propertyMap.find("parallax") != (*it).propertyMap.end())
			{
				temp.parallax = stod((*it).propertyMap["parallax"]);
			}
			else temp.parallax = 1.0;
			
			layers[temp.order] = temp;

			name = nullptr;

			imageData = imageData->NextSiblingElement("imageData");
		}

		iManager = nullptr;
	}

	Level::Level(Level const& copy)
	{
		this->map = copy.map;
		this->layers = copy.layers;
		this->encounterList = copy.encounterList;
	}

	Level::~Level()
	{

	}

	void Level::draw()
	{
		std::string resPath = schooled::getResourcePath("textures");
		Image::ImageManager *iManager = GameEngine::getImageManager();
		for (auto it = layers.begin(); it != layers.end(); it++)
		{
			if ((*it).second.type == "imageLayer")
			{
				tmxparser::TmxImageLayer * temp = &map.imageLayerCollection[(*it).second.index];
				FzlDrawSprite(
					iManager->getImage(resPath + temp->image.source).handle, 
					static_cast<float>(temp->x + temp->offsetx * (*it).second.parallax), 
					static_cast<float>(temp->y + temp->offsety), 0.0f);
			}
		}

	}


}