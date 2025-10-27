#include <SDL2/SDL.h>
#include <stdio.h>
#include <SDL2/SDL_ttf.h>
#include"utils.h"
#include "jddia.h"
#include "mode1.h"
#include "digraph.h"
#include "testdb.h"





int main() {
    menu();
 readBinaryFile();

       users player1, player2,bin;
    //saisieuser("./logsinscriptions.dat",bin);
    // Connexion du premier joueur
    printf("Joueur 1, veuillez vous connecter :\n");
    saisieuserlogins(&player1);
    if (!connectionuser(&player1, "./logsinscriptions.dat")) {
        printf("Échec de la connexion pour Joueur 1.\n");
        return 1;
    }

    // Connexion du deuxième joueur (si nécessaire)
    printf("Joueur 2, veuillez vous connecter :\n");
    saisieuserlogins(&player2);
    if (!connectionuser(&player2, "./logsinscriptions.dat")) {
        printf("Échec de la connexion pour Joueur 2.\n");
        return 1;
    }


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

            afficher_plateau(player1.uid,player2.uid);
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

