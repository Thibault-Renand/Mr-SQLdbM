
#include "board.h"
#include "game.h"
#include "../utils.h"


void playTurn(char board[BOARD_SIZE][BOARD_SIZE], char player) {
    char alp[8] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H'};
    int x, y;
    char xt;
    bool validMove = false;

    if (player == 'x') wprintf(L"C'est au tour du joueur Blanc.\n");
    if (player == 'o') wprintf(L"C'est au tour du joueur Noir.\n");

    while (!validMove) {
        wprintf(L"Entrez votre mouvement (colonne ligne) : ");
        if (scanf(" %c %d", &xt, &y) != 2) {
            wprintf(L"Format invalide. Essayez à nouveau.\n");
            clearInputBuffer();
            continue;
        }

        clearInputBuffer();

       x = y - 1;
       y = findIndex(alp, 8, xt);

        if (isValidMove(board, x, y, player)) {
            makeMove(board, x, y, player);
            validMove = true;
        } else {
            wprintf(L"Mouvement invalide en %ls%c%d%ls. Essayez à nouveau.\n", RED, xt, x+1, RESET);
        }
    }
}


bool canPlay(char board[BOARD_SIZE][BOARD_SIZE], char player) {
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            if (isValidMove(board, i, j, player)) {
                return true;
            }
        }
    }
    return false;
}


bool isGameOver(char board[BOARD_SIZE][BOARD_SIZE]) {
    return !canPlay(board, 'x') && !canPlay(board, 'o');
}


int calculateScore(char board[BOARD_SIZE][BOARD_SIZE], char player) {
    int score = 0;
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            if (board[i][j] == player) {
                score++;
            }
        }
    }
    return score;
}


void printWinner(char board[BOARD_SIZE][BOARD_SIZE]) {
    int scoreX = calculateScore(board, 'x');
    int scoreO = calculateScore(board, 'o');

    wprintf(L"Score final :\n");
    wprintf(L"Joueur Blanc : %d\n", scoreX);
    wprintf(L"Joueur Noir : %d\n", scoreO);

    wprintf(L"%ls", BGRED);
    if (scoreX > scoreO) {
        wprintf(L"Le joueur Blanc a gagné !%ls\n", RESET);
    } else if (scoreO > scoreX) {
        wprintf(L"Le joueur Noir a gagné !%ls\n", RESET);
    } else {
        wprintf(L"C'est une égalité !%ls\n", RESET);
    }
}


void flipDirection(char board[BOARD_SIZE][BOARD_SIZE], int x, int y, int dx, int dy, char player) {
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
        i = x + dx;
        j = y + dy;
        while (board[i][j] == opponent) {
            board[i][j] = player;
            i += dx;
            j += dy;
        }
    }
}


void makeMove(char board[BOARD_SIZE][BOARD_SIZE], int x, int y, char player) {
    //char alp[8] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H'};
    //printf("Le joueur %c joue en "GREEN"%c%d"RESET".\n", player, alp[y], x+1);

    board[x][y] = player;

    int directions[8][2] = {
        {-1,  0}, {1,  0},  // Vertical (haut, bas)
        { 0, -1}, {0,  1},  // Horizontal (gauche, droite)
        {-1, -1}, {1,  1},  // Diagonale (haut-gauche, bas-droite)
        {-1,  1}, {1, -1}   // Diagonale (haut-droite, bas-gauche)
    };

    for (int i = 0; i < 8; i++) {
        int dx = directions[i][0];
        int dy = directions[i][1];
        flipDirection(board, x, y, dx, dy, player);
    }

    //printf("Mouvement effectué en "GREEN"%c%d"RESET".\n", alp[y], x+1);
}
