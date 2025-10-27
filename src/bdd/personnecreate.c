#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

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

// Fonction pour obtenir la date actuelle
Date getCurrentDate() {
    Date date;
    date.year = 2025;
    date.month = 1;
    date.day = 6;
    return date;
}

// Fonction pour créer une table
void createTable(const char* fileName, Header* header) {
    FILE* file = fopen(fileName, "wb");
    if (!file) {
        perror("Erreur : impossible de créer le fichier");
        exit(EXIT_FAILURE);
    }

    fwrite(header, sizeof(Header), 1, file);
    fwrite(header->columns, sizeof(Column), header->nbColumns, file);

    fclose(file);
    printf("Table '%s' créée avec succès.\n", fileName);
}

// Fonction pour ajouter un enregistrement
void addRecord(const char* fileName, Header* header, const char* nom, int age, const char* ville) {
    FILE* file = fopen(fileName, "r+b");
    if (!file) {
        perror("Erreur : impossible d'ouvrir le fichier");
        exit(EXIT_FAILURE);
    }

    fseek(file, sizeof(Header) + header->nbColumns * sizeof(Column), SEEK_SET);
    fseek(file, header->nbRecords * header->lengthRecord, SEEK_CUR);

    // Préparer un enregistrement
    char record[header->lengthRecord];
    memset(record, 0, header->lengthRecord);

    // Copier les champs
    strncpy(record, nom, header->columns[0].length); // Nom
    *(int*)(record + header->columns[0].length) = age; // Age
    strncpy(record + header->columns[0].length + header->columns[1].length, ville, header->columns[2].length); // Ville

    // Écrire dans le fichier
    fwrite(record, header->lengthRecord, 1, file);

    header->nbRecords++;
    rewind(file);
    fwrite(header, sizeof(Header), 1, file);

    fclose(file);
}

// Fonction pour générer un enregistrement aléatoire
void generateRandomRecords(const char* fileName, Header* header, int numRecords) {
    const char* names[] = {"Jean", "Marie", "Pierre", "Claire", "Sophie", "Paul", "Luc", "Anna", "Marc", "Emma"};
    const char* cities[] = {"Paris", "Lyon", "Marseille", "Toulouse", "Nice", "Bordeaux", "Lille", "Nantes", "Strasbourg", "Rennes"};

    srand(time(NULL));

    for (int i = 0; i < numRecords; i++) {
        const char* name = names[rand() % 10];
        const char* city = cities[rand() % 10];
        int age = 18 + rand() % 70; // Âge entre 18 et 87
        addRecord(fileName, header, name, age, city);
    }
}

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
        }
    }
    printf("\n");
}

// Programme principal
int main() {
    // Initialisation
    Header header;
    header.lastUpdate = getCurrentDate();
    header.nbRecords = 0;
    header.nbColumns = 3;

    // Colonnes
    Column columns[3];
    strcpy(columns[0].name, "nom");
    strcpy(columns[0].type, "VARCHAR");
    columns[0].length = 20;

    strcpy(columns[1].name, "age");
    strcpy(columns[1].type, "INT");
    columns[1].length = sizeof(int);

    strcpy(columns[2].name, "ville");
    strcpy(columns[2].type, "VARCHAR");
    columns[2].length = 20;

    header.columns = columns;
    header.lengthRecord = columns[0].length + columns[1].length + columns[2].length;

    // Créer la table
    const char* fileName = "personne.bin";
    createTable(fileName, &header);

    // Ajouter des enregistrements aléatoires
    int numRecords = 1000; // Nombre d'enregistrements à générer
    generateRandomRecords(fileName, &header, numRecords);

    // Lire et afficher les enregistrements depuis le fichier
    FILE* file = fopen(fileName, "rb");
    if (!file) {
        perror("Erreur : impossible d'ouvrir le fichier pour lecture");
        exit(EXIT_FAILURE);
    }

    // Lire le header
    fread(&header, sizeof(Header), 1, file);
    header.columns = malloc(header.nbColumns * sizeof(Column));
    fread(header.columns, sizeof(Column), header.nbColumns, file);

    // Lire les enregistrements
    printf("\nEnregistrements lus :\n");
    for (int i = 0; i < header.nbRecords; i++) {
        char record[header.lengthRecord];
        fread(record, header.lengthRecord, 1, file);
        displayRecord(&header, record);
    }

    fclose(file);
    free(header.columns);

    return 0;
}
