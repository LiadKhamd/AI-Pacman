#pragma once
#include "Point2D.h"
#include "AStar.h"
#include <thread>

const int MIN_RUN = 10;

class Pacman
{
public:
	Pacman(Point2D* pos);
	~Pacman();
	void setMonsters(int size, Point2D* (&monsters));
	void Pacman::setCoins(int numberOfCoins, Point2D** &coins);
	void run();
private:
	Point2D* pos;
	vector<Point2D> coins;
	vector<AStar*> aStarCoins;
	Point2D *lastPosCoin;
	int indexCoin;

	Point2D* monsters;
	int monsterSize;

	AStar** aStarMonsters;
	void runFromMonster();

	void searchCoins();
};

