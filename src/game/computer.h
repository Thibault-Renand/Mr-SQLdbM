#ifndef COMPUTER_H
#define COMPUTER_H

#include <stdlib.h>
#include <string.h>
#include <limits.h>

#include "digraph.h"

int nbCornBord(Board state);
int nbPosMoves(Board state);
int maxCapture(Board state);
void generatePossibleMoves(Board *board, TreeNode *parent);
int alphabeta(TreeNode *node, int depth, bool isMaximizingPlayer, int alpha, int beta);
void playBestMove(TreeNode *root, int depth, bool isMaximizingPlayer);

#endif
