
#include "board.h"
#include "../utils.h"


void initBoard(char board[BOARD_SIZE][BOARD_SIZE]) {
	for (int i=0; i < BOARD_SIZE; i++) {
		for (int j=0; j < BOARD_SIZE; j++) {
			board[i][j] = ' ';
			if ((i==3 && j==3) || (i==4 && j==4)) board[i][j] = 'x'; // Blanc
			if ((i==3 && j==4) || (i==4 && j==3)) board[i][j] = 'o'; // Noir
		}
	}
}


void printBoard(char board[BOARD_SIZE][BOARD_SIZE]) {
	char alp[8] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H'};
	for (int i=0; i < BOARD_SIZE; i++) {

		// Lettres
		if (i==0) {
			printf("\t\t   ");
			for (int j=0; j < BOARD_SIZE; j++) {
				printf(" %c  ", alp[j]);
			}
			printf("\n");
		}

		// Plafond de ligne
		printf("\t\t  ");
		printf(BGGREEN);
		for (int j=0; j < BOARD_SIZE; j++) {
			if (j==0 && i==0) printf("╭");
			if (j==0 && i!=0) printf("├"); 
			printf("───");
			if (j%1==0 && i%BOARD_SIZE!=0 && j!=BOARD_SIZE-1) printf("┼"); 
			if (j%1==0 && i==0 && j!=BOARD_SIZE-1) printf("┬");
			if (j==BOARD_SIZE-1 && i!=0) printf("┤");
			if (j==BOARD_SIZE-1 && i==0) printf("╮");
		}
		printf(RESET);
		printf("\n");

		// Ligne
		printf("\t\t%d ", i+1);  // Chiffres
		printf(BGGREEN);
		for (int j=0; j < BOARD_SIZE; j++) {
			if (board[i][j] == 'x') printf("|"GRAY" ⬤ "RESET""BGGREEN);
			if (board[i][j] == 'o') printf("|"BLACK" ⬤ "RESET""BGGREEN);
			if (board[i][j] == ' ') printf("|   ");
			if (j==BOARD_SIZE-1) printf("|");
		}
		printf(RESET);
		printf("\n");
	}

	// Bas du tableau
	printf("\t\t  ");
	printf(BGGREEN);
	for (int j=0; j < BOARD_SIZE; j++) {
		if (j==0) printf("╰");
		printf("───");
		if (j==BOARD_SIZE-1) printf("╯");
		if (j%1==0 && j!=BOARD_SIZE-1) printf("┴");
	}
	printf(RESET);
	printf("\n");
}


bool isValidMove(char board[BOARD_SIZE][BOARD_SIZE], int x, int y, char player) {
    if (!isInBounds(x, y)) return false;  // En dehors du tableau
    if (board[x][y] != ' ') return false; // Case non vide

    int directions[8][2] = {
        {-1,  0}, {1,  0}, // Vertical (haut, bas)
        { 0, -1}, {0,  1}, // Horizontal (gauche, droite)
        {-1, -1}, {1,  1}, // Diagonale (haut-gauche, bas-droite)
        {-1,  1}, {1, -1}  // Diagonale (haut-droite, bas-gauche)
    };

    for (int i = 0; i < 8; i++) {
        int dx = directions[i][0];
        int dy = directions[i][1];

        if (checkDirection(board, x, y, dx, dy, player)) {
            return true;
        }
    }

    return false;
}


bool isInBounds(int x, int y) {
    return x >= 0 && x < BOARD_SIZE && y >= 0 && y < BOARD_SIZE;
}


bool checkDirection(char board[BOARD_SIZE][BOARD_SIZE], int x, int y, int dx, int dy, char player) {
    char opponent = (player == 'x') ? 'o' : 'x';
    int i = x + dx;
    int j = y + dy;
    bool foundOpponent = false;

    while (isInBounds(i, j) && board[i][j] == opponent) {
        foundOpponent = true;
        i += dx;
        j += dy;
    }

    if (foundOpponent && isInBounds(i, j) && board[i][j] == player) {
        return true;
    }

    return false;
}