#include "GLUT.h"
#include "Const.h"
#include "Monster.h"
#include "Pacman.h"
#include <thread>
#include <time.h>

const int W = 600; // window width
const int H = 600; // window height

int maze[MSIZE][MSIZE];
bool done = false;
const int NUMBER_OF_MONSTER = 2;
const int NUMBER_OF_COINS = 2;

Monster** monster;
Pacman *pacman;
Point2D** coins;

vector<thread> thread_pool;

void SetupMaze()
{
	int i, j;

	for (i = 0; i < MSIZE; i++)
	{
		maze[i][0] = maze[i][MSIZE - 1] = WALL;
		maze[0][i] = maze[MSIZE - 1][i] = WALL;
	}
	// set walls
	for (i = 1; i < MSIZE - 1; i++)
		for (j = 1; j < MSIZE - 1; j++)
		{
			if (i % 2 == 0) // mostly walls
			{
				if (rand() % 100 < 50) maze[i][j] = WALL;
			}
			else // mostly spaces
			{
				if (rand() % 100 < 25) maze[i][j] = WALL;
			}
		}
}

void clearMemory()
{
	for (int i = 0; i < (int)thread_pool.size(); i++)
		thread_pool[i].join();
	thread_pool.clear();
	for (int i = 0; i < NUMBER_OF_MONSTER; i++)
		delete monster[i];
	delete[]monster;
	for (int i = 0; i < NUMBER_OF_COINS; i++)
		delete coins[i];
	delete[]coins;
}

void Clean()
{
	done = false;
	for (int i = 0; i < MSIZE; i++)
	{
		for (int j = 0; j < MSIZE; j++)
		{
			if (maze[i][j] != PACMAN && maze[i][j] != MONSTER&& maze[i][j] != WALL)
				maze[i][j] = SPACE;
		}
	}
}

void init()
{
	Clean();
	int i, j;

	srand(time(0));

	// clean up the maze
	for (i = 0; i < MSIZE; i++)
		for (j = 0; j < MSIZE; j++)
			maze[i][j] = SPACE;

	SetupMaze();

	Point2D* pacmanPos;
	pacmanPos = new Point2D(MSIZE / 2, MSIZE / 2);
	pacman = new Pacman(pacmanPos);
	maze[pacmanPos->getY()][pacmanPos->getX()] = PACMAN;

	monster = new Monster*[NUMBER_OF_MONSTER];
	Point2D* pos;
	int x, y;
	for (int i = 0; i < NUMBER_OF_MONSTER; i++)
	{
		x = rand() % MSIZE;
		y = rand() % MSIZE;
		pos = new Point2D(x, y);
		monster[i] = new Monster(pos, pacmanPos);
		maze[y][x] = MONSTER;
	}

	coins = new Point2D*[NUMBER_OF_COINS];
	for (int i = 0; i < NUMBER_OF_COINS; i++)
	{
		x = rand() % MSIZE;
		y = rand() % MSIZE;
		coins[i] = new Point2D(x, y);
		maze[y][x] = COIN;
	}
	pacman->setCoins(NUMBER_OF_COINS, coins);

	glClearColor(GLclampf(0.7), GLclampf(0.7), GLclampf(0.7), 0);

	glOrtho(-1, 1, -1, 1, -1, 1);
}

void DrawMaze()
{
	int i, j;

	for (i = 0; i < MSIZE; i++)
		for (j = 0; j < MSIZE; j++)
		{
			switch (maze[i][j])
			{
			case WALL:
				glColor3d(0.4, 0, 0); // dark red;
				break;
			case SPACE:
				glColor3d(1, 1, 1); // white;
				break;
			case PACMAN:
				glColor3d(0, 0, 1); // blue;
				break;
			case MONSTER:
				glColor3d(1, 0, 0); // RED;
				break;
			case COIN:
				glColor3d(0.5, 1, 0); //GREENISH YELLOW;
				break;
			}
			// draw square
			glBegin(GL_POLYGON);
			glVertex2d(j*SQSIZE - 1 - SQSIZE / 2, i*SQSIZE - 1 + SQSIZE / 2);
			glVertex2d(j*SQSIZE - 1 + SQSIZE / 2, i*SQSIZE - 1 + SQSIZE / 2);
			glVertex2d(j*SQSIZE - 1 + SQSIZE / 2, i*SQSIZE - 1 - SQSIZE / 2);
			glVertex2d(j*SQSIZE - 1 - SQSIZE / 2, i*SQSIZE - 1 - SQSIZE / 2);
			glEnd();
		}
}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT);

	DrawMaze();

	glutSwapBuffers();// show what was drawn in "frame buffer"
}

void idle()
{
	display();
	glutPostRedisplay();// calls indirectly to display
}

void Menu(int choice)
{
	switch (choice)
	{
	case 1:
		clearMemory();
		init();
		break;
	case 2:
		thread_pool.push_back(std::thread(&Pacman::run, pacman));
		for (int i = 0; i < NUMBER_OF_MONSTER; i++)
			thread_pool.push_back(std::thread(&Monster::run, monster[i]));
		break;
	}
}

void main(int argc, char* argv[])
{
	atexit(clearMemory);
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
	glutInitWindowSize(W, H);
	glutInitWindowPosition(200, 100);
	glutCreateWindow("PACMAN");

	glutDisplayFunc(display); // refresh function
	glutIdleFunc(idle); // idle: when nothing happens
	init();

	glutCreateMenu(Menu);
	glutAddMenuEntry("New maze", 1);
	glutAddMenuEntry("Play", 2);
	glutAttachMenu(GLUT_RIGHT_BUTTON);

	glutMainLoop();
}