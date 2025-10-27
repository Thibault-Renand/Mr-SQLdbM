# Détails d'implémentation :

## Jeu
### Plateau
   - `initBoard()` : Initialise le plateau d'une partie Othello.
   - `printBoard()` : Affiche le plateau d'une partie.
   - `isInBounds()` : Vérifie qu'un mouvement soit dans le plateau.
   - `checkDirection()` : Vérifie si une case adjacente d'une position est disponible.
   - `isValidMove()` : Vérifie qu'un mouvement soit valide selon les [règles du jeu](https://www.ffothello.org/othello/regles-du-jeu/).
### Partie
   - `playTurn()` : Joue un tour **pour un joueur**.
   - `canPlay()` : Vérifie si un joueur peut jouer son tour.
   - `isGameOver()` : Vérifie si la partie est terminée.
   - `calculateScore()` : Calcule le nombre de pion sur le plateau pour un joueur. Cette fonction est également utilisée comme heuristique pour l'ordinateur lorsqu'il cherche à maximiser uniquement ses points.
   - `printWinner()` : Affiche le résultat de la partie.
   - `flipDirection()` : Retourne tous les pions à retourner après le tour d'un joueur.
   - `makeMove()` : Effectue le mouvement d'un joueur.

## Ordinateur
### Composants
   1. Structure `Board` :
      - Représente l’état du plateau (les positions des pièces et le joueur actif).
   2. Structure `TreeNode` :
      - Contient l’état du jeu (`state`), la valeur du nœud pour MinMax (value), et une liste dynamique des enfants (`children`).
   3. Fonctionnalités :
      - `create_node` : Crée un nœud à partir d’un état de plateau.
      - `add_child` : Ajoute un enfant au nœud parent, en ajustant dynamiquement la taille du tableau des enfants.
      - `free_tree` : Libère récursivement la mémoire de l’arbre.
### Fonctionnement
   1. Fonctions d'évaluation :
      - `nbCornBoard()` : Renvoie une évaluation des positions du joueur courant. Elle calcule à quel point le joueur "contrôle" la partie. On peut utiliser cette fonction pour voir qui contrôle le plus de meilleurs emplacements sur un état de partie donné.
      - `nbPosMov()` : Renvoie le nombre de placements possibles pour un état donné.
      - `maxCapture()` : Renvoie le nombre maximal de pions capturable par un joueur pour un état donné. Le placement idéal est récupéré à l'aide d'un pointeur. Pas encore écrite.
   2. Algorithme :

      L'idée derrière est d'utiliser des heuristiques intéressantes pour améliorer la qualité du mouvement joué par l'ordinateur, tout en conservant un temps de calcul raisonnable.
      Tout en faisant attention aux conditions d'arrêt d'une partie de Othello, on construit l'algorithme alphabêta de manière suivante.

      La valeur 𝛼 servira de borne inférieure au score en cours de calcul, et 𝛽 de borne supérieure. Pour l’appel initial, on prendra 𝛼 = −∞ et 𝛽 = +∞.

      On rappelle qu'un état est constitué d'un plateau et du joueur courant. La structure se trouve dans `digraph.h`.
      𝐺 est le graphe des états construit par la fonction `generatePossibleMoves()`. Il y a autant d'état créé que de mouvements possibles, mais tous ne sont pas exploré compte tenu de la construction de l'algorithme.
      
      On n'utilisera pas de fonction de score 𝑔 car j'ai jugé inutile d'en user ici. De plus, l'état courant 𝑠 est directement récupéré du graphe des états, donc il n'est pas passé en argument dans l'implémentation. Idem pour l'heuristique. Je les ai laissés dans le pseudocode pour bien insister sur le fait qu'on utilise quelque chose d'extérieur à la fonction. L'implémentation rajoute également un indicateur `bool isMaximizingPlayer` qui détermine si c'est le tour du joueur maximisant (ordinateur) ou minimisant (joueur).
      
      Cette fonction renvoie une estimation du meilleur score possible pour l'ordinateur à partir de l'état 𝑠, en n'explorant pas plus loin que la profondeur 𝑑.

      Elle est utilisée ensuite pour jouer le "meilleur" coup par la fonction `playBestMove()`.

      ```pseudocode
         1: fonction AlphaBeta(𝐺 = (𝑆1 ⊔ 𝑆2, 𝐴), ℎ, 𝑠, 𝑑, 𝛼, 𝛽) :
         2:
         3:    si 𝑠 est terminal ou d = 0 alors :
         4:       renvoyer ℎ(𝑠)
         5:    sinon si 𝑠 ∈ 𝑆1 alors :
         6:       soit 𝑣 = −∞
         7:       pour chaque 𝑡 tel que (𝑠, 𝑡) ∈ 𝐴 :
         8:         𝑣 ← max(𝑣, AlphaBeta(𝐺, ℎ, 𝑡, 𝑑 − 1, 𝛼, 𝛽))
         9:         si 𝑣 ≥ 𝛽 alors : // Coupure beta
         10:            renvoyer v
         11:         𝛼 ← max(𝛼, 𝑣)
         12:         sinon :
         13:            soit 𝑣 = +∞
         14:            pour chaque 𝑡 tel que (𝑠, 𝑡) ∈ 𝐴 :
         15:               𝑣 ← min(𝑣, AlphaBeta(𝐺, ℎ, 𝑡, 𝑑 − 1, 𝛼, 𝛽))
         16:               si 𝑣 ≤ 𝛼 alors : // Coupure alpha
         17:                  renvoyer v
         18:               𝛽 ← min(𝛽, 𝑣)
      ```
      Ps : un *état terminal* est une fin de partie.

   4. Améliorations possibles :
      - Heuristiques
      - Structure de données : listes chaînées au lieu de tableaux dynamiques (?)
      - Mémoire : stocker les changements états au lieu de copier l'ensemble du tableau

---

- Compilation avec `make`. Supprimer tout avec `make clean`.
- Version : `gcc (Ubuntu 11.4.0-1ubuntu1~22.04) 11.4.0`
- Format du fichier `othello` : `ELF 64-bit LSB pie executable, x86-64, version 1 (SYSV), dynamically linked, interpreter /lib64/ld-linux-x86-64.so.2, BuildID[sha1]=ded6dec10dc22622ef8fc49f531d35da245c23fb, for GNU/Linux 3.2.0, not stripped`

Bon jeu :D
