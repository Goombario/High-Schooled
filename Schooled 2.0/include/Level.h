#ifndef LEVEL_H
#define LEVEL_H

#include "tinyxml2.h"
#include "Image.h"

#include <vector>
#include <string>

namespace Level
{
	class Level
	{
	public:
		Level(std::string xmlFileName);
		~Level();

	private:
		tinyxml2::XMLDocument levelDoc;
	};
}


#endif