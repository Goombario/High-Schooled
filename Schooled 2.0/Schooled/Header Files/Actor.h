#ifndef ACTOR_H
#define ACTOR_H

#include <Windows.h>
#include <string>
#include "Schooled.h"
using std::string;

class Item;
typedef Item* ItemPtr;


class Actor
{
public:
	// Create empty Actor
	Actor();

	// Create actor containing Tile and Stats
	Actor(Tile&, Stats&);

	// Create actor containing Tile, Stats, and a held Item
	Actor(Tile&, Stats&, ItemPtr);

	// Create actor containing Tile, Stats, held item and custom messages
	Actor(Tile&, Stats&, ItemPtr, string, string);

	// Create actor from file
	Actor(std::string);

	// Attack another Actor
	void attack(Actor&);

	// Get member variables
	COORD getLocation();
	Tile getTile();
	Stats getStats();

	int getX();
	int getY();

	std::string getMAttack();
	std::string getMDefend();
	std::string getName();

	bool getIsFinished();
	bool hasActed();

	// Set member variables
	void setLocation(COORD);
	void setTile(Tile);
	void setStats(Stats);

	void setMAttack(string);
	void setMDefend(string);
	void setName(string);

	void setHeldItem(ItemPtr);
	void setIsFinished(bool);
	void setActed(bool);
	void setAggro(bool);
	void setActive(bool);

	// Manipulate held item
	bool holdsItem();
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
	
	bool isFinished=false;
	bool acted=false;
	bool aggro = false;
	WORD colour;
};

// Pointer to an Actor object
typedef Actor* ActorPtr;

#endif