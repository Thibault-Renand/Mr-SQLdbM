#include "testsql.h"
#include "newSQL.h"
#include <string.h>
static OrderData* global_order_data = NULL;

int sum(Record* group, int group_size, int column_index) {
    int total = 0;
    for (int i = 0; i < group_size; i++) {
        total += atoi(group[i].fields[column_index]);
    }
    return total;
}
float avg(Record* group, int group_size, int column_index) {
    if (group_size == 0) return 0;
    int total = sum(group, group_size, column_index);
    return (float)total / group_size;
}

int min(Record* group, int group_size, int column_index) {
    int minimum = atoi(group[0].fields[column_index]);
    for (int i = 1; i < group_size; i++) {
        int value = atoi(group[i].fields[column_index]);
        if (value < minimum) {
            minimum = value;
        }
    }
    return minimum;
}

int max(Record* group, int group_size, int column_index) {
    int maximum = atoi(group[0].fields[column_index]);
    for (int i = 1; i < group_size; i++) {
        int value = atoi(group[i].fields[column_index]);
        if (value > maximum) {
            maximum = value;
        }
    }
    return maximum;
}

Record* apply_where(Header* header, Record* records, int* record_count, char** conditions, int condition_count) {
    Record* filtered_records = malloc(*record_count * sizeof(Record));
    int filtered_count = 0;

    for (int i = 0; i < *record_count; i++) {
        int match = 1;

        for (int j = 0; j < condition_count; j++) {
            char field_name[20], operator[3], value[50];
            sscanf(conditions[j], "%s %s %s", field_name, operator, value);

            char* field_value = NULL;
            for (int k = 0; k < header->nbColumns; k++) {
                if (strcmp(header->columns[k].name, field_name) == 0) {
                    field_value = records[i].fields[k];
                    break;
                }
            }

            if (!field_value) {
                match = 0;
                break;
            }

            if (strcmp(operator, "=") == 0 && strcmp(field_value, value) != 0) match = 0;
            if (strcmp(operator, "!=") == 0 && strcmp(field_value, value) == 0) match = 0;
            if (strcmp(operator, "<") == 0 && atoi(field_value) >= atoi(value)) match = 0;
            if (strcmp(operator, ">") == 0 && atoi(field_value) <= atoi(value)) match = 0;

            if (!match) break;
        }

        if (match) {
            filtered_records[filtered_count++] = records[i];
        }
    }

    *record_count = filtered_count;
    return filtered_records;
}
Record* apply_groupby(Header* header, Record* records, int* record_count, char** groupby_fields, int groupby_count) {
    if (groupby_count == 0) {
        printf("Aucun champ spécifié pour GROUP BY.\n");
        return records;
    }

    // Structure pour stocker les groupes
    typedef struct {
        Record* records;
        int count;
    } Group;

    Group* groups = malloc(*record_count * sizeof(Group));
    int group_count = 0;

    // Parcourir tous les enregistrements
    for (int i = 0; i < *record_count; i++) {
        int found = 0;

        // Vérifier si l'enregistrement appartient déjà à un groupe
        for (int j = 0; j < group_count; j++) {
            int match = 1;

            for (int k = 0; k < groupby_count; k++) {
                int column_index = -1;

                // Trouver l'index de la colonne groupby_fields[k]
                for (int l = 0; l < header->nbColumns; l++) {
                    if (strcmp(header->columns[l].name, groupby_fields[k]) == 0) {
                        column_index = l;
                        break;
                    }
                }

                if (column_index == -1) {
                    printf("Erreur : Colonne '%s' non trouvée.\n", groupby_fields[k]);
                    free(groups);
                    return records;
                }

                // Comparer la valeur du champ
                if (strcmp(records[i].fields[column_index], groups[j].records[0].fields[column_index]) != 0) {
                    match = 0;
                    break;
                }
            }

            if (match) {
                // Ajouter l'enregistrement au groupe existant
                groups[j].records = realloc(groups[j].records, (groups[j].count + 1) * sizeof(Record));
                groups[j].records[groups[j].count++] = records[i];
                found = 1;
                break;
            }
        }

        if (!found) {
            // Créer un nouveau groupe
            groups[group_count].records = malloc(sizeof(Record));
            groups[group_count].records[0] = records[i];
            groups[group_count].count = 1;
            group_count++;
        }
    }

    // Afficher les groupes pour vérifier
    printf("\nGroupes détectés :\n");
    for (int i = 0; i < group_count; i++) {
        printf("Groupe %d (count=%d) :\n", i + 1, groups[i].count);
        for (int j = 0; j < groups[i].count; j++) {
            for (int k = 0; k < header->nbColumns; k++) {
                printf("  %s : %s ", header->columns[k].name, groups[i].records[j].fields[k]);
            }
            printf("\n");
        }
    }

    // Rassembler les enregistrements par groupes (un exemplaire par groupe)
    Record* grouped_records = malloc(group_count * sizeof(Record));
    for (int i = 0; i < group_count; i++) {
        grouped_records[i] = groups[i].records[0]; // Utilise le premier enregistrement comme représentant
        free(groups[i].records);
    }

    free(groups);

    *record_count = group_count;
    return grouped_records;
}

void apply_select(Header* header, Record* records, int record_count, char** select_fields, int select_count) {
    printf("Résultats de SELECT :\n");

    for (int i = 0; i < record_count; i++) {
        printf("Enregistrement %d : ", i + 1);

        for (int j = 0; j < select_count; j++) {
            for (int k = 0; k < header->nbColumns; k++) {
                if (strcmp(select_fields[j], header->columns[k].name) == 0) {
                    printf("%s: %s ", header->columns[k].name, records[i].fields[k]);
                }
            }
        }
        printf("\n");
    }
}
AggregatedResult* apply_groupby_with_aggregates(Header* header, Record* records, int* record_count,char** groupby_fields, int groupby_count,char** aggregate_fields, int aggregate_count,int* result_count) {
    if (groupby_count == 0) {
        printf("Aucun champ spécifié pour GROUP BY.\n");
        return NULL;
    }

    // Find indexes of GROUP BY columns
    int* groupby_column_indexes = malloc(groupby_count * sizeof(int));
    for (int k = 0; k < groupby_count; k++) {
        groupby_column_indexes[k] = -1;
        for (int l = 0; l < header->nbColumns; l++) {
            if (strcmp(header->columns[l].name, groupby_fields[k]) == 0) {
                groupby_column_indexes[k] = l;
                break;
            }
        }
        if (groupby_column_indexes[k] == -1) {
            printf("Erreur : Colonne '%s' pour GROUP BY non trouvée.\n", groupby_fields[k]);
            free(groupby_column_indexes);
            return NULL;
        }
    }

    // Structure to store groups
    typedef struct {
        Record* records;
        int count;
    } Group;

    Group* groups = malloc(*record_count * sizeof(Group));
    int group_count = 0;

    // Group records
    for (int i = 0; i < *record_count; i++) {
        int found = 0;

        for (int j = 0; j < group_count; j++) {
            int match = 1;
            for (int k = 0; k < groupby_count; k++) {
                int column_index = groupby_column_indexes[k];
                if (strcmp(records[i].fields[column_index], groups[j].records[0].fields[column_index]) != 0) {
                    match = 0;
                    break;
                }
            }

            if (match) {
                groups[j].records = realloc(groups[j].records, (groups[j].count + 1) * sizeof(Record));
                groups[j].records[groups[j].count++] = records[i];
                found = 1;
                break;
            }
        }

        if (!found) {
            groups[group_count].records = malloc(sizeof(Record));
            groups[group_count].records[0] = records[i];
            groups[group_count].count = 1;
            group_count++;
        }
    }

    // Compute aggregates for each group
    AggregatedResult* results = malloc(group_count * sizeof(AggregatedResult));
    for (int i = 0; i < group_count; i++) {
        results[i].group_value = strdup(groups[i].records[0].fields[groupby_column_indexes[0]]);
        results[i].count = groups[i].count;
        results[i].sum = 0;
        results[i].min = INT_MAX;
        results[i].max = INT_MIN;

        for (int j = 0; j < groups[i].count; j++) {
            for (int a = 0; a < aggregate_count; a++) {
                char field_name[20], func_name[10];
                sscanf(aggregate_fields[a], "%[^(](%[^)])", func_name, field_name);

                int agg_column_index = -1;
                for (int k = 0; k < header->nbColumns; k++) {
                    if (strcmp(header->columns[k].name, field_name) == 0) {
                        agg_column_index = k;
                        break;
                    }
                }

                if (agg_column_index == -1) {
                    printf("Erreur : Colonne '%s' pour agrégation non trouvée.\n", field_name);
                    continue;
                }

                int value = atoi(groups[i].records[j].fields[agg_column_index]);
                if (strcmp(func_name, "MIN") == 0) results[i].min = (value < results[i].min) ? value : results[i].min;
                if (strcmp(func_name, "MAX") == 0) results[i].max = (value > results[i].max) ? value : results[i].max;
                if (strcmp(func_name, "SUM") == 0) results[i].sum += value;
            }
        }

        results[i].avg = (float)results[i].sum / results[i].count;
    }

    // Free groups
    for (int i = 0; i < group_count; i++) {
        free(groups[i].records);
    }
    free(groups);
    free(groupby_column_indexes);

    *result_count = group_count;
    return results;
}
Record* from(Header* header, char* from_table, int* record_count) {
    // Construire le nom du fichier binaire
    char filename[256];
    snprintf(filename, sizeof(filename), "%s.bin", from_table);

    FILE* file = fopen(filename, "rb");
    if (!file) {
        fprintf(stderr, "Erreur : Impossible d'ouvrir le fichier %s\n", filename);
        return NULL;
    }

    // Lire le header
    if (fread(header, sizeof(Header), 1, file) != 1) {
        fprintf(stderr, "Erreur : Impossible de lire le header depuis %s\n", filename);
        fclose(file);
        return NULL;
    }

    // Allouer de l'espace pour les colonnes
    header->columns = malloc(header->nbColumns * sizeof(Column));
    if (fread(header->columns, sizeof(Column), header->nbColumns, file) != (size_t)header->nbColumns) {
        fprintf(stderr, "Erreur : Impossible de lire les colonnes depuis %s\n", filename);
        fclose(file);
        free(header->columns);
        return NULL;
    }

    // Déboguer le header
    printf("Header chargé : nbRecords=%d, nbColumns=%d, lengthRecord=%d\n",
           header->nbRecords, header->nbColumns, header->lengthRecord);

    // Allouer de l'espace pour les enregistrements
    *record_count = header->nbRecords;
    Record* records = malloc(header->nbRecords * sizeof(Record));

    for (int i = 0; i < header->nbRecords; i++) {
        records[i].fields = malloc(header->nbColumns * sizeof(char*));

        char record[header->lengthRecord];
        if (fread(record, header->lengthRecord, 1, file) != 1) {
            fprintf(stderr, "Erreur : Impossible de lire un enregistrement complet\n");
            fclose(file);
            for (int k = 0; k <= i; k++) {
                for (int l = 0; l < header->nbColumns; l++) {
                    free(records[k].fields[l]);
                }
                free(records[k].fields);
            }
            free(records);
            free(header->columns);
            return NULL;
        }

        // Extraire les champs depuis le record lu
        int offset = 0;
        for (int j = 0; j < header->nbColumns; j++) {
            records[i].fields[j] = malloc(header->columns[j].length + 1);
            if (strcmp(header->columns[j].type, "VARCHAR") == 0) {
                strncpy(records[i].fields[j], record + offset, header->columns[j].length);
                records[i].fields[j][header->columns[j].length] = '\0'; // Null-terminate
                offset += header->columns[j].length;
            } else if (strcmp(header->columns[j].type, "INT") == 0) {
                snprintf(records[i].fields[j], header->columns[j].length + 1, "%d", *(int*)(record + offset));
                offset += sizeof(int);
            }
        }
    }

    fclose(file);
    return records;
}
AggregatedResult* apply_having(AggregatedResult* results, int* result_count, char** having_conditions, int having_count) {
    AggregatedResult* filtered_results = malloc(*result_count * sizeof(AggregatedResult));
    int filtered_count = 0;

    for (int i = 0; i < *result_count; i++) {
        int match = 1;

        for (int j = 0; j < having_count; j++) {
            char field[50], operator[3], value[50];
            sscanf(having_conditions[j], "%s %s %s", field, operator, value);

            int compare_value = 0;

            // Vérifier uniquement les colonnes des groupes
            if (strcmp(field, "AVG") == 0) compare_value = (int)results[i].avg;
            else if (strcmp(field, "COUNT") == 0) compare_value = results[i].count;
            else if (strcmp(field, "SUM") == 0) compare_value = results[i].sum;
            else if (strcmp(field, "MIN") == 0) compare_value = results[i].min;
            else if (strcmp(field, "MAX") == 0) compare_value = results[i].max;
            else {
                printf("Erreur : Champ '%s' inconnu dans HAVING.\n", field);
                match = 0;
                break;
            }

            int value_int = atoi(value);

            // Appliquer le filtre basé sur l'opérateur
            if (strcmp(operator, "=") == 0 && compare_value != value_int) match = 0;
            if (strcmp(operator, "!=") == 0 && compare_value == value_int) match = 0;
            if (strcmp(operator, "<") == 0 && compare_value >= value_int) match = 0;
            if (strcmp(operator, ">") == 0 && compare_value <= value_int) match = 0;
            if (strcmp(operator, "<=") == 0 && compare_value > value_int) match = 0;
            if (strcmp(operator, ">=") == 0 && compare_value < value_int) match = 0;

            if (!match) break;
        }

        if (match) {
            filtered_results[filtered_count++] = results[i];
        }
    }

    *result_count = filtered_count;
    return filtered_results;
}
void apply_select_on_aggregates(AggregatedResult* results, int result_count, char** select_fields, int select_count) {
    printf("Résultats de la requête complexe :\n");

    for (int i = 0; i < result_count; i++) {
        printf("Groupe : %s\n", results[i].group_value);

        for (int j = 0; j < select_count; j++) {
            if (strstr(select_fields[j], "MAX(")) {
                printf("  %s : %d\n", select_fields[j], results[i].max);
            } else if (strstr(select_fields[j], "MIN(")) {
                printf("  %s : %d\n", select_fields[j], results[i].min);
            } else if (strstr(select_fields[j], "SUM(")) {
                printf("  %s : %d\n", select_fields[j], results[i].sum);
            } else if (strstr(select_fields[j], "AVG(")) {
                printf("  %s : %.2f\n", select_fields[j], results[i].avg);
            } else if (strcmp(select_fields[j], "COUNT") == 0) {
                printf("  %s : %d\n", select_fields[j], results[i].count);
            } else {
                // Non-aggregate fields
                printf("  %s : %s\n", select_fields[j], results[i].group_value);
            }
        }
    }
}
int compare_aggregates(const void* a, const void* b) {
    AggregatedResult* agg_a = (AggregatedResult*)a;
    AggregatedResult* agg_b = (AggregatedResult*)b;

    for (int i = 0; i < global_order_data->orderby_count; i++) {
        char* field = global_order_data->orderby_fields[i];
        char* direction = global_order_data->orderby_directions[i];
        int cmp = 0;

        // Comparaison des valeurs
        if (strcmp(field, "COUNT") == 0) {
            cmp = (agg_a->count - agg_b->count);
        } else if (strcmp(field, "SUM") == 0) {
            cmp = (agg_a->sum - agg_b->sum);
        } else if (strcmp(field, "AVG") == 0) {
            double avg_a = (agg_a->count == 0) ? 0 : (double)agg_a->sum / agg_a->count;
            double avg_b = (agg_b->count == 0) ? 0 : (double)agg_b->sum / agg_b->count;
            cmp = (avg_a > avg_b) - (avg_a < avg_b);
        } else if (strcmp(field, agg_a->group_value) == 0) {
            cmp = strcmp(agg_a->group_value, agg_b->group_value);
        }

        // Appliquer la direction ASC/DESC
        if (strcmp(direction, "DESC") == 0) {
            cmp = -cmp;
        }

        // Si un champ donne un résultat, on retourne
        if (cmp != 0) {
            return cmp;
        }
    }

    return 0;
}

// Fonction pour trier les résultats agrégés
void apply_orderby_with_aggregates(AggregatedResult* results, int result_count, char** orderby_fields, char** orderby_directions, int orderby_count) {
    if (result_count <= 1) {
        return; // Rien à trier si un seul groupe
    }

    // Préparer les données pour le tri
    OrderData order_data = {
        .orderby_fields = orderby_fields,
        .orderby_directions = orderby_directions,
        .orderby_count = orderby_count
    };

    // Passer les données dans la variable globale temporaire
    global_order_data = &order_data;

    // Tri des résultats agrégés
    qsort(results, result_count, sizeof(AggregatedResult), compare_aggregates);

    // Nettoyer la variable globale
    global_order_data = NULL;
}
void apply_orderby(Header* header, Record* records, int record_count, char** orderby_fields, char** orderby_directions, int orderby_count) {
    for (int i = 0; i < record_count - 1; i++) {
        for (int j = i + 1; j < record_count; j++) {
            int comparison = 0;

            for (int k = 0; k < orderby_count; k++) {
                char* field = orderby_fields[k];
                char* direction = orderby_directions[k];

                int column_index = -1;

                // Trouver l'index de la colonne à trier
                for (int l = 0; l < header->nbColumns; l++) {
                    if (strcmp(header->columns[l].name, field) == 0) {
                        column_index = l;
                        break;
                    }
                }

                if (column_index == -1) {
                    printf("Erreur : Colonne '%s' non trouvée pour ORDER BY.\n", field);
                    return;
                }

                // Comparer les valeurs des enregistrements
                if (strcmp(header->columns[column_index].type, "INT") == 0) {
                    int value1 = atoi(records[i].fields[column_index]);
                    int value2 = atoi(records[j].fields[column_index]);
                    comparison = value1 - value2;
                } else {
                    comparison = strcmp(records[i].fields[column_index], records[j].fields[column_index]);
                }

                if (strcmp(direction, "DESC") == 0) {
                    comparison = -comparison;
                }

                // Si les enregistrements ne sont pas égaux, on arrête ici
                if (comparison != 0) {
                    break;
                }
            }

            // Échanger les enregistrements si nécessaire
            if (comparison > 0) {
                Record temp = records[i];
                records[i] = records[j];
                records[j] = temp;
            }
        }
    }
}
Record* apply_limit(Record* records, int* record_count, int limit) {
    if (limit <= 0 || limit >= *record_count) {
        return records; // Aucun changement si la limite dépasse ou est invalide
    }

    Record* limited_records = malloc(limit * sizeof(Record));

    for (int i = 0; i < limit; i++) {
        limited_records[i] = records[i];
    }

    *record_count = limit; // Ajuster le nombre de résultats
    return limited_records;
}
Record* apply_offset(Record* records, int* record_count, int offset) {
    if (offset >= *record_count || offset < 0) {
        *record_count = 0; // Aucun résultat si l'offset dépasse
        return NULL;
    }

    int new_count = *record_count - offset;
    Record* offset_records = malloc(new_count * sizeof(Record));

    for (int i = 0; i < new_count; i++) {
        offset_records[i] = records[offset + i];
    }

    *record_count = new_count; // Ajuster le nombre de résultats
    return offset_records;
}
void compute_single_aggregate(Header* header, Record* records, int record_count, const char* aggregate_function) {
    char field_name[20], func_name[10];
    sscanf(aggregate_function, "%[^(](%[^)])", func_name, field_name);

    int column_index = -1;
    for (int i = 0; i < header->nbColumns; i++) {
        if (strcmp(header->columns[i].name, field_name) == 0) {
            column_index = i;
            break;
        }
    }
    if (column_index == -1) {
        printf("Erreur : Colonne '%s' non trouvée.\n", field_name);
        return;
    }

    // Compute the aggregate
    if (strcmp(func_name, "MIN") == 0) {
        printf("MIN(%s): %d\n", field_name, min(records, record_count, column_index));
    } else if (strcmp(func_name, "MAX") == 0) {
        printf("MAX(%s): %d\n", field_name, max(records, record_count, column_index));
    } else if (strcmp(func_name, "SUM") == 0) {
        printf("SUM(%s): %d\n", field_name, sum(records, record_count, column_index));
    } else if (strcmp(func_name, "AVG") == 0) {
        printf("AVG(%s): %.2f\n", field_name, avg(records, record_count, column_index));
    } else {
        printf("Erreur : Fonction d'agrégation '%s' non prise en charge.\n", func_name);
    }
}
AggregatedResult* apply_limit_aggregates(AggregatedResult* results, int* result_count, int limit) {
    if (limit <= 0 || limit >= *result_count) {
        return results;
    }

    AggregatedResult* limited_results = malloc(limit * sizeof(AggregatedResult));
    memcpy(limited_results, results, limit * sizeof(AggregatedResult));
    *result_count = limit;
    return limited_results;
}

AggregatedResult* apply_offset_aggregates(AggregatedResult* results, int* result_count, int offset) {
    if (offset >= *result_count || offset < 0) {
        *result_count = 0;
        return NULL;
    }

    int new_count = *result_count - offset;
    AggregatedResult* offset_results = malloc(new_count * sizeof(AggregatedResult));
    memcpy(offset_results, results + offset, new_count * sizeof(AggregatedResult));
    *result_count = new_count;
    return offset_results;
}



/*
int main() {
    // Step 1: Initialize table metadata
    Column columns[] = {
        {"id", "INT", 4},
        {"nom", "VARCHAR", 20},
        {"ville", "VARCHAR", 20},
        {"age", "INT", 4}
    };

    Header* header = malloc(sizeof(Header));
    header->columns = malloc(4 * sizeof(Column));
    memcpy(header->columns, columns, 4 * sizeof(Column));
    header->nbColumns = 4;
    header->lengthRecord = 48;

    // Step 2: Populate the table with sample data
    Record* records = NULL;
    int record_count = 0;
    records = add_record(header, records, &record_count, (char*[]){"1", "Alice", "Paris", "30"});
    records = add_record(header, records, &record_count, (char*[]){"2", "Bob", "Lyon", "27"});
    records = add_record(header, records, &record_count, (char*[]){"3", "Charlie", "Paris", "22"});
    records = add_record(header, records, &record_count, (char*[]){"4", "David", "Paris", "35"});
    records = add_record(header, records, &record_count, (char*[]){"5", "Eve", "Lyon", "40"});
    records = add_record(header, records, &record_count, (char*[]){"6", "Frank", "Paris", "21"});
    records = add_record(header, records, &record_count, (char*[]){"7", "Grace", "Lyon", "50"});

    // Step 3: Query fields
    char* select_fields[] = {"MAX(age)", "ville"};
    int select_count = 2;

    char* groupby_fields[] = {"ville"};
    int groupby_count = 1;

    // Aggregate fields
    char* aggregate_fields[] = {"MAX(age)"};
    int aggregate_count = 1;

    // Step 4: Apply GROUP BY and calculate aggregates
    int result_count = 0;
    AggregatedResult* groupby_results = apply_groupby_with_aggregates(
        header, 
        records, 
        &record_count, 
        groupby_fields, 
        groupby_count, 
        aggregate_fields, 
        aggregate_count, 
        &result_count
    );

    if (groupby_results == NULL) {
        printf("Erreur : Impossible d'exécuter GROUP BY avec agrégation.\n");
        goto cleanup;
    }

    // Step 5: Display results
    apply_select_on_aggregates(groupby_results, result_count, select_fields, select_count);

cleanup:
    // Free allocated memory
    if (groupby_results != NULL) {
        for (int i = 0; i < result_count; i++) {
            free(groupby_results[i].group_value);
        }
        free(groupby_results);
    }

    for (int i = 0; i < record_count; i++) {
        for (int j = 0; j < header->nbColumns; j++) {
            free(records[i].fields[j]);
        }
        free(records[i].fields);
    }
    free(records);
    free(header->columns);
    free(header);

    return 0;
}
*/