#ifndef MODE1_H
#define MODE1_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#define LARGEUR_FENETRE 800
#define HAUTEUR_FENETRE 800
#define CASE_TAILLE 100

// Déclaration de variables globales
extern SDL_Window* window;
extern SDL_Renderer* renderer;
extern int plateau[8][8];  // Plateau de jeu (8x8)
extern int case_selectionnee[2];  // Case sélectionnée par le joueur
extern int tour_joueur;  // 1 pour les noirs, 2 pour les blancs


void menu();
void afficherMenuPrincipal();
void afficherMenuGaming();
int vérifier_fin_partie();
void sauvegarder_partie( int player1_id, int player2_id, int gagnant_id);
// Fonctions principales
int mouvements_possibles(int joueur);
void afficher_textes(int player1_id, int player2_id);
void initialiser_plateau();
int mouvement_valide(int x1, int y1, int x2, int y2);
void effectuer_mouvement(int x1, int y1, int x2, int y2, int type_mouvement);
void afficher_plateau(int player1_id,int player2_id);
int choix(SDL_Window *window, SDL_Renderer *renderer);
void readBinaryFile();

// Fonction de vérification de la fin de partie

#endif
