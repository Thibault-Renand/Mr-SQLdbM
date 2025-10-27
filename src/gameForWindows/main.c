
#include "board.h"
#include "game.h"
#include "computer.h"
#include "digraph.h"
#include "../utils.h"


int main() {
    setlocale(LC_ALL, "");
    system("cls");

    //char alp[8] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H'};
    char board[BOARD_SIZE][BOARD_SIZE];
    char currentPlayer = 'o';  // Noir

    int choix;

    // Initialisation du plateau
    initBoard(board);

    wprintf(L"Bienvenue au jeu de Othello !\n");

    do {
        wprintf(L" (0) Quitter\n (1) Jouer \n");
        wprintf(L"Choisissez ce que vous voulez faire : ");
        if (scanf(" %d", &choix) != 1) {
            wprintf(L"Format invalide. Essayez à nouveau.\n");
            clearInputBuffer();
            continue;
        }
        clearInputBuffer();

        switch (choix) {

        case 0:
            wprintf(L"Bye Bye !\n");
            break;
        case 1:
            wprintf(L"o : noir\nx : blanc\n\n");
            printBoard(board);

            while (!isGameOver(board)) {
                if (canPlay(board, currentPlayer)) {
                    if (currentPlayer == 'x') {  // Blanc (IA)
                        // Création de l'état initial pour l'IA
                        Board initialBoard;
                        memcpy(initialBoard.cells, board, sizeof(board));
                        initialBoard.currentPlayer = currentPlayer;

                        TreeNode *root = create_node(initialBoard);

                        // L'IA joue son meilleur coup
                        const wchar_t texte_ia[] = L"L'IA réfléchit...";
                        afficher_progressivement_texte(texte_ia, 100);
                        int moveX = -1; 
                        int moveY = -1;

                        playBestMove(root, 5, true, &moveX, &moveY);  // Profondeur 5 pour l'IA

                        // Mise à jour du plateau après le coup de l'IA
                        memcpy(board, root->state.cells, sizeof(board));

                        wprintf(L"L'IA joue en %d%d!\n", moveX+1, moveY+1);

                        // Libération de la mémoire de l'arbre
                        free_tree(root);
                    } else {
                        // Joueur humain (Noir)
                        playTurn(board, currentPlayer);
                    }

                    printBoard(board);
                } else {
                    printf("Le joueur %c ne peut pas jouer. Tour passé.\n", currentPlayer);
                }

                currentPlayer = (currentPlayer == 'x') ? 'o' : 'x';
            }

            wprintf(L"La partie est terminée !\n");
            printWinner(board);
            break;
        default:
            wprintf(L"Commande invalide. Réessayez.\n");
            break;
        }

    } while (choix != 0);    

    return 0;
}
