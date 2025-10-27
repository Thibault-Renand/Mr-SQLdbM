#ifndef GAME_H
#define GAME_H

#include <stdio.h>
#include <stdbool.h>

#include "board.h"
#include "../utils.h"


void playTurn(Board* state);
bool canPlay(char board[BOARD_SIZE][BOARD_SIZE]);
void makeMove(Board* state, int x, int y);
bool winner(char board[BOARD_SIZE][BOARD_SIZE]);
void printWinner(Board state);

#endif