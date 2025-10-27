#ifndef NEWSQL_H
#define NEWSQL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Définition de la structure SQL_QUERY
typedef struct SQL_QUERY {
    char** select_fields;
    char** from_tables;
    char** where_conditions;
    char** groupby_fields;
    char** having_conditions;
    char** orderby_fields;
    char** aggregate_functions; // NEW: Store aggregates like "MIN(age)"
    int select_count;
    int from_count;
    int where_count;
    int groupby_count;
    int having_count;
    int orderby_count;
    int aggregate_count; // NEW: Count of aggregate functions
    int limit;
    int offset;
} SQL_QUERY;


// Déclaration des fonctions
SQL_QUERY* init_query();  // Initialiser une nouvelle requête SQL
void free_query(SQL_QUERY* query);  // Libérer la mémoire d'une requête SQL

void add_select(SQL_QUERY* query, const char* field);  // Ajouter un champ SELECT
void add_from(SQL_QUERY* query, const char* table);    // Ajouter une table FROM
void add_where(SQL_QUERY* query, const char* condition);  // Ajouter une condition WHERE
void add_groupby(SQL_QUERY* query, const char* field);    // Ajouter un champ GROUP BY
void add_having(SQL_QUERY* query, const char* condition); // Ajouter une condition HAVING
void add_orderby(SQL_QUERY* query, const char* field);    // Ajouter un champ ORDER BY
void set_limit(SQL_QUERY* query, int limit);              // Définir la limite de résultats
void set_offset(SQL_QUERY* query, int offset);            // Définir l'offset des résultats

void print_query(const SQL_QUERY* query);  // Afficher la requête SQL

char* read_query();  // Lire une requête SQL depuis l'entrée utilisateur
void parse_query(const char* query_str, SQL_QUERY* query);  // Analyser une requête SQL

void translate_query_to_execution(
    const SQL_QUERY* query,
    char*** select_fields, int* select_count,
    char*** from_tables, int* from_count,
    char*** where_conditions, int* where_count,
    char*** groupby_fields, int* groupby_count,
    char*** having_conditions, int* having_count,
    char*** orderby_fields, char*** orderby_directions, int* orderby_count,
    int* limit, int* offset
);  // Traduire une requête SQL en exécution

// Fonctions internes (non exposées dans l'API publique)
void add_field(char*** fields, int* count, const char* value);  // Ajouter un champ à un tableau dynamique
void process_select_section(const char* content, SQL_QUERY* query);  // Traiter la section SELECT
void process_from_section(const char* content, SQL_QUERY* query);    // Traiter la section FROM
void process_where_section(const char* content, SQL_QUERY* query);   // Traiter la section WHERE
void process_group_by_section(const char* content, SQL_QUERY* query); // Traiter la section GROUP BY
void process_having_section(const char* content, SQL_QUERY* query);  // Traiter la section HAVING
void process_order_by_section(const char* content, SQL_QUERY* query); // Traiter la section ORDER BY

#endif // NEWSQL_H