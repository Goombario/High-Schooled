#ifndef LEVEL_H
#define LEVEL_H

#include "tmxparser.h"
#include "Character.h"

#include <vector>
#include <string>


namespace Image
{
	class ImageManager;
}

namespace Level
{
	struct Encounter
	{
		Character::Character enemy;

	};

	class Level
	{
	public:
		Level(std::string mapFile);
		~Level();

		void draw();
		bool isColliding(Character::Character const&);

	private:
		tmxparser::TmxMap map;
		std::vector<Encounter> encounterList;
	};

	//class LevelManager
	//{
	//public:
	//	LevelManager();
	//	~LevelManager();

	//	int loadLevel(std::string mapFile);
	//private:
	//	std::vector<Level> levelList;
	//};
}


#endif