# readme_fonctionsdb.md

## Introduction
Ce projet fournit un ensemble de fonctions pour la gestion des utilisateurs et l'enregistrement des parties d'un jeu. Le code est écrit en C et contient des fonctionnalités telles que la création d'utilisateurs, la connexion, l'affichage des utilisateurs, et l'ajout d'enregistrements de parties.

---

## Structures
### `users`
Représente un utilisateur avec les champs suivants :
- **uid** : Identifiant unique de l'utilisateur.
- **username** : Nom d'utilisateur (jusqu'à 50 caractères).
- **mdp** : Mot de passe (jusqu'à 50 caractères).
- **parties_jouees** : Nombre total de parties jouées.
- **victoire** : Nombre de victoires.
- **defaites** : Nombre de défaites.

### `GameRecord`
Représente un enregistrement de partie avec les champs suivants :
- **game_id** : Identifiant unique de la partie.
- **player1_id** : ID du joueur 1.
- **player2_id** : ID du joueur 2.
- **score_player1** : Score du joueur 1.
- **score_player2** : Score du joueur 2.
- **date** : Date de la partie (jusqu'à 20 caractères).

---

## Fonctionnalités Principales

### 1. `uidfinder`
**Description** : Recherche et retourne le prochain identifiant utilisateur unique disponible.

**Prototype** :
```c
int uidfinder(const char* filename);
```

**Usage** :
- **Paramètres** :
  - `filename` : Nom du fichier contenant les utilisateurs.
- **Retour** : Le prochain UID unique.

---

### 2. `saisieuser`
**Description** : Permet la création d'un nouvel utilisateur.

**Prototype** :
```c
int saisieuser(const char* filename, users user);
```

**Usage** :
- **Paramètres** :
  - `filename` : Nom du fichier contenant les utilisateurs.
  - `user` : Structure utilisateur à sauvegarder.
- **Retour** : 1 si la sauvegarde est réussie, 0 sinon.

---

### 3. `afficherUtilisateurs`
**Description** : Affiche tous les utilisateurs enregistrés.

**Prototype** :
```c
void afficherUtilisateurs(const char* filename);
```

**Usage** :
- **Paramètres** :
  - `filename` : Nom du fichier contenant les utilisateurs.

---

### 4. `connectionuser`
**Description** : Permet à un utilisateur de se connecter.

**Prototype** :
```c
int connectionuser(users user, const char* filename);
```

**Usage** :
- **Paramètres** :
  - `user` : Structure utilisateur contenant les identifiants.
  - `filename` : Nom du fichier contenant les utilisateurs.
- **Retour** : 1 si la connexion est réussie, 0 sinon.

---

### 5. `add_game_record`
**Description** : Ajoute un enregistrement de partie au fichier.

**Prototype** :
```c
int add_game_record(const char* filename, GameRecord* record);
```

**Usage** :
- **Paramètres** :
  - `filename` : Nom du fichier contenant les enregistrements de parties.
  - `record` : Pointeur vers la structure d'enregistrement de partie.
- **Retour** : 0 si l'opération réussit, -1 sinon.

---

## Exemple d'Utilisation
```c
#include <stdio.h>
#include "functions.h"

int main(void) {
    users user;
    saisieuser("logsinscriptions.dat", user);
    afficherUtilisateurs("logsinscriptions.dat");
    return 0;
}
```

---

## Fichiers Associés
- `logsinscriptions.dat` : Fichier binaire contenant les informations des utilisateurs.
- `gamerecords.dat` : Fichier binaire contenant les enregistrements des parties.

---

## Notes
- Les noms d'utilisateur doivent être uniques.
- Les fichiers sont ouverts en mode binaire pour garantir une compatibilité multiplateforme.
- Les champs des structures sont limités en taille pour éviter les débordements de mémoire.

## Compilation et Exécution
1. Compiler avec `gcc` :
   ```bash
   gcc -o testdb testdb.c
   ```
2. Exécuter l'application :
   ```bash
   ./testdb
   ```
