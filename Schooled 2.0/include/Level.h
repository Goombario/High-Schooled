#ifndef LEVEL_H
#define LEVEL_H

#include "tmxparser.h"
#include "Character.h"
#include "Vector2.h"
#include <vector>
#include <map>
#include <string>

// Forward declaration
namespace Image
{
	class ImageManager;
}

namespace Level
{
	// Helper class that holds the sprites for a layer (UNUSED)
	struct SpriteLayer
	{
		std::vector<Sprite::AnimatedSprite> spriteList;
		Vector::Vector2 pos;
		int depth;
	};

	// Helper struct that holds layer drawing details
	struct LayerInfo
	{
		int zValue;
		double parallax;
		std::string type;
		int index; // Index in the type collection
	};

	class Level
	{
	public:
		Level();

		/* Constructor that loads the data for a level and its images
		* @param string const& mapFile - the name of the TMX file
		* @param string const& dataFile - the name of the XML file that holds image data
		*/
		Level(std::string const& mapFile, std::string const& dataFile);

		// Returns the map structure
		const tmxparser::TmxMap* getMap() const { return &map; }

		// Draws all layers of the map
		void draw();

		//bool isColliding(Character::Character const&);

	private:
		tmxparser::TmxMap map;
		std::map<int, LayerInfo> layers;	// Holds all layer info in order of drawing
	};
}

// Camera
namespace Level
{
	// Class that manipulates the player's viewpoint (camera)
	class Camera
	{
	public:
		Camera();
		Camera(Vector::Vector2 const&);
		~Camera();

		// Set the camera's current position
		inline void setCurrentPos(Vector::Vector2 const& newPos) { currentPos = newPos; }

		// Set the camera's destination for it to move to
		void setDestination(Vector::Vector2 const& newDestination);

		// Move the camera towards the destination (with acceleration?)
		void update();

		// Get the current camera position
		inline Vector::Vector2 const& getCurrentPos() const { return currentPos; }

	private:
		Vector::Vector2 currentPos;
		Vector::Vector2 destination;
		Vector::Vector2 transformation;
		float acceleration;
	};
}


#endif