#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h" // Inclusion pour les macros de couleur

void help_select() {
    printf("Aide pour SELECT : La commande SELECT vous permet de récupérer des données dans une table SQL.\n");
}

int main() {
    help_select(); // Appel de la fonction d'aide
    return 0; // Fin du programme avec succès
}