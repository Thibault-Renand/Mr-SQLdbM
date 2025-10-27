
#include "game.h"


int main() {
    system("clear");

    Board* state = initBoard();

    printf("Bienvenue au jeu du Morpion !\n");
    printBoard(state->cells);

    while (canPlay(state->cells) && !winner(state->cells)) {
        playTurn(state);
        printBoard(state->cells);
        state->currentPlayer = (state->currentPlayer == 'X') ? 'O' : 'X';
    }

    printf("La partie est terminée !\n");
    printWinner(*state);

    free(state);

    return 0;
}
