#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Structures
typedef struct date {
    short year;
    short month;
    short day;
} Date;

typedef struct column {
    char name[20];
    char type[20];
    int length;
} Column;

typedef struct header {
    Date lastUpdate;
    int nbRecords;
    int lengthRecord;
    int nbColumns;
    Column* columns;
} Header;

// Fonction pour afficher un enregistrement
void displayRecord(Header* header, const char* record) {
    for (int i = 0; i < header->nbColumns; i++) {
        Column col = header->columns[i];
        if (strcmp(col.type, "VARCHAR") == 0) {
            printf("%s: %s ", col.name, record);
            record += col.length;
        } else if (strcmp(col.type, "INT") == 0) {
            printf("%s: %d ", col.name, *(int*)record);
            record += sizeof(int);
        } else if (strcmp(col.type, "FLOAT") == 0) {
            printf("%s: %f ", col.name, *(float*)record);
            record += sizeof(float);
        }
    }
    printf("\n");
}

// Programme principal pour vérifier les enregistrements
int main() {
    const char* fileName = "tenant.bin";
    FILE* file = fopen(fileName, "rb+");
    if (!file) {
        perror("Erreur : impossible d'ouvrir le fichier");
        exit(EXIT_FAILURE);
    }

    // Lire le header
    Header header;
    if (fread(&header, sizeof(Header), 1, file) != 1) {
        fprintf(stderr, "Erreur : impossible de lire le header\n");
        fclose(file);
        return 1;
    }

    // Allouer de la mémoire pour les colonnes
    header.columns = malloc(header.nbColumns * sizeof(Column));
    if (fread(header.columns, sizeof(Column), header.nbColumns, file) != (size_t)header.nbColumns) {
        fprintf(stderr, "Erreur : impossible de lire les colonnes\n");
        fclose(file);
        free(header.columns);
        return 1;
    }

    // Afficher le header
    printf("Header :\n");
    printf("  Date de dernière modification : %04d/%02d/%02d\n", 
           header.lastUpdate.year, header.lastUpdate.month, header.lastUpdate.day);
    printf("  Nombre d'enregistrements : %d\n", header.nbRecords);
    printf("  Taille d'un enregistrement : %d\n", header.lengthRecord);
    printf("  Colonnes :\n");
    for (int i = 0; i < header.nbColumns; i++) {
        printf("    - Nom : %s, Type : %s, Longueur : %d\n", 
               header.columns[i].name, header.columns[i].type, header.columns[i].length);
    }

    // Lire et afficher les enregistrements
    printf("\nEnregistrements lus :\n");
    for (int i = 0; i < header.nbRecords; i++) {
        char record[header.lengthRecord];
        if (fread(record, header.lengthRecord, 1, file) != 1) {
            fprintf(stderr, "Erreur : impossible de lire l'enregistrement %d\n", i + 1);
            break;
        }
        printf("Enregistrement %d :\n", i + 1);
        displayRecord(&header, record);
    }

    // Nettoyage
    fclose(file);
    free(header.columns);

    return 0;
}
