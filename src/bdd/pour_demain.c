typedef struct SQL_QUERY {
    char** select_fields;   // Colonnes à sélectionner
    char** from_tables;     // Tables à charger
    char** where_conditions; // Conditions WHERE
    char** groupby_fields;  // Colonnes GROUP BY
    char** having_conditions; // Conditions HAVING
    char** orderby_fields;  // Colonnes ORDER BY
    char** orderby_directions; // ASC ou DESC pour chaque colonne
    int select_count;       // Nombre de colonnes SELECT
    int from_count;         // Nombre de tables FROM
    int where_count;        // Nombre de conditions WHERE
    int groupby_count;      // Nombre de colonnes GROUP BY
    int having_count;       // Nombre de conditions HAVING
    int orderby_count;      // Nombre de colonnes ORDER BY
} SQL_QUERY;

int main() {
    // Définir une requête
    SQL_QUERY query = {
        .select_fields = (char*[]){"MIN(age)"},
        .from_tables = (char*[]){"personne"},
        .where_conditions = (char*[]){"ville = Paris"},
        .groupby_fields = (char*[]){"ville"},
        .having_conditions = (char*[]){"MIN(age) > 20"},
        .orderby_fields = (char*[]){"age"},
        .orderby_directions = (char*[]){"ASC"},
        .select_count = 1,
        .from_count = 1,
        .where_count = 1,
        .groupby_count = 1,
        .having_count = 1,
        .orderby_count = 1
    };

    // Charger les données
    Header* header = malloc(sizeof(Header));
    int record_count = 0;
    Record* records = from(header, query.from_tables[0], &record_count);
    if (!records) {
        fprintf(stderr, "Erreur : Impossible de charger la table '%s'\n", query.from_tables[0]);
        free(header);
        return 1;
    }

    // Appliquer WHERE
    if (query.where_count > 0) {
        records = apply_where(header, records, &record_count, query.where_conditions, query.where_count);
    }

    // Appliquer GROUP BY
    if (query.groupby_count > 0) {
        AggregatedResult* results = NULL;
        int result_count = 0;

        results = apply_groupby_with_aggregates(header, records, &record_count, query.groupby_fields, query.groupby_count, "age", &result_count);

        // Appliquer HAVING
        if (query.having_count > 0) {
            results = apply_having(results, &result_count, query.having_conditions, query.having_count);
        }

        // Afficher les résultats GROUP BY
        printf("Résultats après GROUP BY et HAVING :\n");
        for (int i = 0; i < result_count; i++) {
            printf("Groupe : %s\n", results[i].group_value);
            printf("  COUNT : %d\n", results[i].count);
            printf("  SUM : %d\n", results[i].sum);
            printf("  AVG : %.2f\n", results[i].avg);
            printf("  MIN : %d\n", results[i].min);
            printf("  MAX : %d\n", results[i].max);
        }

        free(results); // Libérer les résultats agrégés
    }

    // Appliquer ORDER BY
    if (query.orderby_count > 0) {
        records = apply_orderby(header, records, record_count, query.orderby_fields, query.orderby_directions, query.orderby_count);
    }

    // Appliquer SELECT
    if (query.select_count > 0) {
        apply_select(header, records, record_count, query.select_fields, query.select_count);
    }

    // Libérer la mémoire
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
