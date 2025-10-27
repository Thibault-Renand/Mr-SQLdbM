#include "newSQL.h"
#include "testsql.h"
void translate_query_to_execution(const SQL_QUERY* query,char*** select_fields, int* select_count,char*** from_tables, int* from_count,char*** where_conditions, int* where_count,char*** groupby_fields, int* groupby_count,char*** having_conditions, int* having_count,char*** orderby_fields, char*** orderby_directions, int* orderby_count,int* limit, int* offset) {
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

    // Extract from order_data
    *orderby_fields = query->order_data.orderby_fields;
    *orderby_directions = query->order_data.orderby_directions;
    *orderby_count = query->order_data.orderby_count;

    *limit = query->limit;
    *offset = query->offset;
}

SQL_QUERY* init_query() {
    SQL_QUERY* query = (SQL_QUERY*)malloc(sizeof(SQL_QUERY));
    if (!query) {
        perror("Memory allocation error");
        exit(EXIT_FAILURE);
    }

    query->select_fields = NULL;
    query->from_tables = NULL;
    query->where_conditions = NULL;
    query->groupby_fields = NULL;
    query->having_conditions = NULL;
    query->aggregate_functions = NULL;

    query->update_fields = NULL;
    query->update_values = NULL;
    query->insert_fields = NULL;
    query->insert_values = NULL;
    query->delete_tables = NULL;
    query->create_table_definitions = NULL;

    query->alter_table_operations = NULL;
    query->alter_table_columns = NULL;
    query->alter_table_definitions = NULL;

    query->select_count = 0;
    query->from_count = 0;
    query->where_count = 0;
    query->groupby_count = 0;
    query->having_count = 0;
    query->aggregate_count = 0;

    query->update_count = 0;
    query->insert_count = 0;
    query->delete_count = 0;
    query->create_table_count = 0;
    query->alter_table_count = 0;

    query->limit = -1;
    query->offset = -1;

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
    query->order_data.orderby_fields = realloc(query->order_data.orderby_fields, (query->order_data.orderby_count + 1) * sizeof(char*));
    query->order_data.orderby_directions = realloc(query->order_data.orderby_directions, (query->order_data.orderby_count + 1) * sizeof(char*));

    if (!query->order_data.orderby_fields || !query->order_data.orderby_directions) {
        perror("Memory allocation error");
        exit(EXIT_FAILURE);
    }

    // Parse direction (default: ASC)
    char* direction = strstr(field, "DESC") ? "DESC" : "ASC";

    query->order_data.orderby_fields[query->order_data.orderby_count] = strdup(field);
    query->order_data.orderby_directions[query->order_data.orderby_count] = strdup(direction);
    query->order_data.orderby_count++;
}
void add_update(SQL_QUERY* query, const char* field, const char* value) {
    if (!query || !field || !value) {
        fprintf(stderr, "Error: NULL input to add_update.\n");
        return;
    }

    query->update_fields = realloc(query->update_fields, (query->update_count + 1) * sizeof(char*));
    query->update_values = realloc(query->update_values, (query->update_count + 1) * sizeof(char*));

    if (!query->update_fields || !query->update_values) {
        perror("Memory allocation error in add_update.");
        exit(EXIT_FAILURE);
    }

    query->update_fields[query->update_count] = strdup(field);
    query->update_values[query->update_count] = strdup(value);

    if (!query->update_fields[query->update_count] || !query->update_values[query->update_count]) {
        perror("Memory allocation error for update field/value.");
        exit(EXIT_FAILURE);
    }

    printf("Debug: Added field '%s' with value '%s'.\n", field, value);
    query->update_count++;
}
void add_delete(SQL_QUERY* query, const char* table) {
    if (!query || !table) {
        fprintf(stderr, "Error: NULL input to add_delete.\n");
        return;
    }

    query->delete_tables = realloc(query->delete_tables, (query->delete_count + 1) * sizeof(char*));
    if (!query->delete_tables) {
        perror("Memory allocation error in add_delete");
        exit(EXIT_FAILURE);
    }

    query->delete_tables[query->delete_count] = strdup(table);
    if (!query->delete_tables[query->delete_count]) {
        perror("Memory allocation error for table name in add_delete");
        exit(EXIT_FAILURE);
    }

    query->delete_count++;
}
void add_insert(SQL_QUERY* query, const char* field, const char* value) {
    if (!query || !field) {
        fprintf(stderr, "Error: NULL input to add_insert.\n");
        return;
    }

    query->insert_fields = realloc(query->insert_fields, (query->insert_count + 1) * sizeof(char*));
    if (!query->insert_fields) {
        perror("Memory allocation error for insert_fields");
        exit(EXIT_FAILURE);
    }

    query->insert_fields[query->insert_count] = strdup(field);
    if (!query->insert_fields[query->insert_count]) {
        perror("Memory allocation error for insert_field value");
        exit(EXIT_FAILURE);
    }

    if (value) {
        query->insert_values = realloc(query->insert_values, (query->insert_count + 1) * sizeof(char*));
        if (!query->insert_values) {
            perror("Memory allocation error for insert_values");
            exit(EXIT_FAILURE);
        }

        query->insert_values[query->insert_count] = strdup(value);
        if (!query->insert_values[query->insert_count]) {
            perror("Memory allocation error for insert_value");
            exit(EXIT_FAILURE);
        }
    }

    query->insert_count++;
}
void add_create_table(SQL_QUERY* query, const char* definition) {
    query->create_table_definitions = realloc(query->create_table_definitions, (query->create_table_count + 1) * sizeof(char*));
    if (!query->create_table_definitions) {
        perror("Memory allocation error");
        exit(EXIT_FAILURE);
    }

    query->create_table_definitions[query->create_table_count] = strdup(definition);
    query->create_table_count++;
}
void print_query(const SQL_QUERY* query) {
    if (!query) {
        printf("No query to print.\n");
        return;
    }

    printf("QUERY DETAILS:\n");

    // SECTION SELECT
    if (query->select_count > 0) {
        for (int i = 0; i < query->select_count; i++) {
            printf("SELECT_FIELD[%d] : %s\n", i, query->select_fields[i]);
        }
        for (int i = 0; i < query->from_count; i++) {
            printf("FROM_TABLE[%d] : %s\n", i, query->from_tables[i]);
        }
        for (int i = 0; i < query->where_count; i++) {
            printf("WHERE_CONDITION[%d] : %s\n", i, query->where_conditions[i]);
        }
        for (int i = 0; i < query->groupby_count; i++) {
            printf("GROUP_BY_FIELD[%d] : %s\n", i, query->groupby_fields[i]);
        }
        for (int i = 0; i < query->having_count; i++) {
            printf("HAVING_CONDITION[%d] : %s\n", i, query->having_conditions[i]);
        }
        for (int i = 0; i < query->order_data.orderby_count; i++) {
            printf("ORDER_BY_FIELD[%d] : %s\n", i, query->order_data.orderby_fields[i]);
            printf("ORDER_BY_DIRECTION[%d] : %s\n", i, query->order_data.orderby_directions[i]);
        }
        if (query->limit > 0) {
            printf("LIMIT : %d\n", query->limit);
        }
        if (query->offset > 0) {
            printf("OFFSET : %d\n", query->offset);
        }
    }

    // SECTION DELETE
    if (query->delete_count > 0) {
        for (int i = 0; i < query->delete_count; i++) {
            printf("DELETE_TABLE[%d] : %s\n", i, query->delete_tables[i]);
        }
        for (int i = 0; i < query->where_count; i++) {
            printf("WHERE_CONDITION[%d] : %s\n", i, query->where_conditions[i]);
        }
    }

    // SECTION UPDATE
    if (query->update_count > 0) {
        for (int i = 0; i < query->from_count; i++) {
            printf("UPDATE_TABLE[%d] : %s\n", i, query->from_tables[i]);
        }
        for (int i = 0; i < query->update_count; i++) {
            printf("UPDATE_FIELD[%d] : %s\n", i, query->update_fields[i]);
            printf("UPDATE_VALUE[%d] : %s\n", i, query->update_values[i]);
        }
        for (int i = 0; i < query->where_count; i++) {
            printf("WHERE_CONDITION[%d] : %s\n", i, query->where_conditions[i]);
        }
    }

    // SECTION INSERT
    if (query->insert_count > 0) {
        for (int i = 0; i < query->from_count; i++) {
            printf("INSERT_TABLE[%d] : %s\n", i, query->from_tables[i]);
        }
        for (int i = 0; i < query->insert_count; i++) {
            printf("INSERT_FIELD[%d] : %s\n", i, query->insert_fields[i]);
            printf("INSERT_VALUE[%d] : %s\n", i, query->insert_values[i]);
        }
    }

    // SECTION CREATE TABLE
    if (query->create_table_count > 0) {
        for (int i = 0; i < query->from_count; i++) {
            printf("CREATE_TABLE[%d] : %s\n", i, query->from_tables[i]);
        }
        for (int i = 0; i < query->create_table_count; i++) {
            printf("CREATE_TABLE_DEFINITION[%d] : %s\n", i, query->create_table_definitions[i]);
        }
    }
}

void free_query(SQL_QUERY* query) {
    if (!query) return;

    for (int i = 0; i < query->select_count; i++) free(query->select_fields[i]);
    free(query->select_fields);

    for (int i = 0; i < query->from_count; i++) free(query->from_tables[i]);
    free(query->from_tables);

    for (int i = 0; i < query->where_count; i++) free(query->where_conditions[i]);
    free(query->where_conditions);

    for (int i = 0; i < query->groupby_count; i++) free(query->groupby_fields[i]);
    free(query->groupby_fields);

    for (int i = 0; i < query->having_count; i++) free(query->having_conditions[i]);
    free(query->having_conditions);

    for (int i = 0; i < query->update_count; i++) {
        free(query->update_fields[i]);
        free(query->update_values[i]);
    }
    free(query->update_fields);
    free(query->update_values);

    for (int i = 0; i < query->delete_count; i++) free(query->delete_tables[i]);
    free(query->delete_tables);

    for (int i = 0; i < query->insert_count; i++) {
        free(query->insert_fields[i]);
        free(query->insert_values[i]);
    }
    free(query->insert_fields);
    free(query->insert_values);

    for (int i = 0; i < query->create_table_count; i++) free(query->create_table_definitions[i]);
    free(query->create_table_definitions);

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

        // Add to SELECT fields
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

    printf("Debug: Processing GROUP BY content: %s\n", content);

    while (content[i]) {
        if (content[i] == ',' || content[i] == '\0') {
            field[field_idx] = '\0';
            printf("Debug: Extracted field: %s\n", field);
            add_groupby(query, field);
            field_idx = 0;
        } else if (!isspace(content[i])) {
            field[field_idx++] = content[i];
        }
        i++;
    }

    if (field_idx > 0) {
        field[field_idx] = '\0';
        printf("Debug: Extracted last field: %s\n", field);
        add_groupby(query, field);
    }

    printf("Debug: Final GROUP BY fields:\n");
    for (int j = 0; j < query->groupby_count; j++) {
        printf("  %s\n", query->groupby_fields[j]);
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
    char* orderby_clause = strdup(content);  // Duplicate content for modification
    if (!orderby_clause) {
        fprintf(stderr, "Erreur d'allocation mémoire pour ORDER BY.\n");
        return;
    }

    query->order_data.orderby_fields = malloc(sizeof(char*) * MAX_ORDERBY_FIELDS);
    query->order_data.orderby_directions = malloc(sizeof(char*) * MAX_ORDERBY_FIELDS);
    query->order_data.orderby_count = 0;

    if (!query->order_data.orderby_fields || !query->order_data.orderby_directions) {
        fprintf(stderr, "Erreur d'allocation mémoire pour les données ORDER BY.\n");
        free(orderby_clause);
        return;
    }

    char* token = strtok(orderby_clause, ",");
    while (token && query->order_data.orderby_count < MAX_ORDERBY_FIELDS) {
        char* trimmed_token = trim_whitespace(token);

        // Determine direction
        char* direction = "ASC";  // Default to ascending
        if (strstr(trimmed_token, "DESC")) {
            direction = "DESC";
            *strstr(trimmed_token, "DESC") = '\0';  // Remove DESC
        } else if (strstr(trimmed_token, "ASC")) {
            *strstr(trimmed_token, "ASC") = '\0';  // Remove ASC
        }

        // Add field and direction
        query->order_data.orderby_fields[query->order_data.orderby_count] = strdup(trim_whitespace(trimmed_token));
        query->order_data.orderby_directions[query->order_data.orderby_count] = strdup(direction);
        query->order_data.orderby_count++;

        token = strtok(NULL, ",");
    }

    free(orderby_clause);
}
char* trim_whitespace(char* str) {
    if (!str) return NULL;

    // Enlever les espaces en début
    while (isspace((unsigned char)*str)) str++;

    // Enlever les espaces en fin
    char* end = str + strlen(str) - 1;
    while (end > str && isspace((unsigned char)*end)) end--;

    *(end + 1) = '\0'; // Terminer la chaîne
    return str;
}
char* extract_token(const char* input, char* token, char delimiter) {
    const char* start = input;
    char* dest = token;
    int in_quotes = 0;

    while (*start) {
        if (*start == '\'' || *start == '\"') {
            if (!in_quotes) {
                in_quotes = 1; // Commencer la gestion des guillemets
                *dest++ = *start; // Inclure le guillemet ouvrant
            } else {
                *dest++ = *start; // Inclure le guillemet fermant
                in_quotes = 0; // Terminer la gestion des guillemets
            }
        } else if (*start == delimiter && !in_quotes) {
            start++; // Sauter le délimiteur
            break;
        } else {
            *dest++ = *start;
        }
        start++;
    }

    *dest = '\0'; // Terminer la chaîne
    return (char*)start;
}

void process_delete_section(const char* query_str, SQL_QUERY* query) {
    if (!query || !query_str) {
        fprintf(stderr, "Error: NULL query or query_str in process_delete_section.\n");
        return;
    }

    const char* from_section = strstr(query_str, "FROM");
    if (!from_section) {
        fprintf(stderr, "Error: DELETE query missing FROM clause.\n");
        return;
    }

    char table_name[128];
    sscanf(from_section + 5, "%127s", table_name);
    add_delete(query, table_name);

    const char* where_section = strstr(query_str, "WHERE");
    if (where_section) {
        char where_condition[256] = {0};
        strncpy(where_condition, where_section + 5, strlen(where_section + 5));
        add_where(query, trim_whitespace(where_condition));
    }
}
void process_update_section(const char* query_str, SQL_QUERY* query) {
    if (!query_str || !query) {
        fprintf(stderr, "Error: NULL input to process_update_section.\n");
        return;
    }

    const char* set_section = strstr(query_str, "SET");
    const char* where_section = strstr(query_str, "WHERE");

    if (!set_section) {
        fprintf(stderr, "Error: UPDATE query missing SET clause.\n");
        return;
    }

    // Extraire le nom de la table
    char table_name[128] = {0};
    sscanf(query_str, "UPDATE %127s", table_name);

    query->from_tables = realloc(query->from_tables, sizeof(char*));
    if (!query->from_tables) {
        perror("Memory allocation error in process_update_section");
        exit(EXIT_FAILURE);
    }
    query->from_tables[0] = strdup(table_name);
    query->from_count = 1;

    // Analyse des champs SET
    char field[128] = {0}, value[128] = {0};
    const char* current = set_section + 4;  // Position après "SET "

    while (current && (!where_section || current < where_section)) {
        char* next_comma = strchr(current, ',');
        if (where_section && (!next_comma || next_comma > where_section)) {
            next_comma = NULL; // Ne pas aller au-delà de WHERE
        }

        if (next_comma) {
            char temp[256] = {0};
            strncpy(temp, current, next_comma - current);
            if (sscanf(temp, "%127[^=]=%127s", field, value) == 2) {
                add_update(query, trim_whitespace(field), trim_whitespace(value));
            }
            current = next_comma + 1; // Passer au prochain champ
        } else {
            if (sscanf(current, "%127[^=]=%127s", field, value) == 2) {
                add_update(query, trim_whitespace(field), trim_whitespace(value));
            }
            break; // Fin des champs SET
        }
    }

    // Traiter WHERE
    if (where_section) {
        char where_condition[256] = {0};
        strncpy(where_condition, where_section + 5, sizeof(where_condition) - 1);
        add_where(query, trim_whitespace(where_condition));
    }
}
void process_insert_section(const char* query_str, SQL_QUERY* query) {
    if (!query_str || !query) {
        fprintf(stderr, "Error: NULL input to process_insert_section.\n");
        return;
    }

    const char* fields_start = strchr(query_str, '(');
    const char* values_start = strstr(query_str, "VALUES");

    if (!fields_start || !values_start) {
        fprintf(stderr, "Error: INSERT query missing fields or VALUES clause.\n");
        return;
    }

    // Extraire le nom de la table
    char table_name[128] = {0};
    sscanf(query_str, "INSERT INTO %127s", table_name);

    add_from(query, table_name);

    // Extraire les champs
    char fields[512] = {0};
    const char* fields_end = strchr(fields_start, ')');
    if (!fields_end) {
        fprintf(stderr, "Error: Invalid INSERT fields syntax.\n");
        return;
    }
    strncpy(fields, fields_start + 1, fields_end - (fields_start + 1));
    fields[fields_end - (fields_start + 1)] = '\0';

    // Extraire les valeurs
    char values[512] = {0};
    const char* values_start_paren = strchr(values_start, '(');
    const char* values_end = strchr(values_start_paren, ')');
    if (!values_start_paren || !values_end) {
        fprintf(stderr, "Error: Invalid INSERT values syntax.\n");
        return;
    }
    strncpy(values, values_start_paren + 1, values_end - (values_start_paren + 1));
    values[values_end - (values_start_paren + 1)] = '\0';

    // Parsing manuel des champs et des valeurs
    const char* field_ptr = fields;
    const char* value_ptr = values;

    char field[128] = {0};
    char value[128] = {0};

    while (*field_ptr && *value_ptr) {
        field_ptr = extract_token(field_ptr, field, ',');
        value_ptr = extract_token(value_ptr, value, ',');

        add_insert(query, trim_whitespace(field), trim_whitespace(value));
    }

    if (*field_ptr || *value_ptr) {
        fprintf(stderr, "Error: Mismatched fields and values in INSERT query.\n");
    }
}
void process_create_table_section(const char* query_str, SQL_QUERY* query) {
    if (!query_str || !query) {
        fprintf(stderr, "Error: NULL input to process_create_table_section.\n");
        return;
    }

    const char* table_start = strstr(query_str, "TABLE");
    const char* fields_start = strchr(query_str, '(');

    if (!table_start || !fields_start) {
        fprintf(stderr, "Error: CREATE TABLE syntax invalid.\n");
        return;
    }

    // Extraire le nom de la table
    char table_name[128] = {0};
    sscanf(table_start + 6, "%127s", table_name);

    add_from(query, table_name);

    // Extraire les définitions des colonnes
    char fields[1024] = {0};
    const char* fields_end = strrchr(query_str, ')');
    if (!fields_end || fields_end <= fields_start) {
        fprintf(stderr, "Error: Invalid CREATE TABLE column definitions.\n");
        return;
    }
    strncpy(fields, fields_start + 1, fields_end - (fields_start + 1));
    fields[fields_end - (fields_start + 1)] = '\0';

    // Parsing manuel des colonnes
    const char* column_ptr = fields;
    char column[256] = {0};

    while (*column_ptr) {
        column_ptr = extract_token(column_ptr, column, ',');
        if (strlen(column) > 0) {
            add_create_table(query, trim_whitespace(column));
        }
    }
}

void parse_query(const char* query_str, SQL_QUERY* query) {
        if (strstr(query_str, "DELETE FROM")) {
        process_delete_section(query_str, query);
    } else if (strstr(query_str, "UPDATE")) {
        process_update_section(query_str, query);
    } else if (strstr(query_str, "INSERT INTO")) {
        process_insert_section(query_str, query);
    } else if (strstr(query_str, "CREATE TABLE")) {
        process_create_table_section(query_str, query);
    } else{
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

}
void test_delete_query() {
    SQL_QUERY* query = init_query();

    const char* test_query = "DELETE FROM users WHERE age > 30";
    parse_query(test_query, query);

    assert(query->delete_count == 1);
    assert(strcmp(query->delete_tables[0], "users") == 0);
    assert(query->where_count == 1);
    assert(strcmp(query->where_conditions[0], "age > 30") == 0);

    free_query(query);
    printf("DELETE query test passed.\n");
}
void test_update_query() {
    printf("Running UPDATE query test...\n");
    SQL_QUERY* query = init_query();

    const char* test_query = "UPDATE products SET price = 19.99, stock = 50 WHERE product_id = 1001";
    parse_query(test_query, query);

    assert(query->update_count == 2);
    assert(strcmp(query->update_fields[0], "price") == 0);
    assert(strcmp(query->update_values[0], "19.99") == 0);
    assert(strcmp(query->update_fields[1], "stock") == 0);
    assert(strcmp(query->update_values[1], "50") == 0);

    assert(query->where_count == 1);
    assert(strcmp(query->where_conditions[0], "product_id = 1001") == 0);

    free_query(query);
    printf("UPDATE query test passed.\n");
}
void test_insert_query() {
    printf("Running INSERT query test...\n");
    SQL_QUERY* query = init_query();

    const char* test_query = "INSERT INTO employees (name, position, salary) VALUES ('John Doe', 'Manager', '75000')";
    parse_query(test_query, query);

    printf("Debug: insert_count = %d\n", query->insert_count);
    for (int i = 0; i < query->insert_count; i++) {
        printf("Debug: Field = '%s', Value = '%s'\n", query->insert_fields[i], query->insert_values[i]);
    }

    assert(query->insert_count == 3);
    assert(strcmp(query->insert_fields[0], "name") == 0);
    assert(strcmp(query->insert_values[0], "'John Doe'") == 0);
    assert(strcmp(query->insert_fields[1], "position") == 0);
    assert(strcmp(query->insert_values[1], "'Manager'") == 0);
    assert(strcmp(query->insert_fields[2], "salary") == 0);
    assert(strcmp(query->insert_values[2], "'75000'") == 0);

    free_query(query);
    printf("INSERT query test passed.\n");
}
// Test CREATE TABLE
void test_create_table_query() {
    printf("Running CREATE TABLE query test...\n");
    SQL_QUERY* query = init_query();

    const char* test_query = "CREATE TABLE customers (id INT PRIMARY KEY, name VARCHAR(100), email VARCHAR(100))";
    parse_query(test_query, query);

    assert(query->create_table_count == 1);
    assert(strcmp(query->create_table_definitions[0], "id INT PRIMARY KEY, name VARCHAR(100), email VARCHAR(100)") == 0);

    free_query(query);
    printf("CREATE TABLE query test passed.\n");
}


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
}
/*
int main() {
    test_delete_query();
    test_update_query();
    test_insert_query();
    test_create_table_query();
    printf("All tests passed successfully!\n");
    return 0;
}*/