#ifndef JDDIA_H
#define JDDIA_H

#include <stdlib.h>
#include <string.h>
#include <limits.h>

#include "digraph.h"

int nbCenterControl(Board state);
int nbPosMoves(Board state);
int nbKings(Board state);
void generatePossibleMoves(Board *board, TreeNode *parent);
int alphabeta(TreeNode *node, int depth, bool isMaximizingPlayer, int alpha, int beta);
void playBestMove(TreeNode *root, int depth, bool isMaximizingPlayer);

#endif
