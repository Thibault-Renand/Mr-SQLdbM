#include <SDL2/SDL.h>
#include <stdio.h>
#include <SDL2/SDL_ttf.h>
#include"utils.h"

#define LARGEUR_FENETRE 800
#define HAUTEUR_FENETRE 800
#define CASE_TAILLE 100

// Déclaration de variables globales
SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;
int plateau[8][8];  // Plateau de jeu (8x8)
int case_selectionnee[2] = {-1, -1};  // Case sélectionnée par le joueur
int tour_joueur = 1;  // 1 pour les noirs, 2 pour les blancs


int vérifier_fin_partie() {
    int pions_noirs = 0;
    int pions_blancs = 0;

    // Compter le nombre de pions restants pour chaque joueur
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if (plateau[i][j] == 1 || plateau[i][j] == 3) pions_noirs++;
            if (plateau[i][j] == 2 || plateau[i][j] == 4) pions_blancs++;
        }
    }

    // Si un joueur n'a plus de pions, la partie est terminée
    if (pions_noirs == 0) return 2;  // Blanc a gagné
    if (pions_blancs == 0) return 1;  // Noir a gagné

    return 0;  // Partie continue
}




void afficher_textes() {
    SDL_Color couleurTexte = {255, 255, 255, 255};  // Texte blanc
    TTF_Font* font = TTF_OpenFont("arial.ttf", 24);
    if (font == NULL) {
        printf("Erreur de chargement de la police : %s\n", TTF_GetError());
        return;
    }

    char texte[50];

    // Vérifier si la partie est terminée
    int fin_partie = vérifier_fin_partie();
    if (fin_partie == 1) {
        snprintf(texte, sizeof(texte), "Les Noirs ont gagné !");
        SDL_Color couleurTexte = {255, 0, 0, 255};  // Rouge
    } else if (fin_partie == 2) {
        snprintf(texte, sizeof(texte), "Les Blancs ont gagné !");
       SDL_Color couleurTexte = {255, 0, 0, 255};  // Rouge
    } else {
        // Sinon afficher le tour actuel
        if (tour_joueur == 1) {
            snprintf(texte, sizeof(texte), "C'est le tour des Noirs");
        } else {
            snprintf(texte, sizeof(texte), "C'est le tour des Blancs");
        }
    }

    SDL_Surface* surface = TTF_RenderText_Solid(font, texte, couleurTexte);
    if (surface == NULL) {
        printf("Erreur de création de la surface de texte: %s\n", TTF_GetError());
    } else {
        SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_Rect destRect = {LARGEUR_FENETRE  - surface->w , HAUTEUR_FENETRE- surface->h, surface->w, surface->h};
        SDL_RenderCopy(renderer, texture, NULL, &destRect);
        SDL_DestroyTexture(texture);
        SDL_FreeSurface(surface);
    }

    TTF_CloseFont(font);
}




void initialiser_plateau() {
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if (i < 3 && (i + j) % 2 == 1) {
                plateau[i][j] = 1;  // Pièces noires
            } else if (i >= 5 && (i + j) % 2 == 1) {
                plateau[i][j] = 2;  // Pièces blanches
            } else {
                plateau[i][j] = 0;  // Case vide
            }
        }
    }
}

int mouvement_valide(int x1, int y1, int x2, int y2) {
    // Vérifier les limites du plateau
    if (x2 < 0 || x2 >= 8 || y2 < 0 || y2 >= 8) return 0;

    // Vérifier si la case d'arrivée est vide
    if (plateau[y2][x2] != 0) return 0;

    int piece = plateau[y1][x1];
    int dx = abs(x2 - x1);
    int dy = abs(y2 - y1);

    // Mouvement des dames
    if (piece == 3 || piece == 4) {
        if (dx == dy) {  // Mouvement sur une diagonale
            int step_x = (x2 > x1) ? 1 : -1;
            int step_y = (y2 > y1) ? 1 : -1;

            int captures = 0;
            int last_capture_x = -1, last_capture_y = -1;

            // Vérifier toutes les cases intermédiaires
            for (int i = 1; i < dx; i++) {
                int cx = x1 + i * step_x;
                int cy = y1 + i * step_y;

                if (plateau[cy][cx] != 0) {
                    if ((piece == 3 && (plateau[cy][cx] == 2 || plateau[cy][cx] == 4)) || 
                        (piece == 4 && (plateau[cy][cx] == 1 || plateau[cy][cx] == 3))) {
                        captures++;
                        last_capture_x = cx;
                        last_capture_y = cy;
                    } else {
                        return 0;  // Blocage ou pièce alliée
                    }
                }
            }

            // Valider le mouvement
            if (captures == 1) {
                // Supprimer la pièce capturée
                plateau[last_capture_y][last_capture_x] = 0;
                return 2;  // Capture valide
            }

            return (captures == 0) ? 1 : 0;  // Mouvement normal ou invalide
        }
    }

    if (piece == 1 || piece == 2) {
        int direction = (piece == 1) ? 1 : -1;

        // Déplacement simple en diagonale
        if (dx == 1 && dy == 1 && y2 == y1 + direction) {
            return 1;
        }

        // Capture
        if (dx == 2 && dy == 2 && y2 == y1 + 2 * direction) {
            int milieu_x = (x1 + x2) / 2;
            int milieu_y = (y1 + y2) / 2;

            // Vérifier si la pièce au milieu est ennemie (pion ou dame)
            if ((piece == 1 && (plateau[milieu_y][milieu_x] == 2 || plateau[milieu_y][milieu_x] == 4)) ||
                (piece == 2 && (plateau[milieu_y][milieu_x] == 1 || plateau[milieu_y][milieu_x] == 3))) {
                // Supprimer la pièce capturée
                plateau[milieu_y][milieu_x] = 0;
                return 2;  // Capture
            }
        }
    }

    return 0;  // Mouvement invalide
}




void effectuer_mouvement(int x1, int y1, int x2, int y2, int type_mouvement) {
    if (type_mouvement == 2) {
        int milieu_x = (x1 + x2) / 2;
        int milieu_y = (y1 + y2) / 2;
        plateau[milieu_y][milieu_x] = 0;  // Retirer la pièce capturée
    }

    // Déplacer la pièce
    plateau[y2][x2] = plateau[y1][x1];
    plateau[y1][x1] = 0;

    // Promotion en dame
    if (y2 == 0 && plateau[y2][x2] == 2) plateau[y2][x2] = 4;
    if (y2 == 7 && plateau[y2][x2] == 1) plateau[y2][x2] = 3;
}

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
                SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            } else if (plateau[i][j] == 2) {
                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
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
    afficher_textes();
    SDL_RenderPresent(renderer);
}


int choix(SDL_Window *window, SDL_Renderer *renderer) {
    SDL_Event event;
    int choixUtilisateur = 0;
    int mouseX, mouseY;
    SDL_Rect choix1 = {100, 100, 200, 50};  // Position et taille du bouton Choix 1
    SDL_Rect choix2 = {100, 200, 200, 50};  // Position et taille du bouton Choix 2

    // Charger la police
    TTF_Font* font = TTF_OpenFont("arial.ttf", 24);
    if (font == NULL) {
        printf("Erreur de chargement de la police : %s\n", TTF_GetError());
        return -1;
    }

    // Couleur du texte
    SDL_Color couleurTexte = {255, 255, 255, 255};  // Blanc

    // Boucle pour gérer les événements
    while (!choixUtilisateur) {
        // Gestion des événements
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                choixUtilisateur = -1;  // Quitter si l'utilisateur ferme la fenêtre
            } else if (event.type == SDL_MOUSEBUTTONDOWN) {
                // Obtenir la position de la souris
                SDL_GetMouseState(&mouseX, &mouseY);

                // Vérification si le clic est dans le bouton Choix 1
                if (mouseX >= choix1.x && mouseX <= choix1.x + choix1.w &&
                    mouseY >= choix1.y && mouseY <= choix1.y + choix1.h) {
                    choixUtilisateur = 1;  // Choix 1
                }

                // Vérification si le clic est dans le bouton Choix 2
                if (mouseX >= choix2.x && mouseX <= choix2.x + choix2.w &&
                    mouseY >= choix2.y && mouseY <= choix2.y + choix2.h) {
                    choixUtilisateur = 2;  // Choix 2
                }
            }
        }

        // Effacement de l'écran
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);  // Blanc
        SDL_RenderClear(renderer);

        // Dessiner Choix 1
        SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);  // Bleu
        SDL_RenderFillRect(renderer, &choix1);

        // Dessiner Choix 2
        SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);  // Vert
        SDL_RenderFillRect(renderer, &choix2);

        // Afficher le texte pour Choix 1
        SDL_Surface* surfaceChoix1 = TTF_RenderText_Solid(font, "Choix 1", couleurTexte);
        SDL_Texture* textureChoix1 = SDL_CreateTextureFromSurface(renderer, surfaceChoix1);
        SDL_Rect destRectChoix1 = {choix1.x + 50, choix1.y + 10, surfaceChoix1->w, surfaceChoix1->h};
        SDL_RenderCopy(renderer, textureChoix1, NULL, &destRectChoix1);
        SDL_DestroyTexture(textureChoix1);
        SDL_FreeSurface(surfaceChoix1);

        // Afficher le texte pour Choix 2
        SDL_Surface* surfaceChoix2 = TTF_RenderText_Solid(font, "Choix 2", couleurTexte);
        SDL_Texture* textureChoix2 = SDL_CreateTextureFromSurface(renderer, surfaceChoix2);
        SDL_Rect destRectChoix2 = {choix2.x + 50, choix2.y + 10, surfaceChoix2->w, surfaceChoix2->h};
        SDL_RenderCopy(renderer, textureChoix2, NULL, &destRectChoix2);
        SDL_DestroyTexture(textureChoix2);
        SDL_FreeSurface(surfaceChoix2);

        // Actualiser l'écran
        SDL_RenderPresent(renderer);
    }

    // Fermer la police
    TTF_CloseFont(font);

    return choixUtilisateur;  // Retourne le choix effectué
}


int main() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("Erreur d'initialisation de SDL: %s\n", SDL_GetError());
        return -1;
    }
    if (TTF_Init() == -1) {
        printf("Erreur d'initialisation de SDL_ttf: %s\n", TTF_GetError());
        return -1;
    }

    window = SDL_CreateWindow("Jeu de Dames", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, LARGEUR_FENETRE, HAUTEUR_FENETRE, SDL_WINDOW_SHOWN);
    if (!window) {
        printf("Erreur de création de la fenêtre: %s\n", SDL_GetError());
        return -1;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        printf("Erreur de création du renderer: %s\n", SDL_GetError());
        return -1;
    }

 int choixUtilisateur = choix(window, renderer);


    // Afficher un menu pour le choix du joueur


    if (choixUtilisateur == 1) {
        initialiser_plateau();
        printf("La partie commence !\n");

        int gameRunning = 1;
        SDL_Event event;
        int x, y;
        while (gameRunning) {
            while (SDL_PollEvent(&event)) {
                if (event.type == SDL_QUIT) {
                    gameRunning = 0;
                }

                if (event.type == SDL_MOUSEBUTTONDOWN) {
                    if (event.button.button == SDL_BUTTON_LEFT) {
                        x = event.button.x / CASE_TAILLE;
                        y = event.button.y / CASE_TAILLE;
                        printf("Clique sur la case (%d, %d)\n", x, y);

                        if (case_selectionnee[0] == -1 && case_selectionnee[1] == -1) {
                            // Vérifiez que la pièce sélectionnée appartient au joueur dont c'est le tour
                            if ((plateau[y][x] == 1 && tour_joueur == 1) || (plateau[y][x] == 2 && tour_joueur == 2) || 
                                (plateau[y][x] == 3 && tour_joueur == 1) || (plateau[y][x] == 4 && tour_joueur == 2)) {
                                case_selectionnee[0] = y;
                                case_selectionnee[1] = x;
                            }
                        } else {
                            int type_mouvement = mouvement_valide(case_selectionnee[1], case_selectionnee[0], x, y);
                            if (type_mouvement != 0) {
                                effectuer_mouvement(case_selectionnee[1], case_selectionnee[0], x, y, type_mouvement);
                                tour_joueur = (tour_joueur == 1) ? 2 : 1;  // Changer de joueur
                            }
                            case_selectionnee[0] = -1;  // Réinitialiser la sélection
                            case_selectionnee[1] = -1;
                        }
                    }
                }
            }

            afficher_plateau();
        }
    } else if (choixUtilisateur == 2) {
        printf("Le choix est à 2: La partie est annulée.\n");
    } else {
        printf("Choix invalide.\n");
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();

    return 0;
}

