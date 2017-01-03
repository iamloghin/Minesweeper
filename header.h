#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <sstream>
#include <ctime>
#include <iostream>
#define MAX 1000

/*
0 is empty
1-8 is number
9 is mine
10 is unrevealed square
11 is flag
*/

// Definitions
int width, height, squareNumber, mineNumber, unrevealed, flagNum, minesLeft;
int squareSize = 32;
int gameBorder_top=4, gameBorder_right=1, gameBorder_bottom=1, gameBorder_left=1;
int board[MAX*MAX] = {0};
int boardRevealed[MAX*MAX] = {0}; // -2 is point of start, -1 is point of dead, 0 is unrevealed, 1 is revealed, 2 is flagged, 3 is not sure
bool first_click= true, gameOver = false, won = false;

// My Colors
sf::Color Background_color(192, 192, 192);
sf::Color MyBlack(0, 0, 0, 255);

// Functions
int nearMines(int x, int y);
void countSurrounding(int x, int y);
void placeMines(int first_click_poz);
bool revealed (int x, int y);
void display(int x, int y, int firstX, int firstY);
bool reveal(int x, int y);
void reset();
