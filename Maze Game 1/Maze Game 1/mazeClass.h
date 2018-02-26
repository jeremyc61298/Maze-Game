/* mazeClass.h */
// Author: Jeremy Campbell
// Definition file for mazeClass 
#pragma once

#include <stack>
#include <windows.h>

// GLOBAL VARIABLES 
// Holds the size of the grid
const int GRIDROWS = 29;
const int GRIDCOLUMNS = 67;

// Struct used to access points in the grid
struct coordinate
{
	int row = 0;
	int column = 0;
};

class Maze
{
public:
	Maze();
	~Maze();
	void printMaze();
	void buildMaze();
	void playGame();
	void endGame(bool);
	void setTextColor(char);

private:
	// VARIABLES 
	char grid[GRIDROWS][GRIDCOLUMNS];
	std::stack<coordinate> pastLocations;
	coordinate currentLocation;
	coordinate startLocation;
	const char START = 'S';
	const char DESTINATION = 'X';
	const char WALL = (char)219;
	const char PATH = ' ';
	const char PLAYER = 'P';
	const char BREADCRUMB = char(46);

	// FUNCTIONS
	void initalizeStartCoordinate();
	void initializeDestinationCoordinate();
	bool testNorthCoordinate(bool);
	bool testSouthCoordinate(bool);
	bool testWestCoordinate(bool);
	bool testEastCoordinate(bool);
	bool checkForValidDirections();
	void pickRandomDirection();
	void digNorth();
	void digSouth();
	void digEast();
	void digWest();
	int getKey();
	void movePlayer(COORD, COORD);
	void replaceOldCoordinate(COORD, COORD);
};
