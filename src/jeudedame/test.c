#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 800
#define BOARD_SIZE 8
#define SQUARE_SIZE (WINDOW_WIDTH / BOARD_SIZE)

typedef enum { EMPTY, PLAYER, AI, PLAYER_KING, AI_KING } PieceType;

typedef struct {
    PieceType type;
} Piece;

Piece board[BOARD_SIZE][BOARD_SIZE];





void SDL_RenderFillCircle(SDL_Renderer* renderer, int x, int y, int radius) {
    int dx = radius, dy = 0, err = 0;
    
    while (dx >= dy) {
        SDL_RenderDrawPoint(renderer, x + dx, y + dy);
        SDL_RenderDrawPoint(renderer, x + dy, y + dx);
        SDL_RenderDrawPoint(renderer, x - dy, y + dx);
        SDL_RenderDrawPoint(renderer, x - dx, y + dy);
        SDL_RenderDrawPoint(renderer, x - dx, y - dy);
        SDL_RenderDrawPoint(renderer, x - dy, y - dx);
        SDL_RenderDrawPoint(renderer, x + dy, y - dx);
        SDL_RenderDrawPoint(renderer, x + dx, y - dy);
        
        if (err <= 0) {
            dy++;
            err += 2*dy + 1;
        }
        if (err > 0) {
            dx--;
            err -= 2*dx + 1;
        }
    }
}

void draw_board(SDL_Renderer* renderer) {
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            SDL_Rect rect = { j * SQUARE_SIZE, i * SQUARE_SIZE, SQUARE_SIZE, SQUARE_SIZE };
            if ((i + j) % 2 == 0) {
                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // Blanc
            } else {
                SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Noir
            }
            SDL_RenderFillRect(renderer, &rect);
            
            // Dessiner des pièces (exemple avec deux couleurs)
            if ((i + j) % 2 == 0) {
                SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // Rouge
                SDL_RenderFillCircle(renderer, j * SQUARE_SIZE + SQUARE_SIZE / 2, i * SQUARE_SIZE + SQUARE_SIZE / 2, SQUARE_SIZE / 3);
            } else {
                SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255); // Vert
                SDL_RenderFillCircle(renderer, j * SQUARE_SIZE + SQUARE_SIZE / 2, i * SQUARE_SIZE + SQUARE_SIZE / 2, SQUARE_SIZE / 3);
            }
        }
    }
}



// Fonction pour initialiser le plateau de jeu
void init_board() {
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            if ((i + j) % 2 == 1) {
                if (i < 3) {
                    board[i][j].type = PLAYER; // Joueur sur les trois premières lignes
                } else if (i > 4) {
                    board[i][j].type = AI; // IA sur les trois dernières lignes
                } else {
                    board[i][j].type = EMPTY;
                }
            } else {
                board[i][j].type = EMPTY;
            }
        }
    }
}

// Fonction pour dessiner le plateau
void draw_board(SDL_Renderer *renderer) {
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // Couleur des cases
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            if ((i + j) % 2 == 1) {
                SDL_Rect rect = { j * SQUARE_SIZE, i * SQUARE_SIZE, SQUARE_SIZE, SQUARE_SIZE };
                SDL_RenderFillRect(renderer, &rect);

                // Dessiner les pièces
                if (board[i][j].type == PLAYER || board[i][j].type == PLAYER_KING) {
                    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // Rouge pour le joueur
                    SDL_RenderFillCircle(renderer, j * SQUARE_SIZE + SQUARE_SIZE / 2, i * SQUARE_SIZE + SQUARE_SIZE / 2, SQUARE_SIZE / 3);
                } else if (board[i][j].type == AI || board[i][j].type == AI_KING) {
                    SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255); // Bleu pour l'IA
                    SDL_RenderFillCircle(renderer, j * SQUARE_SIZE + SQUARE_SIZE / 2, i * SQUARE_SIZE + SQUARE_SIZE / 2, SQUARE_SIZE / 3);
                }
            }
        }
    }
}

// Fonction pour dessiner un cercle (SDL n'a pas de fonction pour ça)
void SDL_RenderFillCircle(SDL_Renderer* renderer, int x, int y, int radius) {
    for (int w = 0; w < radius * 2; w++) {
        for (int h = 0; h < radius * 2; h++) {
            int dx = radius - w; 
            int dy = radius - h;
            if (dx * dx + dy * dy <= radius * radius) {
                SDL_RenderDrawPoint(renderer, x + dx, y + dy);
            }
        }
    }
}

// Fonction simple de mouvement aléatoire pour l'IA
void ai_move() {
    srand(time(NULL));
    int x1, y1, x2, y2;

    // Rechercher une case vide pour déplacer une pièce
    while (1) {
        x1 = rand() % BOARD_SIZE;
        y1 = rand() % BOARD_SIZE;
        x2 = rand() % BOARD_SIZE;
        y2 = rand() % BOARD_SIZE;

        if (board[y1][x1].type == AI && board[y2][x2].type == EMPTY && abs(x2 - x1) == 1 && abs(y2 - y1) == 1) {
            board[y1][x1].type = EMPTY;
            board[y2][x2].type = AI;
            break;
        }
    }
}

// Fonction pour gérer les événements SDL
int handle_events(SDL_Event *event) {
    if (event->type == SDL_QUIT) {
        return 1;
    }
    return 0;
}

int main() {
    // Initialisation de SDL
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        printf("SDL_Init Error: %s\n", SDL_GetError());
        return 1;
    }

    // Création de la fenêtre
    SDL_Window *window = SDL_CreateWindow("Jeu de Dames",
                                          SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                                          WINDOW_WIDTH, WINDOW_HEIGHT,
                                          SDL_WINDOW_SHOWN);
    if (window == NULL) {
        printf("SDL_CreateWindow Error: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    // Création du renderer
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL) {
        printf("SDL_CreateRenderer Error: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    // Initialiser le plateau de jeu
    init_board();

    // Boucle principale
    int quit = 0;
    SDL_Event event;
    while (!quit) {
        while (SDL_PollEvent(&event)) {
            quit = handle_events(&event);
        }

        // Dessiner le plateau de jeu
        draw_board(renderer);

        // Mouvement de l'IA
        ai_move();

        // Afficher les changements à l'écran
        SDL_RenderPresent(renderer);

        // Attente avant le prochain mouvement
        SDL_Delay(500);
    }

    // Libérer les ressources
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
