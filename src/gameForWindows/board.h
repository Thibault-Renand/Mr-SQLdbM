#ifndef BOARD_H
#define BOARD_H


#include <stdio.h>
#include <stdbool.h>

#define BOARD_SIZE 8


void initBoard(char board[BOARD_SIZE][BOARD_SIZE]);
void printBoard(char board[BOARD_SIZE][BOARD_SIZE]);
bool isInBounds(int x, int y);
bool checkDirection(char board[BOARD_SIZE][BOARD_SIZE], int x, int y, int dx, int dy, char player);
bool isValidMove(char board[BOARD_SIZE][BOARD_SIZE], int x, int y, char player);

#endif