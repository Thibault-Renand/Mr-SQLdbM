#include "newSQL.h"
#include "testsql.h"
#include "database_manager.h"

int main() {
    printf("Bienvenue dans l'interpréteur SQL !\n");

    while (1) {
        // Step 1: Input SQL Query
        printf("\nEntrez votre requête SQL (ou tapez 'exit' pour quitter) :\n");
        char* input_query = read_query();
        if (!input_query || strcmp(input_query, "exit") == 0) {
            free(input_query);
            break;
        }

        // Step 2: Parse the Query
        SQL_QUERY* query = init_query();
        parse_query(input_query, query);
        free(input_query);
        const char* filename = query->from_tables[0];
        char nom_temporaire[20];
        snprintf(nom_temporaire, 20, "%s.bin", filename);
        const char* tempFilename = nom_temporaire;        
        //const char* query->from_tables[0]=strcat(query->from_tables[0],".bin");
        printf("\n%s\n",query->from_tables[0]);
        if (query->select_count > 0){




            // Validate Query
            if (query->from_count == 0) {
                printf("Erreur : La clause FROM est obligatoire.\n");
                free_query(query);
                continue;
            }

            // Step 3: Load Table Data
            Header* header = malloc(sizeof(Header)); // Declare and allocate Header
            int record_count = 0;                    // Declare record_count
            Record* records = from(header, query->from_tables[0], &record_count);
            if (!records) {
                printf("Erreur : Impossible de charger la table '%s'.\n", query->from_tables[0]);
                free(header);
                free_query(query);
                continue;
            }

            // Step 4: Apply WHERE Conditions
            if (query->where_count > 0) {
                records = apply_where(header, records, &record_count, query->where_conditions, query->where_count);
            }

                // Step 5: Handle GROUP BY with Aggregates
            AggregatedResult* aggregates = NULL;
            int result_count = 0;
            if (query->groupby_count > 0) {
                aggregates = apply_groupby_with_aggregates(
                    header, records, &record_count,
                    query->groupby_fields, query->groupby_count,
                    query->aggregate_functions, query->aggregate_count,
                    &result_count
                );
                if (!aggregates) {
                    printf("Erreur : Problème lors du traitement de GROUP BY.\n");
                    goto cleanup;
                }
            }

            // Step 6: Apply HAVING
            if (query->having_count > 0 && aggregates != NULL) {
                aggregates = apply_having(aggregates, &result_count, query->having_conditions, query->having_count);
            }

            // Step 7: Apply ORDER BY
            if (query->order_data.orderby_count > 0) {
                if (aggregates) {
                    apply_orderby_with_aggregates(
                        aggregates, result_count,
                        query->order_data.orderby_fields,
                        query->order_data.orderby_directions,
                        query->order_data.orderby_count
                    );
                } else {
                    apply_orderby(
                        header, records, record_count,
                        query->order_data.orderby_fields,
                        query->order_data.orderby_directions,
                        query->order_data.orderby_count
                    );
                }
            }

            // Step 8: Apply LIMIT and OFFSET
            if (aggregates) {
                if (query->offset > 0) {
                    aggregates = apply_offset_aggregates(aggregates, &result_count, query->offset);
                }
                if (query->limit > 0) {
                    aggregates = apply_limit_aggregates(aggregates, &result_count, query->limit);
                }
            } else {
                if (query->offset > 0) {
                    records = apply_offset(records, &record_count, query->offset);
                }
                if (query->limit > 0) {
                    records = apply_limit(records, &record_count, query->limit);
                }
            }

            // Step 9: Display Results
            if (aggregates) {
                apply_select_on_aggregates(aggregates, result_count, query->select_fields, query->select_count);
                free(aggregates);
            } else if (record_count > 0) {
                apply_select(header, records, record_count, query->select_fields, query->select_count);
            } else {
                printf("Aucun résultat à afficher.\n");
            }
                cleanup:
        // Cleanup memory
        for (int i = 0; i < record_count; i++) {
            for (int j = 0; j < header->nbColumns; j++) {
                free(records[i].fields[j]);
            }
            free(records[i].fields);
        }
        free(records);
        free(header->columns);
        free(header);
        free_query(query);
    }
        if (query->create_table_count > 0){
                printf("ABDUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUL");
            Record info;
            info.fields = malloc(20 * sizeof(char*)); // Allocate memory for fields
            if (!info.fields) {
                perror("Memory allocation error for fields");
                exit(EXIT_FAILURE);
            }

                for(int i=0;i<query->create_table_count;i++){
                info.fields[i] = malloc(strlen(query->create_table_definitions[i]) + 1); // Allocate memory for each field
                if (!info.fields[i]) {
                    perror("Memory allocation error for field");
                    exit(EXIT_FAILURE);
                }
                strcpy(info.fields[i], query->create_table_definitions[i]);

                }
            info.fields[query->create_table_count] = NULL; // Terminate with NULL
            createTable(tempFilename, &info);


            // Chargement du fichier pour vérifier le contenu
            Header* header = loadFileHeader(tempFilename);
            if (!header) {
                printf("Erreur lors du chargement du fichier de table.\n");
                return 1;
            }

            // Affichage des informations sur la table
            displayHeader(header);
        }
        if (query->insert_count > 0) {
            Record insertInfo;
            insertInfo.fields = malloc((query->insert_count + 1) * sizeof(char*)); // Allocate memory for fields
            if (!insertInfo.fields) {
                perror("Memory allocation error for insert fields");
                exit(EXIT_FAILURE);
            }

            for (int i = 0; i < query->insert_count; i++) {
                insertInfo.fields[i] = malloc(strlen(query->insert_fields[i]) + strlen(query->insert_values[i]) + 2); // Allocate memory for each field
                if (!insertInfo.fields[i]) {
                    perror("Memory allocation error for insert field");
                    exit(EXIT_FAILURE);
                }
            sprintf(insertInfo.fields[i], "%s %s", query->insert_fields[i], query->insert_values[i]); // Construct the string
            }
            insertInfo.fields[query->insert_count] = NULL; // Terminate with NULL
            insertInto(tempFilename, &insertInfo);

            // Cleanup memory for insert fields
            for (int i = 0; i < query->insert_count; i++) {
                free(insertInfo.fields[i]);
            }
            free(insertInfo.fields);
        }
        if (query->alter_table_columns_count > 0) {
            Record alterInfo;
            alterInfo.fields = malloc((query->alter_table_columns_count + 1) * sizeof(char*)); // Allocate memory for fields
            if (!alterInfo.fields) {
                perror("Memory allocation error for alter fields");
                exit(EXIT_FAILURE);
            }

            for (int i = 0; i < query->alter_table_columns_count; i++) {
                alterInfo.fields[i] = malloc(strlen(query->alter_table_columns[i]) + strlen(query->alter_table_definitions[i]) + 2); // Allocate memory for each field
               if (!alterInfo.fields[i]) {
                   perror("Memory allocation error for alter field");
                    exit(EXIT_FAILURE);
                }
               sprintf(alterInfo.fields[i], "%s %s", query->alter_table_columns[i], query->alter_table_definitions[i]); // Construct the string
            }
            alterInfo.fields[query->alter_table_columns_count] = NULL; // Terminate with NULL
            // Call the appropriate function to alter the table
            alterTableAdd(tempFilename, &alterInfo);

            // Cleanup memory for alter fields
            for (int i = 0; i < query->alter_table_columns_count; i++) {
                free(alterInfo.fields[i]);
            }
            free(alterInfo.fields);
        }
        if (query->delete_count > 0) {
            Record deleteInfo;
            deleteInfo.fields = malloc(2 * sizeof(char*)); // Allocate memory for one condition and NULL
            if (!deleteInfo.fields) {
                perror("Memory allocation error for delete fields");
                 exit(EXIT_FAILURE);
            }

            deleteInfo.fields[0] = malloc(strlen(query->where_conditions[0]) + 1); // Allocate memory for the condition
            if (!deleteInfo.fields[0]) {
                perror("Memory allocation error for delete field");
                exit(EXIT_FAILURE);
            }
            strcpy(deleteInfo.fields[0], query->where_conditions[0]); // Copy the condition
            deleteInfo.fields[1] = NULL; // Terminate with NULL

            deleteRecord(query->from_tables[0], &deleteInfo);

            // Cleanup memory for delete fields
            free(deleteInfo.fields[0]);
            free(deleteInfo.fields);
        }
        if(query->update_count>0){
            printf("abdul\n");
            Record updateInfo;
            strcpy(updateInfo.fields[0],strcat(query->update_fields[0],strcat(" ",query->update_values[0])));
            strcpy(updateInfo.fields[1],query->where_conditions[0]);
            updateRecord(query->from_tables[0], &updateInfo);

            displayRecords(query->from_tables[0]);
        }

    }
    printf("Au revoir !\n");
    return 0;
}
