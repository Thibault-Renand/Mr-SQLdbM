#ifndef BOARD_H
#define BOARD_H


#include <stdio.h>
#include <stdbool.h>

#define BOARD_SIZE 3


typedef struct {
    char cells[BOARD_SIZE][BOARD_SIZE]; 
    char currentPlayer; // 'X' ou 'O'
} Board;


Board* initBoard();
void printBoard(char board[BOARD_SIZE][BOARD_SIZE]);
bool isInBounds(int x, int y);
bool isValidMove(char board[BOARD_SIZE][BOARD_SIZE], int x, int y);

#endif