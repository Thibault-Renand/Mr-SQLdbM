
#include "board.h"
#include "../utils.h"


Board* initBoard() {
    Board* state = (Board*)malloc(sizeof(Board));
    if (state == NULL) {
        printf("Erreur : Allocation de mémoire échouée.\n");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            state->cells[i][j] = ' ';
        }
    }

    state->currentPlayer = 'X';

    return state;
}


void printBoard(char board[BOARD_SIZE][BOARD_SIZE]) {
	char alp[3] = {'A', 'B', 'C'};
	for (int i=0; i < BOARD_SIZE; i++) {

		// Lettres
		if (i==0) {
			printf("\t\t  ");
			for (int j=0; j < BOARD_SIZE; j++) {
				printf(" %c  ", alp[j]);
			}
			printf("\n");
		}

		// Plafond de ligne
		printf("\t\t ");
		for (int j=0; j < BOARD_SIZE; j++) {
			if (j==0 && i==0) printf("╭");
			if (j==0 && i!=0) printf("├"); 
			printf("───");
			if (j%1==0 && i%BOARD_SIZE!=0 && j!=BOARD_SIZE-1) printf("┼"); 
			if (j%1==0 && i==0 && j!=BOARD_SIZE-1) printf("┬");
			if (j==BOARD_SIZE-1 && i!=0) printf("┤");
			if (j==BOARD_SIZE-1 && i==0) printf("╮");
		}
		printf("\n");

		// Ligne
		printf("\t\t%d", i+1);  // Chiffres
		for (int j=0; j < BOARD_SIZE; j++) {
			printf("| %c ", board[i][j]);
			if (j==BOARD_SIZE-1) printf("|");
		}
		printf("\n");
	}

	// Bas du tableau
	printf("\t\t ");
	for (int j=0; j < BOARD_SIZE; j++) {
		if (j==0) printf("╰");
		printf("───");
		if (j==BOARD_SIZE-1) printf("╯");
		if (j%1==0 && j!=BOARD_SIZE-1) printf("┴");
	}
	printf("\n");
}


bool isValidMove(char board[BOARD_SIZE][BOARD_SIZE], int x, int y) {
    if (!isInBounds(x, y)) return false;
    if (board[x][y] != ' ') return false;

    return true;
}


bool isInBounds(int x, int y) {
    return x >= 0 && x < BOARD_SIZE && y >= 0 && y < BOARD_SIZE;
}
