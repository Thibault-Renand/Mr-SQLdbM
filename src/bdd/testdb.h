#ifndef TESTDB_H
#define TESTDB_H
#include <stdio.h>
#include <stdlib.h>
#include<string.h>
#define maxlen 50
typedef struct {
    int uid;
    char username[maxlen]; // Taille fixe pour le nom d'utilisateur
    char mdp[maxlen];    // Taille fixe pour le mot de passe
    int parties_jouees;
    int victoire;
    int defaites;
} users;
typedef struct {
    int game_id;          // Unique id pour chaque partie
    int player1_id;      // ID P1
    int player2_id;      // ID P2
    int score_player1;   // Score P1
    int score_player2;   // Score p2
    char date[maxlen];       // Date de la partie
    int gagnant_id;
    char jeux[maxlen];
} GameRecord;
void vider_buffer();
int uidfinder(const char *filename);
int saisieuser(const char *filename, users user);
void afficherUtilisateurs(const char *filename);
int connectionuser(users *user, const char *filename);
int add_game_record(const char *filename, GameRecord *record);
int saisieuserlogins(users *user);
#endif
