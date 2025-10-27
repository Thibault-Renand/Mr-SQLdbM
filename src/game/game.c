#include "board.h"
#include "game.h"
#include "../utils.h"
#include "../bdd/testdb.h"  // Pour les fonctions de gestion des enregistrements de parties


void playTurn(char board[BOARD_SIZE][BOARD_SIZE], char player) {
    char alp[8] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H'};
    int x, y;
    char xt;
    bool validMove = false;

    if (player == 'x') puts("C'est au tour du joueur Blanc.");
    if (player == 'o') puts("C'est au tour du joueur Noir.");

    while (!validMove) {
        printf("Entrez votre mouvement (colonne ligne) : ");
        if (scanf(" %c %d", &xt, &y) != 2) {
            printf("Format invalide. Essayez à nouveau.\n");
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
            printf("Mouvement invalide en "RED"%c%d"RESET". Essayez à nouveau.\n", xt, x+1);
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
    // Penser à remettre les arguments player
    
    int scoreX = calculateScore(board, 'x');
    int scoreO = calculateScore(board, 'o');

    printf("Score final :\n");
    printf("Joueur Blanc : %d\n", scoreX);
    printf("Joueur Noir : %d\n", scoreO);

    /*
    GameRecord record;
    record.game_id=uidfinder("./src/bdd/output/gamerecords.dat")+1;
    record.player1_id = player1_id;
    record.player2_id = player2_id;
    record.score_player1 = scoreX;
    record.score_player2 = scoreO;
    // Enregistrer la date de la partie
    strcpy(record.date, "date"); // Remplacer par la date actuelle
    */

    if (scoreX > scoreO) {
        // Enregistrer le gagnant
        // record.gagnant_id = player1_id;
        printf("Le joueur Blanc a gagné !\n");
    } else if (scoreO > scoreX) {
        // record.gagnant_id = player2_id;
        printf("Le joueur Noir a gagné !\n");
    } else {
        printf("C'est une égalité !\n");
    }
    // add_game_record("./src/bdd/output/gamerecords.dat", &record);
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
}
