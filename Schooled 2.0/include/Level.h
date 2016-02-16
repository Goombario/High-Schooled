#ifndef LEVEL_H
#define LEVEL_H

#include "tmxparser.h"
#include "Character.h"
#include "Vector2.h"
#include <vector>
#include <map>
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

	struct SpriteLayer
	{
		std::vector<Sprite::AnimatedSprite> spriteList;
		Vector::Vector2 pos;
		int depth;
	};

	struct LayerInfo
	{
		int order;
		double parallax;
		std::string type;
		int index; // Index in the type collection
	};

	class Level
	{
	public:
		Level();
		Level(Level const&);
		Level(std::string const& mapFile, std::string const& dataFile);
		~Level();

		const tmxparser::TmxMap* getMap() const { return &map; }

		void draw();
		//bool isColliding(Character::Character const&);

	private:
		tmxparser::TmxMap map;
		std::map<int, LayerInfo> info;
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