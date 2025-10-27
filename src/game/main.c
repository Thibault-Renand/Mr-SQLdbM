
#include "board.h"
#include "game.h"
#include "../bdd/testdb.h"
#include "../utils.h"
#include "computer.h"
#include "digraph.h"


int main() {

    /*
    users player1, player2,bin;
    //saisieuser("./src/bdd/output/logsinscriptions.dat",bin);
    // Connexion du premier joueur
    printf("Joueur 1, veuillez vous connecter :\n");
    saisieuserlogins(&player1);
    if (!connectionuser(&player1, "./src/bdd/output/logsinscriptions.dat")) {
        printf("Échec de la connexion pour Joueur 1.\n");
        return 1;
    }

    // Connexion du deuxième joueur (si nécessaire)
    printf("Joueur 2, veuillez vous connecter :\n");
    saisieuserlogins(&player2);
    if (!connectionuser(&player2, "./src/bdd/output/logsinscriptions.dat")) {
        printf("Échec de la connexion pour Joueur 2.\n");
        return 1;
    }
    */

    system("clear");

    char alp[8] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H'};
    char board[BOARD_SIZE][BOARD_SIZE];
    char currentPlayer = 'o';  // Noir

    initBoard(board);

    printf("Bienvenue au jeu de Othello !\n");
    printf("o : noir\nx : blanc\n\n");
    printBoard(board);

    while (!isGameOver(board)) {
        if (canPlay(board, currentPlayer)) {
            if (currentPlayer == 'x') {  // Blanc (IA)

                Board initialBoard;
                memcpy(initialBoard.cells, board, sizeof(board));
                initialBoard.currentPlayer = currentPlayer;

                TreeNode *root = create_node(initialBoard);

                puts("L'IA réfléchit...");

                // Trouver mouvement
                int moveX = 0; 
                int moveY = 0;
                Board tempBoard;
                memcpy(&tempBoard, board, sizeof(board));

                playBestMove(root, 8, true);

                memcpy(board, root->state.cells, sizeof(board));

                for (int x = 0; x < BOARD_SIZE; x++) {
                    for (int y = 0; y < BOARD_SIZE; y++) {
                        if (tempBoard.cells[x][y] == ' ' && board[x][y] == currentPlayer) {
                            moveX = x;
                            moveY = y;
                        }
                    }
                }

                printf("L'IA joue en %s%c%d%s!\n", GREEN, alp[moveY], moveX+1, RESET);

                free_tree(root);
            } else {
                playTurn(board, currentPlayer);
            }

            printBoard(board);
        } else {
            printf("Le joueur %c ne peut pas jouer. Tour passé.\n", currentPlayer);
        }
        currentPlayer = (currentPlayer == 'x') ? 'o' : 'x';
    }

    printf("La partie est terminée !\n");
    printWinner(board);
    //printWinner(board,player1.uid,player2.uid);

    return 0;
}
