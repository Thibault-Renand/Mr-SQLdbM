#include "newSQL.h"
void translate_query_to_execution(const SQL_QUERY* query, char*** select_fields, int* select_count,char*** from_tables, int* from_count,char*** where_conditions, int* where_count,char*** groupby_fields, int* groupby_count,char*** having_conditions, int* having_count,char*** orderby_fields, char*** orderby_directions, int* orderby_count,int* limit, int* offset) {

    *select_fields = query->select_fields;
    *select_count = query->select_count;

    *from_tables = query->from_tables;
    *from_count = query->from_count;

    *where_conditions = query->where_conditions;
    *where_count = query->where_count;

    *groupby_fields = query->groupby_fields;
    *groupby_count = query->groupby_count;

    *having_conditions = query->having_conditions;
    *having_count = query->having_count;

    *orderby_fields = query->orderby_fields;
    *orderby_directions = malloc(query->orderby_count * sizeof(char*));
    for (int i = 0; i < query->orderby_count; i++) {
        (*orderby_directions)[i] = strstr(query->orderby_fields[i], "DESC") ? "DESC" : "ASC";
    }
    *orderby_count = query->orderby_count;

    *limit = query->limit;
    *offset = query->offset;

    // NEW: Log aggregate functions for debugging
    printf("Aggregate functions:\n");
    for (int i = 0; i < query->aggregate_count; i++) {
        printf("  %s\n", query->aggregate_functions[i]);
    }
}

SQL_QUERY* init_query() {
    SQL_QUERY* query = (SQL_QUERY*)malloc(sizeof(SQL_QUERY));
    if (!query) {
        perror("Erreur d'allocation mémoire");
        exit(EXIT_FAILURE);
    }

    query->select_fields = NULL;
    query->from_tables = NULL;
    query->where_conditions = NULL;
    query->groupby_fields = NULL;
    query->having_conditions = NULL; // Initialisation pour HAVING
    query->orderby_fields = NULL;
    query->select_count = 0;
    query->from_count = 0;
    query->where_count = 0;
    query->groupby_count = 0;
    query->having_count = 0; // Initialisation compteur HAVING
    query->orderby_count = 0;
    query->aggregate_count=0;
    query->limit = -1;  // Aucune limite par défaut
    query->offset = -1; // Aucun décalage par défaut

    return query;
}

void add_field(char*** fields, int* count, const char* value) {
    *fields = realloc(*fields, (*count + 1) * sizeof(char*));
    if (!*fields) {
        perror("Erreur d'allocation mémoire");
        exit(EXIT_FAILURE);
    }

    (*fields)[*count] = strdup(value); // Copier la chaîne dans la mémoire dynamique
    if (!(*fields)[*count]) {
        perror("Erreur d'allocation mémoire");
        exit(EXIT_FAILURE);
    }

    (*count)++;
}
void set_limit(SQL_QUERY* query, int limit) {
    query->limit = limit;
}
void set_offset(SQL_QUERY* query, int offset) {
    query->offset = offset;
}

void add_select(SQL_QUERY* query, const char* field) {
    add_field(&query->select_fields, &query->select_count, field);
}

void add_from(SQL_QUERY* query, const char* table) {
    add_field(&query->from_tables, &query->from_count, table);
}

void add_where(SQL_QUERY* query, const char* condition) {
    add_field(&query->where_conditions, &query->where_count, condition);
}

void add_groupby(SQL_QUERY* query, const char* field) {
    add_field(&query->groupby_fields, &query->groupby_count, field);
}
void add_having(SQL_QUERY* query, const char* condition) {
    add_field(&query->having_conditions, &query->having_count, condition);
}

void add_orderby(SQL_QUERY* query, const char* field) {
    add_field(&query->orderby_fields, &query->orderby_count, field);
}
void print_query(const SQL_QUERY* query) {
    // SECTION SELECT
    printf("SELECT ");
    for (int i = 0; i < query->select_count; i++) {
        printf("%s", query->select_fields[i]);
        if (i < query->select_count - 1) printf(", ");
    }
    printf("\n");

    // SECTION FROM
    printf("FROM ");
    for (int i = 0; i < query->from_count; i++) {
        printf("%s", query->from_tables[i]);
        if (i < query->from_count - 1) printf(", ");
    }
    printf("\n");

    // SECTION WHERE
    if (query->where_count > 0) {
        printf("WHERE ");
        for (int i = 0; i < query->where_count; i++) {
            printf("%s", query->where_conditions[i]);
            if (i < query->where_count - 1) printf(" AND ");
        }
        printf("\n");
    }

    // SECTION GROUP BY
    if (query->groupby_count > 0) {
        printf("GROUP BY ");
        for (int i = 0; i < query->groupby_count; i++) {
            printf("%s", query->groupby_fields[i]);
            if (i < query->groupby_count - 1) printf(", ");
        }
        printf("\n");
    }

    // SECTION HAVING
    if (query->having_count > 0) {
        printf("HAVING ");
        for (int i = 0; i < query->having_count; i++) {
            printf("%s", query->having_conditions[i]);
            if (i < query->having_count - 1) printf(" AND ");
        }
        printf("\n");
    }

    // SECTION ORDER BY
    if (query->orderby_count > 0) {
        printf("ORDER BY ");
        for (int i = 0; i < query->orderby_count; i++) {
            printf("%s", query->orderby_fields[i]);
            if (i < query->orderby_count - 1) printf(", ");
        }
        printf("\n");
    }
    // SECTION LIMIT
    if (query->limit > 0) {
        printf("LIMIT %d\n", query->limit);
        }
        // SECTION OFFSET
    if (query->offset > 0) {
        printf("OFFSET %d\n", query->offset);
    }
}
void free_query(SQL_QUERY* query) {
    for (int i = 0; i < query->select_count; i++) free(query->select_fields[i]);
    free(query->select_fields);

    for (int i = 0; i < query->from_count; i++) free(query->from_tables[i]);
    free(query->from_tables);

    for (int i = 0; i < query->where_count; i++) free(query->where_conditions[i]);
    free(query->where_conditions);

    for (int i = 0; i < query->groupby_count; i++) free(query->groupby_fields[i]);
    free(query->groupby_fields);

    for (int i = 0; i < query->having_count; i++) free(query->having_conditions[i]); // Libérer HAVING
    free(query->having_conditions);

    for (int i = 0; i < query->orderby_count; i++) free(query->orderby_fields[i]);
    free(query->orderby_fields);

    free(query);
}

char* read_query() {
    printf("Entrez votre requête SQL :\n");
    char* query = malloc(1024 * sizeof(char)); // Allouer suffisamment d'espace
    if (!query) {
        perror("Erreur d'allocation mémoire");
        exit(EXIT_FAILURE);
    }

    if (fgets(query, 1024, stdin) == NULL) {
        free(query);
        return NULL;
    }

    // Retirer le saut de ligne éventuel
    size_t len = strlen(query);
    if (len > 0 && query[len - 1] == '\n') {
        query[len - 1] = '\0';
    }

    return query;
}
void process_select_section(const char* content, SQL_QUERY* query) {
    char* token = strtok((char*)content, ",");
    while (token) {
        // Trim leading and trailing whitespace
        while (isspace(*token)) token++;
        char* end = token + strlen(token) - 1;
        while (end > token && isspace(*end)) *end-- = '\0';

        // Check if the token is an aggregate function
        if (strstr(token, "MIN(") || strstr(token, "MAX(") || 
            strstr(token, "SUM(") || strstr(token, "AVG(") || 
            strstr(token, "COUNT(")) {
            add_field(&query->aggregate_functions, &query->aggregate_count, token);
        }

        // Always add to select_fields for consistency
        add_field(&query->select_fields, &query->select_count, token);

        token = strtok(NULL, ",");
    }
}


void process_from_section(const char* content, SQL_QUERY* query) {
    char table[128];
    int i = 0, table_idx = 0;

    while (content[i]) {
        if (content[i] == ',' || content[i] == '\0') {
            table[table_idx] = '\0';
            if (strlen(table) == 0) {
                fprintf(stderr, "Erreur : table vide détectée dans FROM.\n");
                return;
            }
            add_from(query, table);
            table_idx = 0;
        } else if (!isspace(content[i])) {
            table[table_idx++] = content[i];
        }
        i++;
    }

    if (table_idx > 0) {
        table[table_idx] = '\0';
        add_from(query, table);
    }
}

void process_where_section(const char* content, SQL_QUERY* query) {
    char condition[256] = "";
    const char* ptr = content;

    while (*ptr) {
        if (strchr("><=()", *ptr)) {
            strncat(condition, ptr, 1);
        } else if (isspace(*ptr)) {
            if (condition[strlen(condition) - 1] != ' ') {
                strcat(condition, " ");
            }
        } else {
            strncat(condition, ptr, 1);
        }
        ptr++;
    }

    char* trimmed_condition = strtok(condition, "\n");
    if (trimmed_condition) {
        add_where(query, trimmed_condition);
    }
}

void process_group_by_section(const char* content, SQL_QUERY* query) {
    char field[128];
    int i = 0, field_idx = 0;

    while (content[i]) {
        if (content[i] == ',' || content[i] == '\0') {
            field[field_idx] = '\0';
            add_groupby(query, field);
            field_idx = 0;
        } else if (!isspace(content[i])) {
            field[field_idx++] = content[i];
        }
        i++;
    }

    if (field_idx > 0) {
        field[field_idx] = '\0';
        add_groupby(query, field);
    }
}
void process_having_section(const char* content, SQL_QUERY* query) {
    char condition[256] = "";
    int in_function = 0; // Indicateur pour les fonctions comme COUNT, AVG
    const char* ptr = content;

    while (*ptr) {
        if (*ptr == '(') {
            in_function = 1; // Début d'une fonction
            strncat(condition, ptr, 1);
        } else if (*ptr == ')') {
            in_function = 0; // Fin d'une fonction
            strncat(condition, ptr, 1);
        } else if (isspace(*ptr) && !in_function) {
            // Ignorer les espaces sauf dans les fonctions
            if (condition[strlen(condition) - 1] != ' ') {
                strcat(condition, " ");
            }
        } else {
            strncat(condition, ptr, 1);
        }
        ptr++;
    }

    // Supprimer les espaces résiduels
    char* trimmed_condition = strtok(condition, "\n");
    if (trimmed_condition) {
        add_having(query, trimmed_condition);
    }
}

void process_order_by_section(const char* content, SQL_QUERY* query) {
    char field[128];
    int i = 0, field_idx = 0;

    while (content[i]) {
        if (content[i] == ',' || content[i] == '\0') {
            field[field_idx] = '\0';

            // Vérifier si le champ contient ASC ou DESC
            char* direction = NULL;
            if ((direction = strstr(field, "ASC")) != NULL) {
                *direction = '\0'; // Séparer le champ de ASC
                char combined_field[256];
                snprintf(combined_field, sizeof(combined_field), "%s ASC", field);
                add_orderby(query, combined_field);
            } else if ((direction = strstr(field, "DESC")) != NULL) {
                *direction = '\0'; // Séparer le champ de DESC
                char combined_field[256];
                snprintf(combined_field, sizeof(combined_field), "%s DESC", field);
                add_orderby(query, combined_field);
            } else {
                // Ajouter le champ sans direction explicite (par défaut)
                add_orderby(query, field);
            }

            field_idx = 0; // Réinitialiser le tampon
        } else if (!isspace(content[i])) {
            field[field_idx++] = content[i];
        }
        i++;
    }

    // Ajouter le dernier champ si nécessaire
    if (field_idx > 0) {
        field[field_idx] = '\0';

        char* direction = NULL;
        if ((direction = strstr(field, "ASC")) != NULL) {
            *direction = '\0'; // Séparer le champ de ASC
            char combined_field[256];
            snprintf(combined_field, sizeof(combined_field), "%s ASC", field);
            add_orderby(query, combined_field);
        } else if ((direction = strstr(field, "DESC")) != NULL) {
            *direction = '\0'; // Séparer le champ de DESC
            char combined_field[256];
            snprintf(combined_field, sizeof(combined_field), "%s DESC", field);
            add_orderby(query, combined_field);
        } else {
            // Ajouter le champ sans direction explicite (par défaut)
            add_orderby(query, field);
        }
    }
}

void parse_query(const char* query_str, SQL_QUERY* query) {
    // Locate keywords in the query
    char* select_section = strstr(query_str, "SELECT");
    char* from_section = strstr(query_str, "FROM");
    char* where_section = strstr(query_str, "WHERE");
    char* group_by_section = strstr(query_str, "GROUP BY");
    char* having_section = strstr(query_str, "HAVING");
    char* order_by_section = strstr(query_str, "ORDER BY");
    char* limit_section = strstr(query_str, "LIMIT");
    char* offset_section = strstr(query_str, "OFFSET");

    // Process SELECT section
    if (select_section && from_section) {
        char select_content[1024];
        strncpy(select_content, select_section + 6, from_section - (select_section + 6));
        select_content[from_section - (select_section + 6)] = '\0'; // Null terminate
        process_select_section(select_content, query);
    } else {
        fprintf(stderr, "Error: SELECT or FROM keyword missing.\n");
        return;
    }

    // Process FROM section
    if (from_section) {
        char from_content[1024] = "";
        if (where_section) {
            strncpy(from_content, from_section + 4, where_section - (from_section + 4));
            from_content[where_section - (from_section + 4)] = '\0';
        } else if (group_by_section) {
            strncpy(from_content, from_section + 4, group_by_section - (from_section + 4));
            from_content[group_by_section - (from_section + 4)] = '\0';
        } else if (having_section) {
            strncpy(from_content, from_section + 4, having_section - (from_section + 4));
            from_content[having_section - (from_section + 4)] = '\0';
        } else if (order_by_section) {
            strncpy(from_content, from_section + 4, order_by_section - (from_section + 4));
            from_content[order_by_section - (from_section + 4)] = '\0';
        } else if (limit_section) {
            strncpy(from_content, from_section + 4, limit_section - (from_section + 4));
            from_content[limit_section - (from_section + 4)] = '\0';
        } else if (offset_section) {
            strncpy(from_content, from_section + 4, offset_section - (from_section + 4));
            from_content[offset_section - (from_section + 4)] = '\0';
        } else {
            strcpy(from_content, from_section + 4);
        }
        process_from_section(from_content, query);
    }

    // Process WHERE section
    if (where_section) {
        char where_content[1024] = "";
        if (group_by_section) {
            strncpy(where_content, where_section + 5, group_by_section - (where_section + 5));
            where_content[group_by_section - (where_section + 5)] = '\0';
        } else if (having_section) {
            strncpy(where_content, where_section + 5, having_section - (where_section + 5));
            where_content[having_section - (where_section + 5)] = '\0';
        } else if (order_by_section) {
            strncpy(where_content, where_section + 5, order_by_section - (where_section + 5));
            where_content[order_by_section - (where_section + 5)] = '\0';
        } else if (limit_section) {
            strncpy(where_content, where_section + 5, limit_section - (where_section + 5));
            where_content[limit_section - (where_section + 5)] = '\0';
        } else if (offset_section) {
            strncpy(where_content, where_section + 5, offset_section - (where_section + 5));
            where_content[offset_section - (where_section + 5)] = '\0';
        } else {
            strcpy(where_content, where_section + 5);
        }
        process_where_section(where_content, query);
    }

    // Process GROUP BY section
    if (group_by_section) {
        char group_by_content[1024] = "";
        if (having_section) {
            strncpy(group_by_content, group_by_section + 8, having_section - (group_by_section + 8));
            group_by_content[having_section - (group_by_section + 8)] = '\0';
        } else if (order_by_section) {
            strncpy(group_by_content, group_by_section + 8, order_by_section - (group_by_section + 8));
            group_by_content[order_by_section - (group_by_section + 8)] = '\0';
        } else if (limit_section) {
            strncpy(group_by_content, group_by_section + 8, limit_section - (group_by_section + 8));
            group_by_content[limit_section - (group_by_section + 8)] = '\0';
        } else {
            strcpy(group_by_content, group_by_section + 8);
        }
        process_group_by_section(group_by_content, query);
    }

    // Process HAVING section
    if (having_section) {
        char having_content[1024] = "";
        if (order_by_section) {
            strncpy(having_content, having_section + 6, order_by_section - (having_section + 6));
            having_content[order_by_section - (having_section + 6)] = '\0';
        } else if (limit_section) {
            strncpy(having_content, having_section + 6, limit_section - (having_section + 6));
            having_content[limit_section - (having_section + 6)] = '\0';
        } else {
            strcpy(having_content, having_section + 6);
        }
        process_having_section(having_content, query);
    }

    // Process ORDER BY section
    if (order_by_section) {
        char order_by_content[1024] = "";
        if (limit_section) {
            strncpy(order_by_content, order_by_section + 8, limit_section - (order_by_section + 8));
            order_by_content[limit_section - (order_by_section + 8)] = '\0';
        } else if (offset_section) {
            strncpy(order_by_content, order_by_section + 8, offset_section - (order_by_section + 8));
            order_by_content[offset_section - (order_by_section + 8)] = '\0';
        } else {
            strcpy(order_by_content, order_by_section + 8);
        }
        process_order_by_section(order_by_content, query);
    }

    // Process LIMIT section
    if (limit_section) {
        char limit_content[128] = "";
        if (offset_section) {
            // Extract content only up to OFFSET
            strncpy(limit_content, limit_section + 5, offset_section - (limit_section + 5));
            limit_content[offset_section - (limit_section + 5)] = '\0';
        } else {
            // Extract till the end of the string
            strcpy(limit_content, limit_section + 5);
        }

        int limit_value = atoi(limit_content);
        set_limit(query, limit_value);
    }

    // Process OFFSET section
    if (offset_section) {
        char offset_content[128] = "";
        // Extract till the end of the string or a potential next section
        strcpy(offset_content, offset_section + 6);
    
        int offset_value = atoi(offset_content);
        set_offset(query, offset_value);
    }

}
/*
int main() {
    SQL_QUERY* query = init_query();

    // Lire une requête SQL
    char* input_query = read_query();

    // Analyser la requête
    parse_query(input_query, query);

    // Afficher la requête analysée
    printf("\nRequête SQL analysée :\n");
    print_query(query);

    // Libérer les ressources
    free(input_query);
    free_query(query);

    return 0;
}*/