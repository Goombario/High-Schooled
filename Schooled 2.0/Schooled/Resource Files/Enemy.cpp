#include "../Header Files/Enemy.h"

int Enemy::getMinX(){ return minX; }
int Enemy::getMaxX(){ return maxX; }
int Enemy::getMinY(){ return minY; }
int Enemy::getMaxY(){ return maxY; }

void Enemy::setMinX(int x){ minX = x; }
void Enemy::setMaxX(int x){ maxX = x; }
void Enemy::setMinY(int y){ minY = y; }
void Enemy::setMaxY(int y){ maxY = y; }