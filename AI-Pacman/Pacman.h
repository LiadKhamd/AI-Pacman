#pragma once
#include "Point2D.h"
#include "AStar.h"
#include <thread>

const int MIN_RUN = 30;

class Pacman
{
public:
	Pacman(Point2D* pos);
	~Pacman();
	void Pacman::setMonsters(int numberOfMonster, Point2D** &monsters);
	void Pacman::setCoins(int numberOfCoins, Point2D** &coins);
	void run();
private:
	bool win;

	Point2D* pos;

	vector<Point2D> coins;
	vector<AStar*> aStarCoins;
	Point2D *lastPosCoin;
	int indexCoin;

	vector<Point2D*> monsters;
	AStar* runMonster;
	Point2D *lastPosRunMonster;
	bool runFromMonster();
	void newFromMonsterPoint(Point2D* &monster);

	void searchCoins();
};

