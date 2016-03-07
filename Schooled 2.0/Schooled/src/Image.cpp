#include "Image.h"
#include "Fizzle\Fizzle.h"

namespace Image
{
	ImageManager::ImageManager()
	{

	}

	ImageManager::~ImageManager()
	{
		// Delete all sprites within the manager
		for (auto it = imageMap.begin(); it != imageMap.end(); it++)
		{
			FzlDeleteSprite((*it).second.handle);
		}
	}

	Image const& ImageManager::loadImage(std::string const& filePath, int const& frameWidth, int const& frameHeight)
	{
		// If can't find the filepath in the map, add it.
		if (imageMap.find(filePath) == imageMap.end())
		{
			FzlSpriteHandle newHandle = FzlLoadSprite(filePath.c_str(), frameWidth, frameHeight);
			Image newImage = { newHandle, frameWidth, frameHeight };
			imageMap[filePath] = newImage;
		}
		return getImage(filePath);
	}

	void ImageManager::deleteImage(std::string const& filepath)
	{
		FzlDeleteSprite(imageMap[filepath].handle);
		imageMap.erase(filepath);
	}

}