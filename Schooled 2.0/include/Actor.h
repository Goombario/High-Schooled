#ifndef ACTOR_H
#define ACTOR_H

#include "Schooled.h"
#include <string>
using std::string;

class Item;
typedef Item* ItemPtr;

class Actor
{
public:
	// Create empty Actor
	Actor();

	// Create actor containing Tile and Stats
	Actor(Tile, Stats);

	// Create actor containing Tile, Stats, and a held Item
	Actor(Tile, Stats, ItemPtr);

	// Attack another Actor
	void attack(Actor&);

	// Get member variables
	COORD getLocation() { return location; }
	int getX() { return location.X; }
	int getY() { return location.Y; }
	int getMinX(){ return minX; }
	int getMaxX(){ return maxX; }
	int getMinY(){ return minY; }
	int getMaxY(){ return maxY; }
	Stats getStats() { return stats; }
	Tile getTile() { return tile; }
	std::string getMAttack() { return m_attack; }
	std::string getMDefend() { return m_defend; }
	std::string getName() { return name; }
	bool getIsFinished() { return isFinished; }
	bool hasActed() { return acted; }

	// Set member variables
	void setTile(Tile t) { tile = t; }
	void setStats(Stats s) { stats = s; }
	void setLocation(COORD c) { location = c; }
	void setMinX(int x){ minX = x; }
	void setMaxX(int x){ maxX = x; }
	void setMinY(int y){ minY = y; }
	void setMaxY(int y){ maxY = y; }
	void setMAttack(string s) { m_attack = s; }
	void setMDefend(string s) { m_defend = s; }
	void setName(string s) { name = s; }
	void setHeldItem(ItemPtr i) { item = i; }
	void setIsFinished(bool b) { isFinished = b; }
	void setActed(bool b) { acted = b; }
	void setAggro(bool);
	void setActive(bool b);

	// Manipulate held item
	bool holdItem();
	int dropItem();
	int giveItem();
	void pickUp(ItemPtr);

private:
	// Member variables
	COORD location;
	Stats stats;
	Tile tile;
	ItemPtr item;
	std::string m_attack = "";	// To be read in
	std::string m_defend = "";
	std::string name = "";
	int minX, maxX, minY, maxY;
	bool isFinished=false;
	bool acted=false;
	bool aggro = false;
	WORD colour;
	//SDL_Texture texture;
};

// Pointer to an Actor object
typedef Actor* ActorPtr;

#endif