#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SDL2/SDL.h>
#include <limits.h>
#include <stdbool.h>
#include "game.h"
#include "board.h"
#include "digraph.h"
#include "../utils.h"

// Définition des tailles
#define CASE_TAILLE 60
#define BOARD_SIZE 8

SDL_Renderer* renderer;
int plateau[8][8];  // Le plateau de jeu (8x8)
int case_selectionnee[2] = {-1, -1};  // Sélection de la case pour le joueur
char currentPlayer = 1;  // Joueur humain (1) ou IA (2)


typedef struct {
    int cells[8][8];  // Représente l'état du plateau, ici un tableau 8x8
    int player;       // 0 pour le joueur 1, 1 pour le joueur 2
} Etat;  // L'état du plateau

typedef struct Node {
    Etat state;           // L'état du plateau dans ce nœud
    struct Node* parent;  // Le nœud parent
    struct Node* children;  // Les enfants (successeurs)
    int num_children;      // Nombre d'enfants
} Node;





Node* create_node_from_board(int plateau[8][8], int joueur_actuel) {
    Node* new_node = (Node*)malloc(sizeof(Node));
    if (new_node == NULL) {
        printf("Erreur de mémoire lors de la création du nœud.\n");
        return NULL;  // Si la mémoire ne peut pas être allouée, on retourne NULL
    }

    // Copie l'état du plateau dans le nœud
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            new_node->state.cells[i][j] = plateau[i][j];
        }
    }

    // Assigner le joueur actuel
    new_node->state.player = joueur_actuel;

    // Initialisation des autres attributs du nœud
    new_node->parent = NULL;
    new_node->children = NULL;
    new_node->num_children = 0;

    return new_node;  // Retourne le nœud nouvellement créé
}


// Afficher le plateau
void afficher_plateau() {
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);

    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if ((i + j) % 2 == 0) {
                SDL_SetRenderDrawColor(renderer, 240, 217, 181, 255);
            } else {
                SDL_SetRenderDrawColor(renderer, 181, 136, 99, 255);
            }

            SDL_Rect rect = { j * CASE_TAILLE, i * CASE_TAILLE, CASE_TAILLE, CASE_TAILLE };
            SDL_RenderFillRect(renderer, &rect);

            if (plateau[i][j] == 1) {
                SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);  // Pion noir
            } else if (plateau[i][j] == 2) {
                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);  // Pion blanc
            } else if (plateau[i][j] == 3) {
                SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);  // Dame noire
                SDL_Rect cercle = { j * CASE_TAILLE + CASE_TAILLE / 8, i * CASE_TAILLE + CASE_TAILLE / 8, CASE_TAILLE * 3 / 4, CASE_TAILLE * 3 / 4 };
                SDL_RenderDrawRect(renderer, &cercle);
            } else if (plateau[i][j] == 4) {
                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);  // Dame blanche
                SDL_Rect cercle = { j * CASE_TAILLE + CASE_TAILLE / 8, i * CASE_TAILLE + CASE_TAILLE / 8, CASE_TAILLE * 3 / 4, CASE_TAILLE * 3 / 4 };
                SDL_RenderDrawRect(renderer, &cercle);
            }

            if (plateau[i][j] != 0 && plateau[i][j] < 3) {
                SDL_Rect piece = { j * CASE_TAILLE + CASE_TAILLE / 4, i * CASE_TAILLE + CASE_TAILLE / 4, CASE_TAILLE / 2, CASE_TAILLE / 2 };
                SDL_RenderFillRect(renderer, &piece);
            }
        }
    }

    if (case_selectionnee[0] != -1 && case_selectionnee[1] != -1) {
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 128);
        SDL_Rect rect = { case_selectionnee[1] * CASE_TAILLE, case_selectionnee[0] * CASE_TAILLE, CASE_TAILLE, CASE_TAILLE };
        SDL_RenderFillRect(renderer, &rect);
    }
    SDL_RenderPresent(renderer);
}

// Vérification du mouvement
int mouvement_valide(int x1, int y1, int x2, int y2) {
    if (x2 < 0 || x2 >= 8 || y2 < 0 || y2 >= 8) return 0;
    if (plateau[y2][x2] != 0) return 0;

    int piece = plateau[y1][x1];
    int dx = abs(x2 - x1);
    int dy = abs(y2 - y1);

    if (piece == 3 || piece == 4) {
        if (dx == dy) {
            int step_x = (x2 > x1) ? 1 : -1;
            int step_y = (y2 > y1) ? 1 : -1;

            int captures = 0;
            for (int i = 1; i < dx; i++) {
                int cx = x1 + i * step_x;
                int cy = y1 + i * step_y;

                if (plateau[cy][cx] != 0) {
                    if ((piece == 3 && (plateau[cy][cx] == 2 || plateau[cy][cx] == 4)) || 
                        (piece == 4 && (plateau[cy][cx] == 1 || plateau[cy][cx] == 3))) {
                        captures++;
                    } else {
                        return 0;
                    }
                }
            }
            return (captures == 0) ? 1 : 2;
        }
    }

    if (piece == 1 || piece == 2) {
        int direction = (piece == 1) ? 1 : -1;

        if (dx == 1 && dy == 1 && y2 == y1 + direction) {
            return 1;
        }
        if (dx == 2 && dy == 2 && y2 == y1 + 2 * direction) {
            int middle_x = (x1 + x2) / 2;
            int middle_y = (y1 + y2) / 2;
            if ((piece == 1 && (plateau[middle_y][middle_x] == 2 || plateau[middle_y][middle_x] == 4)) || 
                (piece == 2 && (plateau[middle_y][middle_x] == 1 || plateau[middle_y][middle_x] == 3))) {
                return 2;
            }
        }
    }
    return 0;
}

// Effectuer le mouvement
void effectuer_mouvement(int x1, int y1, int x2, int y2, int type_mouvement) {
    if (type_mouvement == 2) {
        int middle_x = (x1 + x2) / 2;
        int middle_y = (y1 + y2) / 2;
        plateau[middle_y][middle_x] = 0;
    }
    plateau[y2][x2] = plateau[y1][x1];
    plateau[y1][x1] = 0;

    if (y2 == 0 && plateau[y2][x2] == 2) plateau[y2][x2] = 4;
    if (y2 == 7 && plateau[y2][x2] == 1) plateau[y2][x2] = 3;
}