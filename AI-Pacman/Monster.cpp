#include "Monster.h"

extern int maze[MSIZE][MSIZE];
extern bool done;

Monster::Monster(Point2D* pos, Point2D* target)
{
	this->pos = pos;
	this->pacMan = target;
	aStar = new AStar(pos);
	lastPosColor = SPACE;
}


Monster::~Monster()
{
	delete pos;
	delete aStar;
}

void Monster::run() {
	Point2D* helper = NULL;
	while (!done)
	{
		if (aStar->run(*pacMan,-1)) {
			delete helper;
			helper = aStar->getTop();
			if (helper != NULL && !(*helper == *pacMan)) {
				this_thread::sleep_for(chrono::microseconds(MONSTER_SLEEP));
				(maze)[pos->getY()][pos->getX()] = lastPosColor;
				pos->copy(*helper);
				lastPosColor = (maze)[pos->getY()][pos->getX()];
				(maze)[pos->getY()][pos->getX()] = MONSTER;
			}
			else
				done = true;
		}
		else
			break;
	}
	delete helper;
}