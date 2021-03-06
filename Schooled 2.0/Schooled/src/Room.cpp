#include "Room.h"
#include "Actor.h"
#include "Buffer.h"
#include "Item.h"
#include "Schooled.h"
#include "Console_color.h"
#include <fstream>
#include <iostream>
#include <cassert>

namespace con = JadedHoboConsole;

using std::endl;

vector<const Tile> Room::tileIndex;
vector<const Item> Room::itemIndex;
vector<const Actor> Room::actorIndex;

int Room::loadTileIndex(string filename)
{
	string resPath = schooled::getResourcePath("text");
	string line;
	std::ifstream stream(resPath + filename);
	if (stream.fail())
	{
		std::cerr << "ERROR: File open failed. (TileIndex)" << std::endl;
		return 1;
	}

	resPath = schooled::getResourcePath("img");
	while (!stream.eof())
	{
		// Get the information from the file
		getline(stream, line);
		string name = line.substr(line.find(':') + 1);
		getline(stream, line);
		int index = stoi(line.substr(line.find(':') + 1));
		getline(stream, line);
		char symbol = (line.substr(line.find('\'') + 1))[0];
		getline(stream, line);
		WORD colour = stoi(line.substr(line.find(':') + 1, '/'));
		getline(stream, line);
		bool passable = ((line.substr(line.find(':') + 1)) == "true");

		//SDL_Texture *tex = schooled::loadTexture(resPath + name + ".png", ren);

		// Put the tile in the vector
		Tile temp = { passable, index, symbol, colour };
		tileIndex.push_back(temp);
		// If there are more lines, get the empty line
		if (!stream.eof())
		{
			getline(stream, line);
		}
	}
	stream.close();
	return 0;
}

int Room::loadItemIndex(string filename)
{
	string resPath = schooled::getResourcePath("text");
	string line;
	std::ifstream stream(resPath + filename);
	if (stream.fail())
	{
		std::cerr << "File open failed. (ItemIndex)\n";
		return 1;
	}

	resPath = schooled::getResourcePath("img");

	while (!stream.eof())
	{
		// Get the information from the file
		getline(stream, line);
		string name = line.substr(line.find(':') + 1);
		getline(stream, line);
		int index = stoi(line.substr(line.find(':') + 1));
		getline(stream, line);
		char symbol = (line.substr(line.find('\'') + 1))[0];
		getline(stream, line);
		WORD colour = stoi(line.substr(line.find(':') + 1, '/'));
		getline(stream, line);
		bool passable = ((line.substr(line.find(':') + 1)) == "true");
		getline(stream, line);
		int HP = stoi(line.substr(line.find(':') + 1));
		getline(stream, line);
		int EN = stoi(line.substr(line.find(':') + 1));
		getline(stream, line);
		int STR = stoi(line.substr(line.find(':') + 1));
		getline(stream, line);
		string mPickup = (line.substr(line.find(':') + 1));

		//SDL_Texture *tex = schooled::loadTexture(resPath + name + ".png", ren);

		// Put the item in the vector
		Item temp;
	
		temp.setTile({ passable, index, symbol, colour });
		temp.setStats({ HP, EN, STR });
		temp.setMPickup(mPickup);
		temp.setName(name);
		itemIndex.push_back(temp);

		// If there are more lines, get the empty line
		if (!stream.eof())
		{
			getline(stream, line);
		}
	}
	stream.close();
	return 0;
}

int Room::loadActorIndex(string filename)
{
	string resPath = schooled::getResourcePath("text");
	string line;
	std::ifstream stream(resPath + filename);
	if (stream.fail())
	{
		std::cerr << "File open failed. (ActorIndex)\n";
		return 1;
	}

	resPath = schooled::getResourcePath("img");
	actorIndex.push_back(Actor());
	while (!stream.eof())
	{
		// Get the information from the file
		getline(stream, line);
		string name = line.substr(line.find(':') + 1);
		getline(stream, line);
		int index = stoi(line.substr(line.find(':') + 1));
		getline(stream, line);
		char symbol = (line.substr(line.find('\'') + 1))[0];
		getline(stream, line);
		WORD colour = stoi(line.substr(line.find(':') + 1, '/'));
		getline(stream, line);
		bool passable = ((line.substr(line.find(':') + 1)) == "true");
		getline(stream, line);
		int HP = stoi(line.substr(line.find(':') + 1));
		getline(stream, line);
		int EN = stoi(line.substr(line.find(':') + 1));
		getline(stream, line);
		int STR = stoi(line.substr(line.find(':') + 1));
		getline(stream, line);
		ItemPtr heldItem = getItemStats((line.substr(line.find(':') + 1)));
		getline(stream, line);
		string m_attack = (line.substr(line.find(':') + 1));
		getline(stream, line);
		string m_defend = (line.substr(line.find(':') + 1));

		//SDL_Texture *tex = schooled::loadTexture(resPath + "Person 1" + ".png", ren);

		// Put the actor in the vector
		Actor temp;
		temp.setTile({ passable, index, symbol, colour});
		temp.setStats({ HP, EN, STR });
		temp.setHeldItem(heldItem);
		temp.setName(name);
		temp.setMAttack(m_attack);
		temp.setMDefend(m_defend);
		temp.setLocation({ -1, -1 });
		actorIndex.push_back(temp);

		// If there are more lines, get the empty line
		if (!stream.eof())
		{
			getline(stream, line);
		}
	}
	stream.close();
	return 0;
}

void Room::display(Buffer& buffer){
	int tile;
	ActorPtr tempA;
	ItemPtr tempI;
	Tile *iTile;

	for (int a = 0; a < schooled::MAP_HEIGHT; a++){
		for (int b = 0; b < schooled::MAP_WIDTH; b++){

			if (tileArray[a][b] > 0)	// Draw tile otherwise
			{
				tile = tileArray[a][b];
				iTile = &tileIndex[tile];
				buffer.draw(iTile->character, iTile->colorCode, a + schooled::OFFSET, b);

				/*schooled::renderTexture(iTile->texture, ren,
					b*schooled::TILE_SIZE + schooled::TILE_SIZE_CENTER,
					a*schooled::TILE_SIZE + schooled::TILE_SIZE_CENTER);*/
			}
			if (itemArray[a][b] > 0)	// If item at position, draw
			{
				tile = itemArray[a][b];
				tempI = &itemIndex[tile];
				buffer.draw(tempI->getTile().character, tempI->getTile().colorCode, a + schooled::OFFSET, b);

				/*schooled::renderTexture(tempI->getTile().texture, ren,
					b*schooled::TILE_SIZE + schooled::TILE_SIZE_CENTER,
					a*schooled::TILE_SIZE + schooled::TILE_SIZE_CENTER);*/
			}
			if (actorArray[a][b] > 0)		// If actor at position, draw
			{
				tile = actorArray[a][b];
				tempA = &getActor({ b, a });
				buffer.draw(tempA->getTile().character, tempA->getTile().colorCode, a+ schooled::OFFSET, b);

				/*schooled::renderTexture(tempA->getTile().texture, ren,
					b*schooled::TILE_SIZE + schooled::TILE_SIZE_CENTER,
					a*schooled::TILE_SIZE + schooled::TILE_SIZE_CENTER);*/
			}
			
			
		}
	}
	
}

ItemPtr Room::getItemStats(int a)
{
	return &itemIndex[a];		// Do the thing
}

// Randomizes a number, outputs it to string
string Room::randomLog(bool a)
{
	int random;
	if (a == false)
		random = rand() % (10) + 1;
	else
		random = rand() % (25) + 1;
	string logNum = std::to_string(random);
	return logNum;
}

ItemPtr Room::getItemStats(string s)	// Return item stats based on Item name
{
	for (unsigned int i = 0; i < itemIndex.size(); i++)
	{
		if (itemIndex[i].getName() == s)
		{
			return &itemIndex[i];
		}
	}
	return nullptr;
}

int Room::findActor(COORD c)		// Does the other thing (finds actor in actor list)
{
	for (unsigned int i = 0; i < actorList.size(); i++)
	{
		if (actorList[i].getLocation() == c)
			return i;
	}
	return -1;
}

// Function definitions
Actor& Room::getActor(COORD c)
{
	int i = findActor(c);
	assert(i >= 0);
	return actorList[i];
}

Actor& Room::getActor(int i)
{
	return actorList[i];
}

vector<Actor>& Room::getActorList()
{
	return actorList;
}

void Room::removeActor(COORD c)
{
	int i = findActor(c);
	assert(i >= 0);
	actorList.erase(actorList.begin() + i);
}

//Enemy AIs
void Room::moveActors(COORD p, Actor& a)
{
	if (a.getTile().tileInt == 13) // If Cat, move Cat
	{
		moveCat(p, a);
	}
	else if (a.getTile().tileInt < 13) // If Enemy, move Enemy
	{
		moveEnemy1(p, a);
	}
	else if (a.getTile().tileInt == 20)	// If Minion, move Minion
	{
		moveMinion(a);
		if (a.getIsFinished())	// When Minion is finished moving
		{
			COORD tempLocation = a.getLocation();
			a = actorIndex[22];
			a.setLocation(tempLocation);
		}
	}
}

bool Room::isPassable(COORD tile){
	int mapX = tile.X;
	int mapY = tile.Y;
	if (mapX < 0 || mapX >= schooled::MAP_WIDTH || mapY < 0 || mapY >= schooled::MAP_HEIGHT)	// If out of bounds, return false
		return false;

	int tileValue = tileArray[mapY][mapX];
	int itemValue = itemArray[mapY][mapX];
	int actorValue = actorArray[mapY][mapX];
	ItemPtr tempI = &itemIndex[itemValue];
	if (tileIndex[tileValue].isPassable && actorValue == 0 && tempI->getTile().isPassable)		// If nothing is in the way, return true
		return true;
	return false;
}

Room::Room() {}

Room::Room(string fileName)
{
	// Open file for input
	std::ifstream stream;
	stream.open(schooled::getResourcePath("rooms") + (fileName));

	if (stream.fail())
	{
		std::cerr << "File open failed (Room)";
		exit(1);
	}

	string m;
	getline(stream, m);
	message = m;
	getline(stream, m); // blank

	// Get the tile array
	for (int a = 0; a < schooled::MAP_HEIGHT; a++){
		for (int b = 0; b < schooled::MAP_WIDTH; b++){
			stream >> tileArray[a][b];
		}
	}
	getline(stream, m); // blank

	// Get the entrance coordinates
	for (COORD &c : entrances)
	{
		stream >> c.X >> c.Y;
	}
	getline(stream, m); // blank

	// Get the item array
	for (int a = 0; a < schooled::MAP_HEIGHT; a++){
		for (int b = 0; b < schooled::MAP_WIDTH; b++){
			stream >> itemArray[a][b];
		}
	}
	getline(stream, m); // blank

	// Get the actor array
	for (int a = 0; a < schooled::MAP_HEIGHT; a++){
		for (int b = 0; b < schooled::MAP_WIDTH; b++){
			stream >> actorArray[a][b];
			if (actorArray[a][b] > 0)
			{
				actorList.push_back(actorIndex[actorArray[a][b]]);
				actorList[actorList.size() - 1].setLocation({ b, a });
			}
		}
	}
	stream.close();

	// Add transition doors to the map
	COORD temp = { 0, 0 };
	for (COORD c : entrances)
	{
		if (c != temp)
		{	
			itemArray[c.Y][c.X] = 2;
		}
	}
}

string Room::getMessage() { return message; }

int Room::save(string fileName)
{
	std::ofstream stream;
	stream.open(schooled::getResourcePath("rooms") + fileName);

	if (stream.fail())
	{
		std::cerr << "File open failed. (Room::save)" << std::endl;
		return 1;
	}

	stream << message << endl << endl;

	// Save the tile array
	for (int a = 0; a < schooled::MAP_HEIGHT; a++){
		for (int b = 0; b < schooled::MAP_WIDTH; b++){
			stream << tileArray[a][b] << " ";
		}
		stream << endl;
	}
	stream << endl;

	// Save the entrance coordinates
	for (COORD &c : entrances)
	{
		stream << c.X << " " << c.Y << endl;
	}
	stream << endl;

	// Save the item array
	for (int a = 0; a < schooled::MAP_HEIGHT; a++){
		for (int b = 0; b < schooled::MAP_WIDTH; b++){
			stream << itemArray[a][b] << " ";
		}
		stream << endl;
	}
	stream << endl;

	// Save the actor array
	for (int a = 0; a < schooled::MAP_HEIGHT; a++){
		for (int b = 0; b < schooled::MAP_WIDTH; b++){
			stream << actorArray[a][b] << " ";
		}
		stream << endl;
	}
	stream << std::flush;
	stream.close();
	return 0;
}

void Room::moveEnemy1(COORD playerPos, Actor& enemy)
{
	// Initializing variables
	int differenceX, differenceY, deltaX, deltaY;
	int enemyX = enemy.getX();
	int enemyY = enemy.getY();
	int playerX = playerPos.X;
	int playerY = playerPos.Y;
	deltaX = 0;
	deltaY = 0;
	differenceX = enemyX - playerX;
	differenceY = enemyY - playerY;
	// Initializing range for attacking distance
	bool inRange = differenceX > -4 && differenceX < 4 && differenceY > -4 && differenceY < 4;

	// If line of sight equals true, or inRange is true but out of line of sight, move
	if (lineOfSight(playerPos, enemy) == true || inRange && lineOfSight(playerPos, enemy) == false)
	{
		// If within attacking range, stop moving
		if (differenceX > -2 && differenceX < 2 && differenceY == 0 ||
			differenceY > -2 && differenceY < 2 && differenceX == 0)
		{
			deltaX = 0;
			deltaY = 0;
		}
		else // Move towards the player
		{
			// Chooses best route
			if (abs(differenceX) > abs(differenceY))
			{
				deltaX = (enemyX > playerX) ? -1 : 1;
				if (!isPassable({ enemyX + deltaX, enemyY + deltaY }))
				{
					deltaX = 0;
					deltaY = (enemyY > playerY) ? -1 : 1;
				}
			}
			else
			{
				deltaY = (enemyY > playerY) ? -1 : 1;
				if (!isPassable({ enemyX + deltaX, enemyY + deltaY }))
				{
					deltaY = 0;
					deltaX = (enemyX > playerX) ? -1 : 1;
				}
			}
		}
	}
	else // If out of sight and out of range, stop moving
	{
		deltaX = 0;
		deltaY = 0;
	}

	if (isPassable({ enemyX + deltaX, enemyY + deltaY }))	//checks if anticipated move is legal, if true redraw the actor
	{
		setActorInt(enemy.getLocation(), 0);
		enemy.setLocation({ enemyX + deltaX, enemyY + deltaY });
		setActorInt(enemy.getLocation(), enemy.getTile().tileInt);
		enemy.setActed(true);
	}
}

// Same as move enemy, but dumber (without the inRange effect, if it can't see you, you aren't there)
void Room::moveCat(COORD playerPos, Actor& enemy)	
{
	int differenceX, differenceY, deltaX, deltaY;
	int enemyX = enemy.getX();
	int enemyY = enemy.getY();
	int playerX = playerPos.X;
	int playerY = playerPos.Y;
	deltaX = 0;
	deltaY = 0;
	differenceX = enemyX - playerX;
	differenceY = enemyY - playerY;

	if (lineOfSight(playerPos, enemy))
	{
		if (differenceX > -2 && differenceX < 2 && differenceY > -2 && differenceY < 2){
			deltaX = 0;
			deltaY = 0;
		}
		else{
			if (differenceX > differenceY)
			{
				if (enemyX > playerX)
					deltaX = -1;
				else if (enemyX < playerX)
					deltaX = 1;
				else
					deltaX = 0;
				if (!isPassable({ enemyX + deltaX, enemyY + deltaY }))
				{
					deltaX = 0;
					if (enemyY > playerY)
						deltaY = -1;
					else if (enemyY < playerY)
						deltaY = 1;
					else
						deltaY = 0;
				}
			}
			else
			{
				if (enemyY > playerY)
					deltaY = -1;
				else if (enemyY < playerY)
					deltaY = 1;
				else
					deltaY = 0;
				if (!isPassable({ enemyX + deltaX, enemyY + deltaY }))
				{
					deltaY = 0;
					if (enemyX > playerX)
						deltaX = -1;
					else if (enemyX < playerX)
						deltaX = 1;
					else
						deltaX = 0;
				}
			}
		}
	}
	else
	{
		deltaX = 0;
		deltaY = 0;
	}
	

	if (isPassable({ enemyX + deltaX, enemyY + deltaY }))
	{
		setActorInt(enemy.getLocation(), 0);
		enemy.setLocation({ enemyX + deltaX, enemyY + deltaY });
		setActorInt(enemy.getLocation(), enemy.getTile().tileInt);
		enemy.setActed(true);
	}
}

void Room::moveMinion(Actor& minion)
{
	// Initializing variables
	int enemyX = minion.getX();
	int enemyY = minion.getY();
	int deltaX = 0;
	int deltaY = 0;
	bool bossDefeated = true;

	// Checks for if the associated enemy is defeated or not
	for (int a = 0; a < schooled::MAP_HEIGHT; a++)
	{
		for (int b = 0; b < schooled::MAP_WIDTH; b++)
		{
			if (getActorInt({ b, a }) == 2 || getActorInt({ b, a }) == 2)
			{
				bossDefeated = false;
			}
		}
	}

	// If the enemy is defeated, move Minion one tile away from the door
	if (bossDefeated == true)
	{
		if (isPassable({ enemyX, enemyY - 1 }) && getTileInt({ enemyX, enemyY - 1 }) != 2)
		{
			deltaX = 0;
			deltaY = -1;
		}
		else if (isPassable({ enemyX, enemyY + 1 }) && getTileInt({ enemyX, enemyY + 1 }) != 2)
		{
			deltaX = 0;
			deltaY = 1;
		}
		else if (isPassable({ enemyX -1, enemyY }) && getTileInt({ enemyX -1, enemyY }) != 2)
		{
			deltaX = -1;
			deltaY = 0;
		}
		else if (isPassable({ enemyX + 1, enemyY }) && getTileInt({ enemyX + 1, enemyY }) != 2)
		{
			deltaX = 1;
			deltaY = 0;
		}
		// Checks if anticipated move is legal, if it is, move Minion
		if (isPassable({ enemyX + deltaX, enemyY + deltaY }))
		{
			setActorInt(minion.getLocation(), 0);
			minion.setLocation({ enemyX + deltaX, enemyY + deltaY });
			setActorInt(minion.getLocation(), 22);
			minion.setActed(true);			// Resets the Minion actor to the SCARED_MINION actor so it won't move again
			minion.setIsFinished(true);
		}
	}
}

bool Room::lineOfSight(COORD playerPos, Actor& enemy)
{
	bool isFound = false;
	int counter = 1;

	// Check the boundary to the left
	while (isPassable({ enemy.getX() - counter, enemy.getY() }))
	{
		counter++;
	}
	enemy.setMinX(enemy.getX() - counter);
	counter = 1;

	// Check the boundary to the right
	while (isPassable({ enemy.getX() + counter, enemy.getY() }))
	{
		counter++;
	}
	enemy.setMaxX(enemy.getX() + counter);
	counter = 1;

	// Check the boundary to the up
	while (isPassable({ enemy.getX(), enemy.getY() - counter }))
	{
		counter++;
	}
	enemy.setMinY(enemy.getY() - counter);
	counter = 1;

	// Check the boundary to the down
	while (isPassable({ enemy.getX(), enemy.getY() + counter }))
	{
		counter++;
	}
	enemy.setMaxY(enemy.getY() + counter);
	counter = 1;

	if (playerPos.X >= enemy.getMinX() && playerPos.Y >= enemy.getMinY() &&
		playerPos.X <= enemy.getMaxX() && playerPos.Y <= enemy.getMaxY())
	{
		enemy.setAggro(true);
		return true;
	}
	enemy.setAggro(false);
	return false;
}

// Checks if enemy is adjacent to player
bool Room::isAdjacent(COORD playerPos, Actor& enemy){
	if (enemy.getX() == playerPos.X && enemy.getY() == playerPos.Y - 1 || 
		enemy.getX() == playerPos.X && enemy.getY() == playerPos.Y + 1 || 
		enemy.getX() == playerPos.X - 1 && enemy.getY() == playerPos.Y || 
		enemy.getX() == playerPos.X + 1 && enemy.getY() == playerPos.Y)
	{
		return true;
	}
	return false;
}
