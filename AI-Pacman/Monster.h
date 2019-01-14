#pragma once
#include "Const.h"
#include "AStar.h"
#include <thread>

class Monster
{
public:
	Monster(Point2D* pos, Point2D* target);
	~Monster();
	void run();
private:
	Point2D *pos;
	Point2D *pacMan;
	AStar *aStar;
	int lastPosColor;
};

