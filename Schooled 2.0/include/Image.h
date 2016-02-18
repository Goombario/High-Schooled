#ifndef IMAGE_H
#define IMAGE_H

#include "Fizzle\DataTypes\FizzleDataTypes.h"
#include "tmxparser.h"
#include <string>
#include <map>

namespace Level
{
	class Level;
}

namespace Image
{

	class ImageManager
	{
	public:
		ImageManager();
		~ImageManager();

		/* Get the handle based on the given filepath. If it doesn't exist, returns -1.
		* @param string filePath - the path to the image
		*/
		FzlSpriteHandle const& getHandle(std::string const& filePath) const;
		
		/* Loads the image into memory. If it exists already, nothing happens.
		* @param string filePath - the path to the image
		*/
		void loadHandle(std::string const& filePath, int const& frameWidth, int const& frameHeight);
		
		/* Deletes the image from memory, and removes it from the map.
		* @param string filePath - the path to the image
		*/
		void deleteHandle(std::string const& filePath);

	private:
		std::map<std::string, FzlSpriteHandle> imageMap;
	};
}



#endif