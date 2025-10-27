#ifndef GAME_H
#define GAME_H

#include <stdio.h>
#include <stdbool.h>


void playTurn(char board[8][8], char player);
bool canPlay(char board[BOARD_SIZE][BOARD_SIZE], char player);
void flipDirection(char board[BOARD_SIZE][BOARD_SIZE], int x, int y, int dx, int dy, char player);
void makeMove(char board[BOARD_SIZE][BOARD_SIZE], int x, int y, char player);
bool isGameOver(char board[8][8]);
int calculateScore(char board[8][8], char player);
//void printWinner(char board[BOARD_SIZE][BOARD_SIZE], int player1_id, int player2_id);
void printWinner(char board[BOARD_SIZE][BOARD_SIZE]);

#endif

