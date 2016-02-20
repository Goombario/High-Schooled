#include "Image.h"
#include "Fizzle\Fizzle.h"

namespace Image
{
	ImageManager::ImageManager()
	{

	}

	ImageManager::~ImageManager()
	{
		for (auto it = imageMap.begin(); it != imageMap.end(); it++)
		{
			FzlDeleteSprite((*it).second);
		}
	}

	FzlSpriteHandle ImageManager::getHandle(std::string const& filepath) const
	{
		return imageMap.at(filepath);
	}

	void ImageManager::loadHandle(std::string const& filePath, int const& frameWidth, int const& frameHeight)
	{
		// If can't find the filepath in the map, add it.
		if (imageMap.find(filePath) == imageMap.end())
		{
			FzlSpriteHandle newHandle = FzlLoadSprite(filePath.c_str(), frameWidth, frameHeight);
			imageMap[filePath] = newHandle;
		}
	}

	void ImageManager::deleteHandle(std::string const& filepath)
	{
		FzlDeleteSprite(imageMap[filepath]);
		imageMap.erase(filepath);
	}

}