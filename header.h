#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <ctime>
#include <iostream>
#define MAX 1000

/*
0 is empty
1-8 is number
9 is mine
10 is hide square
11 is flag
*/

// Definitions
int width, height, squareNumber, mineNumber, unrevealed, flagNum;
int squareSize = 32;
int board[MAX * MAX] = { 0 };
int boardRevealed[MAX * MAX] = { 0 }; // 0 is hide, 1 is show, 2 is flag
bool gameOver = false, won = false;

// My Colors
sf::Color Background_color(192, 192, 192);
sf::Color MyBlack(0, 0, 0, 255);

// Functions
int nearMines(int x, int y);
void countSurrounding(int x, int y);
void placeMines();
bool revealed(int x, int y);
void display(int x, int y, int firstX, int firstY);
void reveal(int x, int y);
