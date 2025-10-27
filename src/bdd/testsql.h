#ifndef TESTSQL_H
#define TESTSQL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include "database_manager.h"
// Définition des structures


typedef struct {
    char** orderby_fields;       // Tableau des champs à trier (ex. {"AVG", "ville"})
    char** orderby_directions;  // Tableau des directions ("ASC" ou "DESC") correspondant aux champs
    int orderby_count;          // Nombre de champs dans ORDER BY
} OrderData;

typedef struct {
    char* group_value;
    int count;
    int sum;
    int min;
    int max;
    float avg;
} AggregatedResult;

// Déclaration des fonctions
Header* create_table(int nbColumns, Column* columns);
Record* add_record(Header* header, Record* records, int* record_count, char** values);
Record* apply_where(Header* header, Record* records, int* record_count, char** conditions, int condition_count);
Record* apply_groupby(Header* header, Record* records, int* record_count, char** groupby_fields, int groupby_count);
void apply_select(Header* header, Record* records, int record_count, char** select_fields, int select_count);
AggregatedResult * apply_groupby_with_aggregates(Header * header, Record * records, int * record_count, char ** groupby_fields, int groupby_count, char ** aggregate_fields, int aggregate_count, int * result_count);
Record *from(Header *header, char *from_table, int *record_count);
AggregatedResult *apply_having(AggregatedResult *results, int *result_count, char **having_conditions, int having_count);
void apply_select_on_aggregates(AggregatedResult* results, int result_count, char** select_fields, int select_count);
void apply_orderby_with_aggregates(AggregatedResult* results, int result_count, char** orderby_fields, char** orderby_directions, int orderby_count);
void apply_orderby(Header* header, Record* records, int record_count, char** orderby_fields, char** orderby_directions, int orderby_count);
Record* apply_limit(Record* records, int* record_count, int limit);
Record* apply_offset(Record* records, int* record_count, int offset);

void compute_single_aggregate(Header *header, Record *records, int record_count, const char *aggregate_function);

AggregatedResult *apply_limit_aggregates(AggregatedResult *results, int *result_count, int limit);

AggregatedResult *apply_offset_aggregates(AggregatedResult *results, int *result_count, int offset);

// Fonctions utilitaires
int sum(Record* group, int group_size, int column_index);
float avg(Record* group, int group_size, int column_index);
int min(Record* group, int group_size, int column_index);
int max(Record* group, int group_size, int column_index);

#endif // TESTSQL_H