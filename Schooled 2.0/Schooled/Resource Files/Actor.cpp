#include <fstream>
#include <Windows.h>
#include <string>
#include "../Header Files/Schooled.h"
#include "../Header Files/Actor.h"
#include "../Header Files/Item.h"
#include "../Header Files/Console_color.h"
#include "../Header Files/Room.h"
using std::string;

// Colours
namespace con = JadedHoboConsole;

Actor::Actor() {}

Actor::Actor(Tile& t, Stats& s, ItemPtr i, string atk, string def) : tile(t), stats(s), item(i) 
{
	m_attack = atk;
	m_defend = def;
}

Actor::Actor(Tile& t, Stats& s, ItemPtr i) : tile(t), stats(s), item(i) {}

Actor::Actor(Tile& t, Stats& s) : tile(t), stats(s) {}

Actor::Actor(std::string filename)
{
	// Load enemies from file
	std::ifstream stream("Enemies/" + filename);
	std::string line;

	std::getline(stream, line);

	stream.close();

}

void Actor::attack(Actor& defender){
	defender.stats.HP -= stats.STR;
}

int Actor::dropItem()
{
	if (item == 0)	// If item equals 0, no held item
	{
		return 0;
	}
	int itemInt = item->getTile().tileInt;	// Otherwise, return Item from Actor
	item = nullptr;		// set Held Item to null pointer
	return itemInt;
}

bool Actor::holdsItem()
{
	if (item == 0)	// Return false if no held item
	{
		return false;
	}
	return true;	// Return true if holds item
	
}

void Actor::pickUp(ItemPtr pickUp)
{
	// Have pickup stats alter player stats
	stats.HP += pickUp->getStats().HP;	
	stats.EN += pickUp->getStats().EN;
	stats.STR += pickUp->getStats().STR;
}

void Actor::setAggro(bool b)
{
	if (b == true)	// Setting aggro to true
	{
		tile.colorCode = con::fgHiRed;
		aggro = true;
	}
	else	// Setting aggro to false
	{
		tile.colorCode = Room::actorIndex[tile.tileInt].tile.colorCode;
		aggro = false;
	}
}

void Actor::setActive(bool isActive)
{
	if (isActive)	// If actor is moving
	{
		tile.colorCode = con::fgHiMagenta;
	}
	else if (aggro)	// If aggro
	{
		tile.colorCode = con::fgHiRed;
	}
	else			// No aggro
	{
		tile.colorCode = Room::actorIndex[tile.tileInt].tile.colorCode;
	}
}

// Get member variables
COORD Actor::getLocation() { return location; }
int Actor::getX() { return location.X; }
int Actor::getY() { return location.Y; }
Stats Actor::getStats() { return stats; }
Tile Actor::getTile() { return tile; }
std::string Actor::getMAttack() { return m_attack; }
std::string Actor::getMDefend() { return m_defend; }
std::string Actor::getName() { return name; }
bool Actor::getIsFinished() { return isFinished; }
bool Actor::hasActed() { return acted; }

// Set member variables
void Actor::setTile(Tile t) { tile = t; }
void Actor::setStats(Stats s) { stats = s; }
void Actor::setLocation(COORD c) { location = c; }
void Actor::setMAttack(string s) { m_attack = s; }
void Actor::setMDefend(string s) { m_defend = s; }
void Actor::setName(string s) { name = s; }
void Actor::setHeldItem(ItemPtr i) { item = i; }
void Actor::setIsFinished(bool b) { isFinished = b; }
void Actor::setActed(bool b) { acted = b; }