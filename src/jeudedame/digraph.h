#ifndef DIGRAPH_H
#define DIGRAPH_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>


#define BOARD_SIZE 8


typedef struct {
    char cells[BOARD_SIZE][BOARD_SIZE]; 
    char currentPlayer; // 'x' ou 'o'
} Board;


typedef struct TreeNode {
    Board state;
    int value;  
    struct TreeNode **children;
    int numChildren;
} TreeNode;


TreeNode* create_node(Board state);
void add_child(TreeNode *parent, TreeNode *child);
void free_tree(TreeNode *node);


#endif
