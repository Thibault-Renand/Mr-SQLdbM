
#include "computer.h"
#include "game.h"
#include "board.h"
#include "digraph.h"
#include "../utils.h"


// Nombre de pions dans game.h : calculateScore()
// Jouer coup dans game.h      : MakeMove()
// Coup valide dans board.h    : isValidMove()


/* 
Fonction d'évaluations :
	Nombre de pions              : +1 par pion.
	Contrôle des coins/bordures  : +6/3 par pion.
	Mobilité                     : +1 par coup possible.
	Capture                      : +max(N) avec N le nombre de pions capturés pour un placement.
*/


int nbCornBord(Board state) {
	int a = 0;
	if (state.cells[0][0] == state.currentPlayer) a+=3;
	if (state.cells[0][BOARD_SIZE-1] == state.currentPlayer) a+=3;
	if (state.cells[BOARD_SIZE-1][0] == state.currentPlayer) a+=3;
	if (state.cells[BOARD_SIZE-1][BOARD_SIZE-1] == state.currentPlayer) a+=3;
	for (int i=0; i < BOARD_SIZE; i++) {
		if (state.cells[0][i] == state.currentPlayer) a+=3;
		if (state.cells[i][0] == state.currentPlayer) a+=3;
		if (state.cells[BOARD_SIZE-1][i] == state.currentPlayer) a+=3;
		if (state.cells[i][BOARD_SIZE-1] == state.currentPlayer) a+=3;
	}
	return a;
}


int nbPosMoves(Board state) {
	int a = 0;
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            if (isValidMove(state.cells, i, j, state.currentPlayer)) a++;
        }
    }
    return a;
}


int maxCapture(Board state) {
	// regarder tous les mouvements possibles
	// les faire et les évaluer
	return 0;
}


void generatePossibleMoves(Board *board, TreeNode *parent) {
    for (int x = 0; x < BOARD_SIZE; x++) {
        for (int y = 0; y < BOARD_SIZE; y++) {
            // Vérifie si le mouvement est valide
            if (isValidMove(board->cells, x, y, board->currentPlayer)) {
                // Crée une copie du plateau actuel
                Board newBoard;
                memcpy(&newBoard, board, sizeof(Board));

                // Applique le mouvement au plateau
                makeMove(newBoard.cells, x, y, newBoard.currentPlayer);

                // Change le joueur courant
                newBoard.currentPlayer = (newBoard.currentPlayer == 'x') ? 'o' : 'x';

                // Crée un nouveau nœud pour cet état
                TreeNode *child = create_node(newBoard);

                // Ajoute ce nœud comme enfant du parent
                add_child(parent, child);
            }
        }
    }
}


int minimax(TreeNode *node, int depth, bool isMaximizingPlayer, int alpha, int beta) {
    // Condition d'arrêt : profondeur maximale atteinte ou jeu terminé
    if (depth == 0 || isGameOver(node->state.cells)) {
        return calculateScore(node->state.cells, node->state.currentPlayer);  // HEURISTIQUE
    }

    if (isMaximizingPlayer) {
        int maxEval = INT_MIN;

        // Génère les enfants si non générés
        if (node->numChildren == 0) {
            generatePossibleMoves(&node->state, node);
        }

        // Parcourt les enfants
        for (int i = 0; i < node->numChildren; i++) {
            int eval = minimax(node->children[i], depth - 1, false, alpha, beta);
            maxEval = (eval > maxEval) ? eval : maxEval;

            // Met à jour alpha
            alpha = (alpha > eval) ? alpha : eval;

            // Élagage alpha-bêta
            if (beta <= alpha) {
                break;
            }
        }
        return maxEval;
    } else {
        int minEval = INT_MAX;

        // Génère les enfants si non générés
        if (node->numChildren == 0) {
            generatePossibleMoves(&node->state, node);
        }

        // Parcourt les enfants
        for (int i = 0; i < node->numChildren; i++) {
            int eval = minimax(node->children[i], depth - 1, true, alpha, beta);
            minEval = (eval < minEval) ? eval : minEval;

            // Met à jour beta
            beta = (beta < eval) ? beta : eval;

            // Élagage alpha-bêta
            if (beta <= alpha) {
                break;
            }
        }
        return minEval;
    }
}


void playBestMove(TreeNode *root, int depth, bool isMaximizingPlayer, int *moveX, int *moveY) {
    int bestValue = isMaximizingPlayer ? INT_MIN : INT_MAX;
    TreeNode *bestMove = NULL;

    // Génère les mouvements possibles pour l'état actuel
    if (root->numChildren == 0) {
        generatePossibleMoves(&root->state, root);
    }

    // Parcourt les enfants et applique Minimax
    for (int i = 0; i < root->numChildren; i++) {
        int eval = minimax(root->children[i], depth - 1, !isMaximizingPlayer, INT_MIN, INT_MAX);

        // Maximisation
        if (isMaximizingPlayer && eval > bestValue) {
            bestValue = eval;
            bestMove = root->children[i];
        }

        // Minimisation
        if (!isMaximizingPlayer && eval < bestValue) {
            bestValue = eval;
            bestMove = root->children[i];
        }
    }

    // Applique le meilleur mouvement trouvé
    if (bestMove != NULL) {
        // Copie le meilleur état trouvé dans l'état courant
        memcpy(&root->state, &bestMove->state, sizeof(Board));

        // Trouve le mouvement qui a conduit à cet état
        for (int x = 0; x < BOARD_SIZE; x++) {
            for (int y = 0; y < BOARD_SIZE; y++) {
                if (root->state.cells[x][y] != bestMove->state.cells[x][y]) {
                    *moveX = x;
                    *moveY = y;
                    return;
                }
            }
        }
    } else {
        printf("Aucun mouvement valide disponible pour l'IA.\n");
    }
}

