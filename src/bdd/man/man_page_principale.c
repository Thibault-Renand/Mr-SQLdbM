#include "manpage.h"


const char *manual[] = {
    "TITRE",
    "",
    "    Manuel d'aide pour newSQL.",
    "",
    "SYNOPSIS",
    "",
    "    HELP [options]",
    "",
    "DESCRIPTION",
    "",
    "    Bienvenue dans la page d'aide pour le projet newSQL !",
    "    Cette page contient les toutes les commandes disponibles pour l'application, ainsi qu'une brève description des commandes.",
    "",
    "OPTIONS",
    "    Il est bon de noter que HELP est une commande.",
    "",
    "    HELP                   Affiche cette page d'aide",
    "    HELP [commande]        Affiche la page d'aide correspondante pour la commande",
    "",
    "COMMANDES",
    "",
    "    ALTER TABLE     Modifie une table existante. Idéal pour ajouter une colonne, supprimer une colonne ou modifier une",
    "                    colonne existante, par exemple pour changer le type.",
    "",
    "    CREATE TABLE    Crée une table en newSQL.",
    "",
    "    DROP TABLE      Supprime définitivement une table d'une base de données. Cela supprime en même temps les éventuels",
    "                    index, trigger, contraintes et permissions associées à cette table.",
    "",
    "    DELETE          Supprime des lignes dans une table. En utilisant cette commande associé à WHERE il est possible de",
    "                    sélectionner les lignes concernées qui seront supprimées.",
    "",
    "    FROM            Spécifie dans quelle table chercher les données.",
    "",
    "    GROUP BY        Groupe plusieurs résultats et utiliser une fonction de totaux sur un groupe de résultat.",
    "",
    "    HAVING          Presque similaire à WHERE à la seule différence que HAVING permet de filtrer en utilisant des fonctions",
    "                    telles que COUNT(), AVG(), MIN() ou MAX().",
    "",
    "    LIMIT           Spécifie le nombre maximum de résultats que l'ont souhaite obtenir. Cette clause est souvent associé à un",
    "                    OFFSET.",
    "",
    "    OFFSET          Effectue un décalage sur le jeu de résultat.",
    "",
    "    ORDER BY        Trie les lignes dans un résultat d'une requête newSQL.",
    "",
    "    SELECT          Retourne des enregistrements dans un tableau de résultat. Cette commande peut sélectionner une ou plusieurs",
    "                    colonnes d'une table.",
    "",
    "    UPDATE          Effectue des modifications sur des lignes existantes. Très souvent cette commande est utilisée avec WHERE pour",
    "                    spécifier sur quelles lignes doivent porter la ou les modifications.",
    "",
    "    WHERE           Extraire les lignes d'une base de données qui respectent une condition. Cela permet d'obtenir uniquement les",
    "                    informations désirées.",
    "",
    "FONCTIONS",
    "",
    "    AVG()           Calcule une valeur moyenne sur un ensemble d'enregistrement de type numérique et non nul.",
    "",
    "    COUNT()         Compte le nombre d'enregistrement dans une table.",
    "",
    "    MAX()           Retourne la valeur maximale d'une colonne dans un set d'enregistrement. La fonction peut s'appliquée à des ",
    "                    données numériques ou alphanumériques.",
    "",
    "    MIN()           Retourne la plus petite valeur d'une colonne sélectionnée. Cette fonction s'applique aussi bien à des données",
    "                    numériques qu'à des données alphanumériques.",
    "",
    "USAGE",
    "",
    "    Utilisez les flèches haut/bas pour naviguer.",
    "    Quittez la page avec 'q'.",
    "",
    "AUTHEURS",
    "",
    "    Mathys RAGEADE, Thibault RENAND, Jolan VAN BOCKSTAELE & Mattéo BECASSE",
    "",
   NULL
};

const char* help_select[] = {
    "*** SELECT ***",
    "",
    " L'utilisation basique de cette commande s'effectue de la manière suivante :",
    "",
    "     SELECT nom_du_champ FROM nom_du_tableau",
    "",
    " Cette requête SQL va sélectionner (SELECT) le champ \"nom_du_champ\" provenant (FROM) du tableau appelé \"nom_du_tableau\".",
    "",
    " Avec une table client il est possible de lire plusieurs colonnes à la fois. Il suffit tout simplement de séparer les noms",
    " des champs souhaités par une virgule. Pour obtenir les prénoms et les noms des clients il faut alors faire la requête suivante:",
    "",
    "     SELECT prenom, nom FROM client",
    "",
    " On peut aussi récupérer toutes les informations de la table avec *:",
    "",
    "     SELECT * FROM nom_du_tableau",
    "",
    NULL
};

const char* help_from[] = {
    "*** FROM ***",
    "",
    " La commande FROM est le GPS de ta requête SQL. Elle indique d'où tu veux tirer tes données.",
    "",
    "     SELECT nom_du_champ FROM nom_du_tableau",
    "",
    NULL
};

const char* help_where[] = {
    "*** WHERE ***",
    "",
    "DESCRIPTION",
    "",
    "     La commande WHERE s'utilise en complément à une requête utilisant SELECT.",
    "",
    "          SELECT nom_colonnes FROM nom_table WHERE condition",
    "",
    "OPERATEURS DE COMPARAISON",
    "",
    "     Il existe plusieurs opérateurs de comparaisons. La liste ci-jointe présente quelques uns des opérateurs les plus couramment",
    "     utilisés.",
    "",
    "     ****************************************************************************************************",
    "     *  Opérateur    *  Description                                                                    *",
    "     ****************************************************************************************************",
    "     *  =            *  Egale                                                                           *",
    "     *  <>           *  Pas égale                                                                      *",
    "     *  !=           *  Pas égale                                                                      *",
    "     *  >            *  Supérieur à                                                                   *",
    "     *  <            *  Inférieur à                                                                   *",
    "     *  >=           *  Supérieur ou égale à                                                         *",
    "     *  <=           *  Inférieur ou égale à                                                         *",
    "     *  IN           *  Liste de plusieurs valeurs possibles                                            *",
    "     *  BETWEEN      *  Valeur comprise dans un intervalle donnée (utile pour les nombres ou dates)     *",
    "     *  LIKE         *  Recherche en spécifiant le début, milieu ou fin d'un mot.                     *",
    "     *  IS NULL      *  Valeur est nulle                                                                *",
    "     *  IS NOT NULL  *  Valeur n'est pas nulle                                                          *",
    "     ****************************************************************************************************",
    "",
    "",
    NULL
};

const char* help_order_by[] = {
    "*** ORDER BY ***",
    "",
    "     Une requête où l'on souhaite filtrer l'ordre des résultats utilise la commande ORDER BY de la sorte :",
    "",
    "          SELECT colonne1, colonne2",
    "          FROM table",
    "          ORDER BY colonne1",
    "",
    "     Par défaut les résultats sont classés par ordre ascendant, toutefois il est possible d'inverser l'ordre en utilisant le",
    "     suffixe DESC après le nom de la colonne. Par ailleurs, il est possible de trier sur plusieurs colonnes en les séparant ",
    "     par une virgule. Une requête plus élaborée ressemblerait à cela :",
    "",
    "          SELECT colonne1, colonne2, colonne3",
    "          FROM table",
    "          ORDER BY colonne1 DESC, colonne2 ASC",
    "",
    NULL
};

const char* help_limit[] = {
    "*** LIMIT ***",
    "",
    " La syntaxe commune aux principaux systèmes de gestion de bases de données est la suivante :",
    "",
    "     SELECT *",
    "     FROM table",
    "     LIMIT 10",
    "",
    " Cette requête permet de récupérer seulement les 10 premiers résultats d'une table. Bien entendu, si la table contient",
    " moins de 10 résultats, alors la requête retournera toutes les lignes.",
    "",
    " Bon à savoir : la bonne pratique lorsque l'on utilise LIMIT consiste à utiliser également la clause ORDER BY pour s'assurer",
    " que quoi qu'il en soit ce sont toujours les bonnes données qui sont présentées. En effet, si le système de tri est non",
    " spécifié, alors il est en principe inconnu et les résultats peuvent être imprévisible.",
    "",
    NULL
};

const char* help_offset[] = {
    "*** OFFSET ***",
    "",
    " La syntaxe commune aux principaux systèmes de gestion de bases de données est la suivante :",
    "",
    "     SELECT *",
    "     FROM table",
    "     OFFSET 5",
    "",
    " Cette requête permet de n'afficher seulement que les 5 premières lignes rencontrées. OFFSET est souvent utilisée avec LIMIT.",
    "",
    NULL
};

const char* help_createtable[] = {
    "                          ~CREATE TABLE~",
    "                          ==============",
    "",
    "DESCRIPTION",
    "",
    "    La commande CREATE TABLE permet de créer une nouvelle table dans une base de données.",
    "    Une table est composée de colonnes (définies par leur nom et leur type de données) et peut",
    "    inclure des contraintes pour garantir l'intégrité des données.",
    "",
    "SYNTAXE",
    "",
    "    CREATE TABLE table_name (",
    "        column1 datatype [constraint],",
    "        column2 datatype [constraint],",
    "        ...",
    "        columnN datatype [constraint]",
    "    );",
    "",
    "PARAMETRES",
    "",
    "    table_name",
    "         Le nom de la table à créer. Il doit être unique dans la base de données.",
    "",
    "    columnN",
    "         Le nom de la colonne. Chaque colonne doit avoir un nom unique au sein de la table.",
    "",
    "    datatype",
    "         Le type de données que cette colonne contiendra (par exemple : INT, VARCHAR, DATE).",
    "",
    "    constraint (facultatif) :",
    "         Une contrainte sur la colonne pour garantir l'intégrité des données.",
    "         Exemples de contraintes :",
    "              - PRIMARY KEY : Identifie de manière unique chaque enregistrement.",
    "              - FOREIGN KEY : Référence une clé primaire dans une autre table.",
    "              - NOT NULL : La colonne ne peut pas contenir de valeur NULL.",
    "              - UNIQUE : La colonne ne peut contenir que des valeurs uniques.",
    "              - DEFAULT : Spécifie une valeur par défaut pour cette colonne.",
    "              - CHECK : Définit une condition que chaque valeur doit satisfaire.",
    "",
    "EXEMPLE",
    "",
    "     Créer une table 'employees' avec des colonnes de base 'id', 'name', 'age' et 'hire_date'.",
    "",
    "          CREATE TABLE employees (",
    "               id INT PRIMARY KEY,",
    "               name VARCHAR(50),",
    "               age INT,",
    "               hire_date DATE",
    "          );",
    "",
    NULL
};

const char* help_alter_table[] = {
    "                          ~ALTER TABLE~",
    "                          =============",
    "",
    "DESCRIPTION",
    "",
    "     La commande ALTER TABLE permet de modifier une table existante dans une base de données.",
    "     Elle peut être utilisée pour ajouter, modifier ou supprimer des colonnes, ou pour ajouter ou supprimer des contraintes",
    "     dans la table.",
    "",
    "SYNTAXE",
    "",
    "     ALTER TABLE table_name",
    "     ADD column_name datatype [constraint];",
    "",
    "     ALTER TABLE table_name",
    "     MODIFY column_name datatype [constraint];",
    "",
    "     ALTER TABLE table_name",
    "     DROP COLUMN column_name;",
    "",
    "     ALTER TABLE table_name",
    "     ADD CONSTRAINT constraint_name constraint_definition;",
    "",
    "     ALTER TABLE table_name",
    "     DROP CONSTRAINT constraint_name;",
    "",
    "PARAMETRES",
    "",
    "     table_name :",
    "          Le nom de la table à modifier. Elle doit exister dans la base de données.",
    "",
    "     column_name :",
    "          Le nom de la colonne à ajouter, modifier ou supprimer.",
    "",
    "     constraint_name :",
    "          Le nom de la contrainte à ajouter ou supprimer.",
    "",
    "     datatype :",
    "          Le type de données de la colonne (par exemple : INT, VARCHAR, DATE).",
    "",
    "     constraint :",
    "          Une contrainte pour garantir l'intégrité des données (par exemple : PRIMARY KEY, NOT NULL, UNIQUE).",
    "",
    "EXEMPLES",
    "",
    "     Ajouter une nouvelle colonne dans une table existante :",
    "",
    "          ALTER TABLE employees ADD phone_number VARCHAR(15);",
    "",
    "     Modifier le type de données d'une colonne existante :",
    "",
    "          ALTER TABLE employees MODIFY phone_number CHAR(10);",
    "",
    "     Supprimer une colonne d'une table :",
    "",
    "          ALTER TABLE employees DROP COLUMN phone_number;",
    "",
    "     Ajouter une contrainte UNIQUE à une colonne :",
    "",
    "          ALTER TABLE employees ADD CONSTRAINT unique_email UNIQUE (email);",
    "",
    "     Supprimer une contrainte existante :",
    "",
    "          ALTER TABLE employees DROP CONSTRAINT unique_email;",
    "",
    NULL
};

const char* help_operateurslogiques[] = {
    "                          ~AND, NOT, OR~",
    "                          ==============",
    "",
    "DESCRIPTION",
    "",
    "     Les opérateurs logiques AND, NOT, OR sont utilisés pour combiner ou exclure des conditions dans les clauses SQL,",
    "     comme WHERE et HAVING.",
    "     Ils permettent de définir des critères complexes pour les requêtes SQL.",
    "",
    "SYNTAXE",
    "",
    "     condition1 AND condition2",
    "     condition1 OR condition2",
    "     NOT condition",
    "",
    "DEFINITIONS",
    "",
    "     AND",
    "",
    "          Retourne TRUE si les deux conditions sont vraies.",
    "",
    "     OR",
    "",
    "          Retourne TRUE si au moins une des deux conditions est vraie.",
    "",
    "     NOT",
    "",
    "          Inverse la valeur d'une condition (TRUE devient FALSE, et vice versa).",
    "",
    "EXEMPLES",
    "",
    "     Utilisation de AND pour filtrer plusieurs critères",
    "",
    "          SELECT * FROM employees",
    "          WHERE age > 30 AND department = 'Engineering';",
    "",
    "     Utilisation de OR pour accepter l'une des deux conditions",
    "",
    "          SELECT * FROM employees",
    "          WHERE department = 'Engineering' OR department = 'Sales';",
    "",
    "     Utilisation de NOT pour exclure une condition",
    "",
    "          SELECT * FROM employees",
    "          WHERE NOT department = 'Marketing';",
    "",
    NULL
};

const char* help_groupby[] = {
    "                          ~GROUP BY~",
    "                          ==========",
    "",
    "DESCRIPTION",
    "",
    "     La commande GROUP BY est utilisée pour regrouper les lignes d'une table qui ont des valeurs identiques dans des",
    "     colonnes spécifiées.",
    "     Elle est souvent utilisée avec des fonctions d'agrégation comme COUNT, AVG, MIN et MAX pour résumer les données.",
    "",
    "SYNTAXE",
    "",
    "     SELECT column1, column2, ..., aggregate_function(columnN)",
    "     FROM table_name",
    "     WHERE condition",
    "     GROUP BY column1, column2, ...;",
    "",
    "PARAMETRES",
    "",
    "     column1, column2, ...",
    "",
    "          Les colonnes utilisées pour regrouper les lignes.",
    "",
    "     aggregate_function(columnN)",
    "",
    "          Une fonction d'agrégation appliquée à une colonne, comme COUNT, SUM, AVG, etc.",
    "",
    "     table_name",
    "",
    "          Le nom de la table contenant les données.",
    "",
    "     condition",
    "",
    "          Une condition facultative pour filtrer les lignes avant le regroupement.",
    "",
    "EXEMPLE",
    "",
    "     Regrouper les employés par département et compter leur nombre",
    "",
    "          SELECT department, COUNT(*)",
    "          FROM employees",
    "          GROUP BY department;",
    NULL
};

const char* help_having[] = {
    "                          ~HAVING~",
    "                          ========",
    "",
    "DESCRIPTION",
    "",
    "     La commande HAVING est utilisée pour filtrer les groupes de données après qu'une clause GROUP BY ait été appliquée",
    "     dans une requête SQL.",
    "     Elle permet de spécifier des conditions basées sur les résultats des fonctions d'agrégation.",
    "",   
    "SYNTAXE",
    "",
    "     SELECT column1, aggregate_function(column2), ...",
    "     FROM table_name",
    "     WHERE condition",
    "     GROUP BY column1, column2, ...",
    "     HAVING condition;",
    "",
    "PARAMETRES",
    "",
    "     column1, column2, ...",
    "",
    "          Les colonnes utilisées pour regrouper ou afficher les données.",
    "",
    "     aggregate_function(column)",
    "",
    "          Une fonction d'agrégation appliquée à une colonne, comme COUNT, SUM, AVG, etc.",
    "",
    "     condition",
    "",
    "          Une condition à appliquer sur les groupes de données après le regroupement.",
    "",
    "     table_name",
    "",
    "          Le nom de la table contenant les données.",
    "",
    "EXEMPLE",
    "",
    "    Afficher les départements avec plus de 5 employés :",
    "         SELECT department, COUNT(*) AS num_employees",
    "         FROM employees",
    "         GROUP BY department",
    "         HAVING COUNT(*) > 5;",
    NULL
};


void manpage(const char* manual_text[]) {
    int ch;
    int start_line = 0; // Première ligne visible
    int max_lines;      // Nombre maximal de lignes affichables
    int num_lines = 0;  // Nombre total de lignes dans le texte

    // Compter le nombre de lignes dans le texte
    while (manual_text[num_lines] != NULL) {
        num_lines++;
    }

    // Initialisation ncurses
    initscr();
    start_color();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);

    // Initialiser les couleurs
    init_pair(1, COLOR_BLACK, COLOR_CYAN); // Texte noir sur fond cyan

    // Calculer les dimensions de l'écran
    max_lines = LINES - 2; // Garder 2 lignes pour l'interface (barres en haut/bas)

    // Affichage initial
    while (1) {
        // Préparer le tampon d'affichage
        erase(); // Effacer uniquement le contenu visible

        // Afficher le texte, limité à l'écran
        for (int i = 0; i < max_lines; i++) {
            if (start_line + i >= num_lines) break;
            mvprintw(i, 0, "%s", manual_text[start_line + i]);
        }

        // Afficher une barre d'état avec un fond cyan
        attron(COLOR_PAIR(1)); // Activer les couleurs
        mvprintw(LINES - 1, 0, " 'q' pour quitter | Utilisez les flèches pour naviguer");
        // Afficher la position dynamique
        mvprintw(LINES - 1, COLS - 20, "Ligne %d/%d", ((num_lines < max_lines) ? num_lines : start_line + max_lines), num_lines);
        attroff(COLOR_PAIR(1)); // Désactiver les couleurs

        // Préparer l'affichage
        wnoutrefresh(stdscr); // Mettre à jour le tampon
        doupdate();           // Appliquer les changements

        // Gestion des entrées utilisateur
        ch = getch();
        if (ch == 'q') {
            break; // Quitter
        } else if (ch == KEY_DOWN) {
            if (start_line + max_lines < num_lines) {
                start_line++;
            }
        } else if (ch == KEY_UP) {
            if (start_line > 0) {
                start_line--;
            }
        }

        // Limiter la vitesse de rafraîchissement
        napms(10); // Pause de 10 millisecondes pour éviter les répétitions rapides
    }

    // Terminer ncurses
    endwin();
    return;
}


void submenu() {
    int choix;
    int temp;

    putchar('\n');

    printf("Menu d'aide :\n"
            " (0)  Quitter\n"
            " (1)  Manuel\n"
            " (2)  SELECT\n"
            " (3)  FROM\n"
            " (4)  WHERE\n"
            " (5)  ORDER BY\n"
            " (6)  LIMIT\n"
            " (7)  OFFSET\n"
            " (8)  CREATE TABLE\n"
            " (9)  ALTER TABLE\n"
            " (10) GROUP BY\n"
            " (11) HAVING\n"
            " (12) DROP TABLE - non fait\n"
            " (13) DELETE FROM - non fait\n"
            " (14) CREATE INDEX - non fait\n");

    putchar('\n');

    do {
        printf("Entrez votre choix :\n");
        printf("   > ");
        temp = scanf("%d", &choix);
        if (temp != 1) {
            printf("Format invalide. Essayez à nouveau.\n");
            clearInputBuffer();
        }
    } while (temp != 1);

    clearInputBuffer();

    switch (choix) {

    case 0:
        puts("Sortie de la page d'aide.");
        break;
    case 1:
        manpage(manual);
        break;
    case 2:
        manpage(help_select);
        break;
    case 3:
        manpage(help_from);
        break;
    case 4:
        manpage(help_where);
        break;
    case 5:
        manpage(help_order_by);
        break;
    case 6:
        manpage(help_limit);
        break;
    case 7:
        manpage(help_offset);
        break;
    case 8:
        manpage(help_createtable);
        break;
    case 9:
        manpage(help_alter_table);
        break;
    case 10:
        manpage(help_groupby);
        break;
    case 11:
        manpage(help_having);
        break;
    default:
        puts("commande inconnue");
        break;
    }

    return;
}
