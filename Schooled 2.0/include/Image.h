#ifndef IMAGE_H
#define IMAGE_H

#include "Fizzle\DataTypes\FizzleDataTypes.h"
#include <string>
#include <iostream>
#include <map>

// Image management structures and classes
namespace Image
{
	// Structure that holds data for a single image
	struct Image
	{
		FzlSpriteHandle handle;
		int frameWidth;
		int frameHeight;
	};

	// Class that manages image loading and deleting
	class ImageManager
	{
	public:
		ImageManager();
		~ImageManager();

		/* Get the image based on the given filepath. If it doesn't exist, returns -1.
		* @param string filePath - the path to the image file
		*/
		inline Image const& getImage(std::string const& filePath) const;
		
		/* Loads the image into memory. If it exists already, nothing happens.
		* @param string const& filePath - the path to the image
		* @param int const& frameWidth - the width of the image
		* @param int const& frameHeight - the height of the image
		*/
		Image const& loadImage(std::string const& filePath, int const& frameWidth, int const& frameHeight);
		
		/* Deletes the image from memory, and removes it from the map.
		* @param string filePath - the path to the image file
		*/
		void deleteImage(std::string const& filePath);

	private:
		std::map<std::string, Image> imageMap;
	};

	/* Get the image based on the given filepath. If it doesn't exist, returns -1.
	* @param string filePath - the path to the image file
	*/
	Image const& ImageManager::getImage(std::string const& filePath) const 
	{ 
		if (imageMap.find(filePath) != imageMap.end())
		{
			return imageMap.at(filePath);
		}
		std::cerr << "Error: Image not found at " << filePath << std::endl;
		exit(-2);
	}
}

#endif