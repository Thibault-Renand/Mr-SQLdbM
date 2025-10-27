#include <SDL2/SDL.h>
#include <stdio.h>
#include <SDL2/SDL_ttf.h>
#include"utils.h"
#include"testdb.h"
#include "mode1.h"
#include <time.h>
#define maxlen 50

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

   
    if (pions_noirs == 0){
        
     return 2;
     }  // Blanc a gagné
    
    
    if (pions_blancs == 0){
        
        return 1;
    }  // Noir a gagné

   if (!mouvements_possibles(1)) return 2; // Noir bloqué, Blanc gagne
   if (!mouvements_possibles(2)) return 1; // Blanc bloqué, Noir gagne
   
    
    return 0;  // Partie continue
}


int mouvements_possibles(int joueur) {
    for (int y = 0; y < 8; y++) {
        for (int x = 0; x < 8; x++) {
            if ((joueur == 1 && (plateau[y][x] == 1 || plateau[y][x] == 3)) ||
                (joueur == 2 && (plateau[y][x] == 2 || plateau[y][x] == 4))) {
                
                // Tester toutes les directions pour chaque type de pièce
                int directions[4][2] = {
                    {1, 1}, {1, -1}, {-1, 1}, {-1, -1}
                };

                for (int d = 0; d < 4; d++) {
                    int nx = x + directions[d][0];
                    int ny = y + directions[d][1];

                    // Tester les mouvements simples
                    if (mouvement_valide(x, y, nx, ny) == 1) {
                        return 1;  // Mouvement possible
                    }

                    // Tester les captures
                    nx = x + 2 * directions[d][0];
                    ny = y + 2 * directions[d][1];
                    if (mouvement_valide(x, y, nx, ny) == 2) {
                        return 1;  // Capture possible
                    }
                }
            }
        }
    }

    return 0;  // Aucun mouvement possible
}




void afficher_textes(int player1_id, int player2_id) {
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
        // Les Noirs ont gagné
        sauvegarder_partie( player1_id, player2_id, player1_id);
        snprintf(texte, sizeof(texte), "Les Noirs ont gagné !");
        couleurTexte = (SDL_Color){255, 0, 0, 255}; // Texte rouge
        exit(1); // Arrêter le programme
    } else if (fin_partie == 2) {
        // Les Blancs ont gagné
        sauvegarder_partie( player1_id, player2_id, player2_id);
        snprintf(texte, sizeof(texte), "Les Blancs ont gagné !");
        couleurTexte = (SDL_Color){255, 0, 0, 255}; // Texte rouge
        exit(1); // Arrêter le programme
    } else {
        // Sinon afficher le tour actuel
        if (tour_joueur == 1) {
            snprintf(texte, sizeof(texte), "C'est le tour des Noirs");
        } else {
            snprintf(texte, sizeof(texte), "C'est le tour des Blancs");
        }
    }

    // Générer et afficher le texte
    SDL_Surface* surface = TTF_RenderText_Solid(font, texte, couleurTexte);
    if (surface == NULL) {
        printf("Erreur de création de la surface de texte: %s\n", TTF_GetError());
    } else {
        SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_Rect destRect = {LARGEUR_FENETRE - surface->w, HAUTEUR_FENETRE - surface->h, surface->w, surface->h};
        SDL_RenderCopy(renderer, texture, NULL, &destRect);
        SDL_DestroyTexture(texture);
        SDL_FreeSurface(surface);
    }

    TTF_CloseFont(font);
}



void sauvegarder_partie(int player1_id, int player2_id, int gagnant_id) {
    const char* filename = "ssuuuiiiiiiii";

    // Vérifier si le fichier existe déjà
    FILE* file = fopen(filename, "rb+");
    if (!file) {
        // Si le fichier n'existe pas, on le crée
        file = fopen(filename, "wb");
        if (!file) {
            printf("Erreur de création du fichier %s\n", filename);
            return;
        }
    }

    // Trouver un game_id unique
    GameRecord record;
    int game_id = 1;
    while (fread(&record, sizeof(GameRecord), 1, file) == 1) {
        if (record.game_id >= game_id) {
            game_id = record.game_id + 1;
        }
    }

    // Préparer le nouvel enregistrement
    GameRecord new_record;
    new_record.game_id = game_id;
    new_record.player1_id = player1_id;
    new_record.player2_id = player2_id;
    new_record.score_player1 = 0; // À mettre à jour selon votre logique
    new_record.score_player2 = 0; // À mettre à jour selon votre logique
    new_record.gagnant_id = gagnant_id;
    strcpy(new_record.jeux, "dames");

    // Ajouter la date actuelle
    time_t now = time(NULL);
    struct tm* t = localtime(&now);
    strftime(new_record.date, 50, "%Y-%m-%d %H:%M:%S", t);

    // Se déplacer à la fin pour écrire le nouvel enregistrement
    fseek(file, 0, SEEK_END);
    fwrite(&new_record, sizeof(GameRecord), 1, file);

    // Debug : Afficher les données sauvegardées
    printf("Sauvegarde réussie :\n");
    printf("Game ID: %d\n", new_record.game_id);
    printf("Player 1 ID: %d\n", new_record.player1_id);
    printf("Player 2 ID: %d\n", new_record.player2_id);
    printf("Score Player 1: %d\n", new_record.score_player1);
    printf("Score Player 2: %d\n", new_record.score_player2);
    printf("Date: %s\n", new_record.date);
    printf("Winner ID: %d\n", new_record.gagnant_id);
    printf("Game: %s\n", new_record.jeux);
    printf("----------------------\n");

    fclose(file);
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


void readBinaryFile() {
    FILE *file = fopen("ssuuuiiiiiiii", "rb"); // Ouvre le fichier en mode binaire
    if (file == NULL) {
        perror("Erreur d'ouverture du fichier");
        return;
    }

    GameRecord record; // Variable pour stocker une instance de la structure
    while (fread(&record, sizeof(GameRecord), 1, file) == 1) {
        // Affiche les données lues
         printf("Game ID: %d\n", record.game_id);
    printf("Player 1 ID: %d\n", record.player1_id);
    printf("Player 2 ID: %d\n", record.player2_id);
    printf("Score Player 1: %d\n", record.score_player1);
    printf("Score Player 2: %d\n", record.score_player2);
    printf("Date: %s\n", record.date);
    printf("Winner ID: %d\n", record.gagnant_id);
    printf("Game: %s\n", record.jeux);
    printf("----------------------\n");
    }

    fclose(file); // Ferme le fichier
}


void effectuer_mouvement(int x1, int y1, int x2, int y2, int type_mouvement) {
    // Gestion des captures
    if (type_mouvement == 2) {
        int milieu_x = (x1 + x2) / 2;
        int milieu_y = (y1 + y2) / 2;

        // Supprime la pièce uniquement si elle existe
        if ((plateau[milieu_y][milieu_x] == 1 || plateau[milieu_y][milieu_x] == 2 || 
             plateau[milieu_y][milieu_x] == 3 || plateau[milieu_y][milieu_x] == 4)) {
            plateau[milieu_y][milieu_x] = 0;
        }
    }

    // Déplacement de la pièce
    plateau[y2][x2] = plateau[y1][x1];
    plateau[y1][x1] = 0;

    // Promotion en dame
    if (y2 == 0 && plateau[y2][x2] == 2) plateau[y2][x2] = 4;
    if (y2 == 7 && plateau[y2][x2] == 1) plateau[y2][x2] = 3;
}


void afficher_plateau(int player1_id, int player2_id) {
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
    afficher_textes(player1_id, player2_id);
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

void afficherMenuPrincipal() {
    printf("===== Menu Principal =====\n");
    printf("1. SQL\n");
    printf("2. Gaming\n");
    printf("3. Quitter\n");
    printf("==========================\n");
}

// Fonction pour afficher le sous-menu Gaming
void afficherMenuGaming() {
    printf("===== Menu Gaming =====\n");
    printf("1. Dames\n");
    printf("2. Othello\n");
    printf("3. Démineur\n");
    printf("4. Morpion\n");
    printf("5. Retour au menu principal\n");
    printf("==========================\n");
}

// Fonction principale du menu
void menu() {
    int choixPrincipal;
    int choixGaming;

    do {
        afficherMenuPrincipal();
        printf("Votre choix: ");
        scanf("%d", &choixPrincipal);

        switch (choixPrincipal) {
            case 1:
                printf("Vous avez choisi SQL.\n");
                // Ajoute ici le code pour gérer les options SQL
                break;

            case 2:
                do {
                    afficherMenuGaming();
                    printf("Votre choix: ");
                    scanf("%d", &choixGaming);

                    switch (choixGaming) {
                        case 1:
                            printf("Vous avez choisi Dames.\n");
                            // Ajoute ici le code pour le jeu Dames
                            break;

                        case 2:
                            printf("Vous avez choisi Othello.\n");
                            // Ajoute ici le code pour le jeu Othello
                            break;

                        case 3:
                            printf("Vous avez choisi Démineur.\n");
                            // Ajoute ici le code pour le jeu Démineur
                            break;

                        case 4:
                            printf("Vous avez choisi Morpion.\n");
                            // Ajoute ici le code pour le jeu Morpion
                            break;

                        case 5:
                            printf("Retour au menu principal...\n");
                            break;

                        default:
                            printf("Choix invalide. Veuillez réessayer.\n");
                    }

                } while (choixGaming != 5);
                break;

            case 3:
                printf("Quitter le programme.\n");
                break;

            default:
                printf("Choix invalide. Veuillez réessayer.\n");
        }

    } while (choixPrincipal != 3);
}

