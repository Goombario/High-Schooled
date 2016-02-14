#ifndef IMAGE_H
#define IMAGE_H

#include "Fizzle\DataTypes\FizzleDataTypes.h"
#include <string>
#include <vector>

namespace Image
{
	struct Image
	{
		std::string name;
		std::string source;
		int width;
		int height;
		int tileWidth;
		int tileHeight;
		FzlSpriteHandle handle;
	};

	class ImageManager
	{
	public:
		ImageManager();
		~ImageManager();

		Image getImage(std::string name);
		int const push(Image const&);
		int const pop(std::string name);
		int const load(std::string name);
		int const unload(std::string name);

	private:
		std::vector<Image> imageList;
	};
}



#endif