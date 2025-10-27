#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../utils.h" // Inclusion de utils.h pour les couleurs

#include "help_commande.h"


void help_select() {
    printf("%s*** Bienvenue dans l'aide de la commande SQL : %sSELECT%s ***%s\n", GREEN, CYAN, GREEN, RESET);
    printf("%sLa commande %sSELECT%s est comme un buffet à volonté : elle te permet de choisir ce que tu veux voir dans ta base de données.%s\n", GRAY, CYAN, GRAY, RESET);
    printf("%sExemple :%s\n", RED, RESET);
    printf("%sSELECT%s colonne1, colonne2 FROM table;\n", BLUE, GRAY);
    printf("%sSELECT%s * FROM table;\n", BLUE, GRAY);
    printf("%sAstuce amusante :%s Pense à SELECT comme un projecteur : il éclaire ce que tu veux voir.%s\n", RED, RESET, RESET);
}

void help_from() {
    printf("%s*** Bienvenue dans l'aide de la commande SQL : %sFROM%s ***%s\n", GREEN, CYAN, GREEN, RESET);
    printf("%sLa commande %sFROM%s est le GPS de ta requête SQL. Elle indique d'où tu tires tes données.%s\n", GRAY, CYAN, GRAY, RESET);
    printf("%sExemple :%s\n", RED, RESET);
    printf("%sSELECT%s colonne1 FROM %stable%s;\n", BLUE, GRAY, CYAN, GRAY);
    printf("%sAstuce drôle :%s FROM, c'est comme dire : 'Je vais chercher mes pâtes dans la cuisine !'%s\n", RED, RESET, RESET);
}

void help_where() {
    printf("%s*** Bienvenue dans l'aide de la commande SQL : %sWHERE%s ***%s\n", GREEN, CYAN, GREEN, RESET);
    printf("%sLa commande %sWHERE%s est le filtre de ta requête SQL. Elle te permet de sélectionner uniquement ce qui t'intéresse.%s\n", GRAY, CYAN, GRAY, RESET);
    printf("%sExemple :%s\n", RED, RESET);
    printf("%sSELECT%s colonne1 FROM table WHERE colonne1 = 'valeur';\n", BLUE, GRAY);
    printf("%sAstuce drôle :%s WHERE, c'est comme dire : 'Je veux seulement les bonbons rouges dans le paquet !'%s\n", RED, RESET, RESET);
}

void help_order_by() {
    printf("%s*** Bienvenue dans l'aide de la commande SQL : %sORDER BY%s ***%s\n", GREEN, CYAN, GREEN, RESET);
    printf("%sLa commande %sORDER BY%s est comme un organisateur d'étagère. Elle range tes données dans l'ordre que tu veux.%s\n", GRAY, CYAN, GRAY, RESET);
    printf("%sExemple :%s\n", RED, RESET);
    printf("%sSELECT%s colonne1 FROM table ORDER BY colonne1 ASC;\n", BLUE, GRAY);
    printf("%sAstuce drôle :%s ORDER BY, c'est comme dire : 'Range-moi ça par taille, du plus petit au plus grand !'%s\n", RED, RESET, RESET);
}

void help_limit() {
    printf("%s*** Bienvenue dans l'aide de la commande SQL : %sLIMIT%s ***%s\n", GREEN, CYAN, GREEN, RESET);
    printf("%sLa commande %sLIMIT%s te permet de dire : 'Je veux seulement les X premières lignes.'%s\n", GRAY, CYAN, GRAY, RESET);
    printf("%sExemple :%s\n", RED, RESET);
    printf("%sSELECT%s colonne1 FROM table LIMIT 10;\n", BLUE, GRAY);
    printf("%sAstuce drôle :%s LIMIT, c'est comme dire : 'Pas plus de 10 bonbons, je fais attention à ma ligne !'%s\n", RED, RESET, RESET);
}

void help_offset() {
    printf("%s*** Bienvenue dans l'aide de la commande SQL : %sOFFSET%s ***%s\n", GREEN, CYAN, GREEN, RESET);
    printf("%sLa commande %sOFFSET%s te permet de sauter un certain nombre de lignes avant de commencer à afficher les résultats.%s\n", GRAY, CYAN, GRAY, RESET);
    printf("%sExemple :%s\n", RED, RESET);
    printf("%sSELECT%s colonne1 FROM table LIMIT 10 OFFSET 5;\n", BLUE, GRAY);
    printf("%sAstuce drôle :%s OFFSET, c'est comme dire : 'Ignore les 5 premières parts de gâteau, je prends la 6ème !'%s\n", RED, RESET, RESET);
}

void help_createtable(){
	printf(GREEN"*** Bienvenue dans l'aide de la commande SQL : ***\n\n"RESET);
	    printf(CYAN"                          ~CREATE TABLE~\n"RESET);
    printf("                          ==============\n\n");

    printf("DESCRIPTION:\n");
    printf("  La commande");
    printf(CYAN"  CREATE TABLE"RESET);
    printf(" permet de créer une nouvelle");
    printf(CYAN"  table"RESET);
    printf("  dans une base de données.\n");
    printf("  Une");
    printf(CYAN" table"RESET);
    printf(" est composée de colonnes (définies par leur nom et leur type de données) et peut\n");
    printf("  inclure des contraintes pour garantir l'intégrité des données.\n\n");

    printf("SYNTAXE:\n");
    printf(GREEN"  CREATE TABLE table_name (\n");
    printf("      column1 datatype [constraint],\n");
    printf("      column2 datatype [constraint],\n");
    printf("      ...\n");
    printf("      columnN datatype [constraint]\n");
    printf("  );\n\n"RESET);

    printf("PARAMÈTRES:\n");
    printf(GREEN"  table_name :\n");
    printf("      Le nom de la table à créer. Il doit être unique dans la base de données.\n\n");

    printf("  columnN :\n");
    printf("      Le nom de la colonne. Chaque colonne doit avoir un nom unique au sein de la table.\n\n");

    printf("  datatype :\n");
    printf("      Le type de données que cette colonne contiendra (par exemple : INT, VARCHAR, DATE).\n\n");

    printf("  constraint (facultatif) :\n");
    printf("      Une contrainte sur la colonne pour garantir l'intégrité des données.\n"RESET);
    printf("      Exemples de contraintes :\n        - ");
    printf(CYAN"PRIMARY KEY"RESET);
    printf(" : Identifie de manière unique chaque enregistrement.\n        - ");
    printf(CYAN"FOREIGN KEY"RESET);
    printf(" : Référence une clé primaire dans une autre table.\n        - ");
    printf(CYAN"NOT NULL"RESET);
    printf(" : La colonne ne peut pas contenir de valeur NULL.\n        - ");
    printf(CYAN"UNIQUE"RESET);
    printf(" : La colonne ne peut contenir que des valeurs uniques.\n        - ");
    printf(CYAN"DEFAULT"RESET);
    printf(" : Spécifie une valeur par défaut pour cette colonne.\n        - ");
    printf(CYAN"CHECK"RESET);
    printf(" : Définit une condition que chaque valeur doit satisfaire.\n\n");

    printf(RED"EXEMPLES:\n\n");
    printf("1. Créer une table simple avec des colonnes de base :\n");
    printf("   CREATE TABLE employees (\n");
    printf("       id INT PRIMARY KEY,\n");
    printf("       name VARCHAR(50),\n");
    printf("       age INT,\n");
    printf("       hire_date DATE\n");
    printf("   );\n\n"RESET);
}

void help_alter_table() {
    printf(GREEN"*** Bienvenue dans l'aide de la commande SQL : ***\n\n"RESET);
    printf(CYAN"                          ~ALTER TABLE~\n"RESET);
    printf("                          =============\n\n");

    printf("DESCRIPTION:\n");
    printf("  La commande");
    printf(CYAN" ALTER TABLE"RESET);
    printf(" permet de modifier une table existante dans une base de données.\n");
    printf("  Elle peut être utilisée pour ajouter, modifier ou supprimer des colonnes, ou pour\n");
    printf("  ajouter ou supprimer des contraintes dans la table.\n\n");

    printf("SYNTAXE:\n");
    printf(GREEN"  ALTER TABLE table_name\n");
    printf("      ADD column_name datatype [constraint];\n\n");
    printf("  ALTER TABLE table_name\n");
    printf("      MODIFY column_name datatype [constraint];\n\n");
    printf("  ALTER TABLE table_name\n");
    printf("      DROP COLUMN column_name;\n\n");
    printf("  ALTER TABLE table_name\n");
    printf("      ADD CONSTRAINT constraint_name constraint_definition;\n\n");
    printf("  ALTER TABLE table_name\n");
    printf("      DROP CONSTRAINT constraint_name;\n\n"RESET);

    printf("PARAMÈTRES:\n");
    printf(GREEN"  table_name :\n");
    printf("      Le nom de la table à modifier. Elle doit exister dans la base de données.\n\n");

    printf("  column_name :\n");
    printf("      Le nom de la colonne à ajouter, modifier ou supprimer.\n\n");

    printf("  constraint_name :\n");
    printf("      Le nom de la contrainte à ajouter ou supprimer.\n\n");

    printf("  datatype :\n");
    printf("      Le type de données de la colonne (par exemple : INT, VARCHAR, DATE).\n\n");

    printf("  constraint :\n");
    printf("      Une contrainte pour garantir l'intégrité des données (par exemple : PRIMARY KEY,\n");
    printf("      NOT NULL, UNIQUE).\n\n"RESET);

    printf(RED"EXEMPLES:\n\n");
    printf("1. Ajouter une nouvelle colonne dans une table existante :\n");
    printf("   ALTER TABLE employees ADD phone_number VARCHAR(15);\n\n");

    printf("2. Modifier le type de données d'une colonne existante :\n");
    printf("   ALTER TABLE employees MODIFY phone_number CHAR(10);\n\n");

    printf("3. Supprimer une colonne d'une table :\n");
    printf("   ALTER TABLE employees DROP COLUMN phone_number;\n\n");

    printf("4. Ajouter une contrainte UNIQUE à une colonne :\n");
    printf("   ALTER TABLE employees ADD CONSTRAINT unique_email UNIQUE (email);\n\n");

    printf("5. Supprimer une contrainte existante :\n");
    printf("   ALTER TABLE employees DROP CONSTRAINT unique_email;\n\n"RESET);
}

void help_operateurslogiques() {
    printf(GREEN"*** Bienvenue dans l'aide des opérateurs logiques SQL : ***\n\n"RESET);
    printf(CYAN"                          ~AND, NOT, OR~\n"RESET);
    printf("                          ==============\n\n");

    printf("DESCRIPTION:\n");
    printf("  Les opérateurs logiques");
    printf(CYAN" AND, NOT, OR"RESET);
    printf(" sont utilisés pour combiner ou exclure\n");
    printf("  des conditions dans les clauses SQL, comme");
    printf(GREEN" WHERE"RESET);
    printf(" et");
    printf(GREEN" HAVING"RESET);
    printf(".\n");
    printf("  Ils permettent de définir des critères complexes pour les requêtes SQL.\n\n");

    printf("SYNTAXE:\n");
    printf(GREEN"  condition1 AND condition2\n");
    printf("  condition1 OR condition2\n");
    printf("  NOT condition\n\n"RESET);

    printf("DÉFINITIONS:\n");
    printf(GREEN"  AND :\n");
    printf("      Retourne TRUE si les deux conditions sont vraies.\n\n");

    printf(GREEN"  OR :\n");
    printf("      Retourne TRUE si au moins une des deux conditions est vraie.\n\n");

    printf(GREEN"  NOT :\n");
    printf("      Inverse la valeur d'une condition (TRUE devient FALSE, et vice versa).\n\n");

    printf(RED"EXEMPLES:\n\n");

    printf("1. Utilisation de");
    printf(CYAN" AND");
    printf(RED" pour filtrer plusieurs critères :\n");
    printf("   SELECT * FROM employees\n");
    printf("   WHERE age > 30 AND department = 'Engineering';\n\n");

    printf("2. Utilisation de");
    printf(CYAN" OR");
    printf(RED" pour accepter l'une des deux conditions :\n");
    printf("   SELECT * FROM employees\n");
    printf("   WHERE department = 'Engineering' OR department = 'Sales';\n\n");

    printf("3. Utilisation de");
    printf(CYAN" NOT");
    printf(RED" pour exclure une condition :\n");
    printf("   SELECT * FROM employees\n");
    printf("   WHERE NOT department = 'Marketing';\n\n"RESET);
   }

void help_groupby() {
    printf(GREEN"*** Bienvenue dans l'aide de la commande SQL : ***\n\n"RESET);
    printf(CYAN"                          ~GROUP BY~\n"RESET);
    printf("                          ==========\n\n");

    printf("DESCRIPTION:\n");
    printf("  La commande");
    printf(CYAN" GROUP BY"RESET);
    printf(" est utilisée pour regrouper les lignes d'une table\n");
    printf("  qui ont des valeurs identiques dans des colonnes spécifiées.\n");
    printf("  Elle est souvent utilisée avec des fonctions d'agrégation comme");
    printf(GREEN" COUNT"RESET);
    printf(", ");
    printf(GREEN"SUM"RESET);
    printf(",\n");
    printf("  ");
    printf(GREEN"AVG"RESET);
    printf(", ");
    printf(GREEN"MIN"RESET);
    printf(" et ");
    printf(GREEN"MAX"RESET);
    printf(" pour résumer les données.\n\n");

    printf("SYNTAXE:\n");
    printf(GREEN"  SELECT column1, column2, ..., aggregate_function(columnN)\n");
    printf("  FROM table_name\n");
    printf("  WHERE condition\n");
    printf("  GROUP BY column1, column2, ...;\n\n"RESET);

    printf("PARAMÈTRES:\n");
    printf(GREEN"  column1, column2, ... :\n");
    printf("      Les colonnes utilisées pour regrouper les lignes.\n\n");

    printf(GREEN"  aggregate_function(columnN) :\n");
    printf("      Une fonction d'agrégation appliquée à une colonne, comme COUNT, SUM, AVG, etc.\n\n");

    printf(GREEN"  table_name :\n");
    printf("      Le nom de la table contenant les données.\n\n");

    printf(GREEN"  condition :\n");
    printf("      Une condition facultative pour filtrer les lignes avant le regroupement.\n\n");

    printf(RED"EXEMPLE:\n");

    printf("Regrouper les employés par département et compter leur nombre :\n");
    printf("   SELECT department, COUNT(*)\n");
    printf("   FROM employees\n");
    printf("   GROUP BY department;\n\n"RESET);
   } 
void help_having() {
    printf(GREEN"*** Bienvenue dans l'aide de la commande SQL : ***\n\n"RESET);
    printf(CYAN"                          ~HAVING~\n"RESET);
    printf("                          ========\n\n");

    printf("DESCRIPTION:\n");
    printf("  La commande");
    printf(CYAN" HAVING"RESET);
    printf(" est utilisée pour filtrer les groupes de données\n");
    printf("  après qu'une clause");
    printf(CYAN" GROUP BY"RESET);
    printf(" ait été appliquée dans une requête SQL.\n");
    printf("  Elle permet de spécifier des conditions basées sur les résultats des\n");
    printf("  fonctions d'agrégation (par exemple COUNT, SUM, AVG, etc.).\n\n");

    printf("SYNTAXE:\n");
    printf(GREEN"  SELECT column1, aggregate_function(column2), ...\n");
    printf("  FROM table_name\n");
    printf("  WHERE condition\n");
    printf("  GROUP BY column1, column2, ...\n");
    printf("  HAVING condition;\n\n"RESET);

    printf("PARAMÈTRES:\n");
    printf(GREEN"  column1, column2, ... :\n");
    printf("      Les colonnes utilisées pour regrouper ou afficher les données.\n\n");

    printf(GREEN"  aggregate_function(column) :\n");
    printf("      Une fonction d'agrégation appliquée à une colonne, comme COUNT, SUM, AVG, etc.\n\n");

    printf(GREEN"  condition :\n");
    printf("      Une condition à appliquer sur les groupes de données après le regroupement.\n\n");

    printf(GREEN"  table_name :\n");
    printf("      Le nom de la table contenant les données.\n\n");

    printf(RED"EXEMPLES:\n\n");

    printf("Afficher les départements avec plus de 5 employés :\n");
    printf("   SELECT department, COUNT(*) AS num_employees\n");
    printf("   FROM employees\n");
    printf("   GROUP BY department\n");
    printf("   HAVING COUNT(*) > 5;\n\n");
}

int main() {
    // Appel aux fonctions d'aide
    help_select();
    help_from();
    help_where();
    help_order_by();
    help_limit();
    help_offset();
    help_createtable();
    help_alter_table();
    help_operateurslogiques();
    help_groupby();
    help_having();

    return 0;
}
