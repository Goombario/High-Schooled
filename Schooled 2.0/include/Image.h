#ifndef IMAGE_H
#define IMAGE_H

#include "Fizzle\DataTypes\FizzleDataTypes.h"
#include "tmxparser.h"
#include <string>
#include <vector>

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

		tmxparser::TmxImage getImage(std::string name);
		int const push(tmxparser::TmxImage const&);
		int const pop(std::string name);
		int const load(std::string name);
		int const unload(std::string name);

	private:
		std::vector<tmxparser::TmxImage> imageList;
	};
}



#endif