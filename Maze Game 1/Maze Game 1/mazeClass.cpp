/* mazeClass.cpp */
// Author: Jeremy Campbell
// Implementation file for mazeClass
#include "mazeClass.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <windows.h>

using std::cout;
using std::endl;

Maze::Maze()
{
	buildMaze();
	printMaze();
}

Maze::~Maze()
{

}

void Maze::buildMaze()
{
	srand(time(NULL));

	// Fill the array with walls
	for (int i = 0; i < GRIDROWS; i++)
	{
		for (int j = 0; j < GRIDCOLUMNS; j++)
		{
			grid[i][j] = WALL;
		}
	}

	initalizeStartCoordinate();
	grid[currentLocation.row][currentLocation.column] = PATH;

	do
	{
		if (checkForValidDirections())
		{
			pastLocations.push(currentLocation);
			pickRandomDirection();
		}
		else
		{
			// Backtrack - take the new currentLocation from the 
			// top of the stack
			currentLocation = pastLocations.top();
			pastLocations.pop();
		}
	} while (!pastLocations.empty());

	grid[startLocation.row][startLocation.column] = START;
	initializeDestinationCoordinate();

}

void Maze::printMaze()
{
	for (int i = 0; i < GRIDROWS; i++)
	{
		for (int j = 0; j < GRIDCOLUMNS; j++)
		{
			setTextColor(grid[i][j]);
			cout << grid[i][j];
		}
		cout << endl;
	}
}

// This function sets specific colors to specific characters to be printed. 
void Maze::setTextColor(char charToPrint)
{
	if (charToPrint == WALL)
	{
		// light grey
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 8);
	}
	else if (charToPrint == START)
	{
		// green
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 2);
	}
	else if (charToPrint == DESTINATION)
	{
		// red
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 4);
	}
	else if (charToPrint == PLAYER)
	{
		// yellow
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 14);
	}
	else if (charToPrint == BREADCRUMB)
	{
		// light blue
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 11);
	}
	else
	{
		// white, default
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
	}
}

// Randomly choose a destination coordinate that is odd,odd
void Maze::initalizeStartCoordinate()
{
	int column;
	int row;
	do
	{
		row = rand() % GRIDROWS;
	} while (row % 2 == 0);

	do
	{
		column = rand() % GRIDCOLUMNS;
	} while (column % 2 == 0);

	currentLocation.row = row;
	currentLocation.column = column;
	startLocation = currentLocation;
}

// Randomly choose a destination coordinate on the Path
void Maze::initializeDestinationCoordinate()
{
	coordinate destination;
	do
	{
		destination.row = rand() % GRIDROWS;
		destination.column = rand() % GRIDCOLUMNS;
	} while (grid[destination.row][destination.column] != PATH);

	grid[destination.row][destination.column] = DESTINATION;
}

// Test each direction, but do not actually dig.
bool Maze::checkForValidDirections()
{
	bool possibleValidMove = false;
	bool dig = false;
	if (testNorthCoordinate(dig) ||
		testSouthCoordinate(dig) ||
		testEastCoordinate(dig) ||
		testWestCoordinate(dig))
	{
		possibleValidMove = true;
	}
	return possibleValidMove;
}

bool Maze::testNorthCoordinate(bool dig)
{
	bool isValidMove = false;
	coordinate northCoordinate;
	northCoordinate.row = currentLocation.row - 2;
	northCoordinate.column = currentLocation.column;

	if (grid[northCoordinate.row][northCoordinate.column] != PATH && northCoordinate.row > 0)
	{
		// Simply test the direction if dig is false. If true, dig in that direction.
		isValidMove = true;
		if (dig)
		{
			digNorth();
			currentLocation = northCoordinate;
		}
	}
	return isValidMove;
}

bool Maze::testSouthCoordinate(bool dig)
{
	bool isValidMove = false;
	coordinate southCoordinate;
	southCoordinate.row = currentLocation.row + 2;
	southCoordinate.column = currentLocation.column;

	if (grid[southCoordinate.row][southCoordinate.column] != PATH && southCoordinate.row < GRIDROWS)
	{
		// Simply test the direction if dig is false. If true, dig in that direction.
		isValidMove = true;
		if (dig)
		{
			digSouth();
			currentLocation = southCoordinate;
		}
	}
	return isValidMove;
}

bool Maze::testEastCoordinate(bool dig)
{
	bool isValidMove = false;
	coordinate eastCoordinate;
	eastCoordinate.row = currentLocation.row;
	eastCoordinate.column = currentLocation.column + 2;

	if (grid[eastCoordinate.row][eastCoordinate.column] != PATH && eastCoordinate.column < GRIDCOLUMNS)
	{
		// Simply test the direction if dig is false. If true, dig in that direction.
		isValidMove = true;
		if (dig)
		{
			digEast();
			currentLocation = eastCoordinate;
		}
	}
	return isValidMove;
}

bool Maze::testWestCoordinate(bool dig)
{
	bool isValidMove = false;
	coordinate westCoordinate;
	westCoordinate.row = currentLocation.row;
	westCoordinate.column = currentLocation.column - 2;

	if (grid[westCoordinate.row][westCoordinate.column] != PATH && westCoordinate.column > 0)
	{
		// Simply test the direction if dig is false. If true, dig in that direction.
		isValidMove = true;
		if (dig)
		{
			digWest();
			currentLocation = westCoordinate;
		}
	}
	return isValidMove;
}

void Maze::pickRandomDirection()
{
	// Dig will test the test functions to actually
	// change currentLocation 
	bool dig = true;
	bool moved = false;
	while (!moved)
	{
		int whichFunction = 1 + rand() % 4;
		switch (whichFunction)
		{
		case 1:
			if (testNorthCoordinate(dig))
			{
				moved = true;
			}
			break;
		case 2:
			if (testSouthCoordinate(dig))
			{
				moved = true;
			}
			break;
		case 3:
			if (testEastCoordinate(dig))
			{
				moved = true;
			}
			break;
		case 4:
			if (testWestCoordinate(dig))
			{
				moved = true;
			}
			break;
		}
	}
}

void Maze::digNorth()
{
	for (int i = currentLocation.row - 1; i >= currentLocation.row - 2; i--)
	{
		grid[i][currentLocation.column] = PATH;
	}
}

void Maze::digSouth()
{
	for (int i = currentLocation.row + 1; i <= currentLocation.row + 2; i++)
	{
		grid[i][currentLocation.column] = PATH;
	}
}

void Maze::digEast()
{
	for (int i = currentLocation.column + 1; i <= currentLocation.column + 2; i++)
	{
		grid[currentLocation.row][i] = PATH;
	}
}

void Maze::digWest()
{
	for (int i = currentLocation.column - 1; i >= currentLocation.column - 2; i--)
	{
		grid[currentLocation.row][i] = PATH;
	}
}

//This function was written by Dr. Steil 
int Maze::getKey()
{
	int result = 0;
	while (result == 0)
	{
		short MAX_SHORT = 0x7FFF; //111111111111111
		if (GetAsyncKeyState(VK_LEFT) & MAX_SHORT) {
			result = VK_LEFT;
		}
		else if (GetAsyncKeyState(VK_UP) & MAX_SHORT) {
			result = VK_UP;
		}
		else if (GetAsyncKeyState(VK_RIGHT) & MAX_SHORT) {
			result = VK_RIGHT;
		}
		else if (GetAsyncKeyState(VK_DOWN) & MAX_SHORT) {
			result = VK_DOWN;
		}
		else if (GetAsyncKeyState(VK_ESCAPE) & MAX_SHORT) {
			result = VK_ESCAPE;
		}
	}
	return result;
}

//This function, playGame, was given by Dr.Steil, and modified by myself, Jeremy C.
void Maze::playGame()
{
	int row = startLocation.row;
	int column = startLocation.column;
	int key = getKey();
	COORD oldCoord = { column, row };
	COORD firstCoord = { column, row };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), firstCoord);

	while (key != VK_ESCAPE) {
		if (key == VK_LEFT && column > 0 && grid[row][column - 1] != WALL) {
			column--;
		}
		else if (key == VK_RIGHT && grid[row][column + 1] != WALL) {
			column++;
		}
		else if (key == VK_UP && row > 0 && grid[row - 1][column] != WALL) {
			row--;
		}
		else if (key == VK_DOWN && grid[row + 1][column] != WALL) {
			row++;
		}

		COORD newCoord = { column, row };

		// This will display to the screen where the user's current location
		// is on the grid, but in memory it will store a breadcrumb at their
		// current location.
		replaceOldCoordinate(oldCoord, newCoord);

		movePlayer(oldCoord, newCoord);
		oldCoord = newCoord;
		key = getKey();
	}

	//Did not solve the maze
	endGame(false);
}

void Maze::movePlayer(COORD oldCoord, COORD newCoord)
{
	// If the maze has been solved, endGame. Keep playing if not.
	// The player will disappear, showing the completed maze
	if (grid[newCoord.Y][newCoord.X] == DESTINATION)
	{
		SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), oldCoord);
		endGame(true);
	}
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), newCoord);
	setTextColor(PLAYER);
	cout << PLAYER;
}

// Upon moving, this will replace the character where the player previously was on the grid, 
// in memory, and on the screen. Start will never change in memory.
void Maze::replaceOldCoordinate(COORD oldCoord, COORD newCoord)
{
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), oldCoord);

	// If player is stepping off the startLocation
	if (grid[oldCoord.Y][oldCoord.X] == grid[startLocation.row][startLocation.column])
	{
		setTextColor(START);
		cout << START;
	}
	// If the player is stepping on anything besides a Path
	else if (grid[newCoord.Y][newCoord.X] != PATH)
	{
		grid[oldCoord.Y][oldCoord.X] = PATH;
		cout << PATH;
	}
	// If the player is stepping on the Path and not the startLocation
	else if (grid[oldCoord.Y][oldCoord.X] != grid[startLocation.row][startLocation.column])
	{
		grid[oldCoord.Y][oldCoord.X] = BREADCRUMB;
		setTextColor(BREADCRUMB);
		cout << BREADCRUMB;
	}
}

void Maze::endGame(bool won)
{
	COORD endMessageLocation = { 0, GRIDROWS + 1 };

	if (won)
	{
		//Drop the last breadcrumb
		setTextColor(BREADCRUMB);
		cout << BREADCRUMB;
		SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), endMessageLocation);
		setTextColor('W');
		cout << "You've solved the maze!" << endl;
	}
	else
	{
		SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), endMessageLocation);
		setTextColor('W');
		cout << "Nice try" << endl;
	}
	exit(0);
}

