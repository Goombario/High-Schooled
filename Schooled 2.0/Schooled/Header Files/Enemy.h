#ifndef ENEMY_H
#define ENEMY_H

#include "Actor.h"

class Enemy : public Actor
{
public:
	int getMinX();
	int getMaxX();
	int getMinY();
	int getMaxY();

	void setMinX(int);
	void setMaxX(int);
	void setMinY(int);
	void setMaxY(int);

private:
	int minX, maxX, minY, maxY;
};

#endif