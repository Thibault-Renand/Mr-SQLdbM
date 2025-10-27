
#include "game.h"


void playTurn(Board* state) {
    char alp[3] = {'A', 'B', 'C'};
    int x, y;
    char xt;
    bool validMove = false;

    printf("C'est au tour du joueur %c !\n", state->currentPlayer);

    while (!validMove) {
        printf("Entrez votre mouvement (colonne ligne) : ");
        if (scanf(" %c %d", &xt, &y) != 2) {
            printf("Format invalide. Essayez à nouveau.\n");
            clearInputBuffer();
            continue;
        }

        clearInputBuffer();

        x = y - 1;
        y = findIndex(alp, BOARD_SIZE, xt);

        if (isValidMove(state->cells, x, y)) {
            makeMove(state, x, y);
            validMove = true;
        } else {
            printf("Mouvement invalide en "RED"%c%d"RESET". Essayez à nouveau.\n", xt, x+1);
        }
    }
}


bool canPlay(char board[BOARD_SIZE][BOARD_SIZE]) {
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            if (isValidMove(board, i, j)) {
                return true;
            }
        }
    }
    return false;
}

bool winner(char board[BOARD_SIZE][BOARD_SIZE]) {
    // Lignes
    for (int i = 0; i < BOARD_SIZE; i++) {
        if (board[i][0] != ' ' && board[i][0] == board[i][1] && board[i][1] == board[i][2]) {
            return true;
        }
    }

    // Colonnes
    for (int j = 0; j < BOARD_SIZE; j++) {
        if (board[0][j] != ' ' && board[0][j] == board[1][j] && board[1][j] == board[2][j]) {
            return true;
        }
    }

    // Diagonale gauche
    if (board[0][0] != ' ' && board[0][0] == board[1][1] && board[1][1] == board[2][2]) {
        return true;
    }

    // Diagonale droite
    if (board[0][2] != ' ' && board[0][2] == board[1][1] && board[1][1] == board[2][0]) {
        return true;
    }

    return false;
}



void printWinner(Board state) {
    if (winner(state.cells)) {
        printf("Le joueur %c a gagné !\n", (state.currentPlayer == 'X') ? 'O' : 'X');
    } else {
        printf("C'est un match nul !\n");
    }
}


void makeMove(Board* state, int x, int y) {
    state->cells[x][y] = state->currentPlayer;
}
