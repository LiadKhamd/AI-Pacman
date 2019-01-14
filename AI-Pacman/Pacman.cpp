#include "Pacman.h"

extern int maze[MSIZE][MSIZE];
extern bool done;

Pacman::Pacman(Point2D* pos)
{
	this->pos = pos;
	this->lastPosCoin = NULL;
}


Pacman::~Pacman()
{
	delete lastPosCoin;
	delete pos;
}

void Pacman::setMonsters(int size, Point2D* (&monsters))
{
	//this->monsters = monsters;
	//monsterSize = size;
	//aStarMonsters = new AStar*[size];
	//for (int i = 0; i < size; i++)
	//	aStarMonsters[i] = new AStar(monsters[i]);
}

void Pacman::setCoins(int numberOfCoins, Point2D** &coins)
{
	AStar *astar;
	for (int i = 0; i < numberOfCoins; i++)
	{
		astar = new AStar(pos);
		if (astar->run(*(coins[i]))) {
			this->coins.push_back(*(coins[i]));
			this->aStarCoins.push_back(astar);
		}
		else
			delete astar;
	}
	if (this->coins.size() == 0)
		done = true;
}

void Pacman::run() {
	//int minH, temp;

	//check visibale coins and monster

	while (!done)
	{
		//for (int i = 0; i < monsterSize; i++)
		//{
		//	temp = abs(pos.getX() - monsters[i].getX()) + abs(pos.getY() - monsters[i].getY());// Manhattan Distance
		//	if (i == 0 || minH > temp) {
		//		minH = temp;
		//	}
		//}
		//if (minH <= MIN_RUN) {
		//	runFromMonster();
		//}
		//else {
		searchCoins();
		//}
	}
}

void Pacman::runFromMonster()
{

}

void Pacman::searchCoins()
{
	if (lastPosCoin == NULL || !(*pos == *lastPosCoin))
	{
		int minLen, temp;
#pragma omp parallel for ordered schedule(dynamic)
		for (int i = 0; i < (int)coins.size(); i++)
		{
			aStarCoins[i]->run(coins[i]);
			temp = aStarCoins[i]->getLen();
			if (i == 0 || minLen > temp) {
				indexCoin = i;
				minLen = temp;
			}
		}
	}
	Point2D* helper = aStarCoins[indexCoin]->getTop();
	this_thread::sleep_for(chrono::microseconds(PACMAN_SLEEP));
	(maze)[pos->getY()][pos->getX()] = SPACE;
	pos->copy(*helper);
	delete helper;
	delete lastPosCoin;
	if ((maze)[pos->getY()][pos->getX()] == COIN) {
		coins.erase(coins.begin() + indexCoin);
		delete aStarCoins[indexCoin];
		lastPosCoin = NULL;
		aStarCoins.erase(aStarCoins.begin() + indexCoin);
		if (coins.size() == 0)
			done = true;
	}
	else
	lastPosCoin = new Point2D(*pos);
	(maze)[pos->getY()][pos->getX()] = PACMAN;
}