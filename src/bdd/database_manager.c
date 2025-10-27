#include "database_manager.h"


// gérer les leaks après les retour avec erreurs
// gérer toutes les erreurs lors d'une mauvaise requête utilisateur
// faire des fonctions auxiliaires pour faciliter la libération mémoire

/*
int main() {
    // Nom du fichier pour la table
    const char* filename = "table_test.bin";
    const char* filetemp = "personne.bin";

    // Création d'une structure Record pour simuler les données de colonnes
    Record info;
    puts("---------- TEST CREATE TABLE --------------");
    char* fields[] = {
        "name VARCHAR",
        "age INT",
        "salary FLOAT",
        "company VARCHAR",
        "gender VARCHAR",
        "hobbies VARCHAR",
        NULL
    };
    info.fields = fields;

    // Création de la table
    puts("--------- CREATION DE TABLE --------------");
    createTable(filename, &info);

    puts("------------ FIN CREATION TABLE ------------");
    puts("------------ DEBUT INSERTION DANS TABLE ----");

    Record insertInfo;
    char* insertFields[] = {
        "name Alice Bob Eve Jésus Jean Yves",
        "age 25 30 28 2025 33 87",
        "salary 50000.0 60000.0 70000.0 0.5 2300.0 4213.89",
        "company TechCorp BizGroup CyberInc Nazareth Toulon Toulon",
        "gender Female Male Female Divinity Male Male",
        "hobbies Reading Running Eating Preaching Running Sleeping",
        NULL
    };
    insertInfo.fields = insertFields;

    // ajout des enregistrements mgl
    insertInto(filename, &insertInfo);

    // vérification du nombre d'enregistrements après insertion
    Header* updatedHeader = loadFileHeader(filename);
    if (!updatedHeader) {
        printf("Erreur lors du rechargement du fichier de table.\n");
        return 1;
    }

    printf("Nombre d'enregistrements après insertion : %d\n", updatedHeader->nbRecords);

    puts("------------ FIN INSERTION ----------------");
    puts("------------ AFFICHAGE ENREGISTREMENTS ----");

    displayRecords(filename);
    puts("----------- DEBUT ALTER TABLE ADD ---------");

    Record alterAddInfo;
    char* fields2[] = {
        "animal VARCHAR",
        NULL
    };
    alterAddInfo.fields = fields2;

    alterTableAdd(filename, &alterAddInfo);

    displayRecords(filename);

    puts("----------- DEBUT ALTER TABLE DROP --------");

    Record alterDropInfo;
    char* fields3[] = {
        "animal VARCHAR(32)",
        NULL
    };
    alterDropInfo.fields = fields3;

    alterTableDrop(filename, &alterDropInfo);

    displayRecords(filename);
   
    puts("----------- DEBUT DELETE FROM -------------");

    Record deleteInfo;
    char* fields4[] = {
        "age > 30",
        NULL
    };
    deleteInfo.fields = fields4;

    deleteRecord(filename, &deleteInfo);

    displayRecords(filename);

    puts("----------- DEBUT UPDATE SET ---------------");

    Record updateInfo;
    char* fields5[] = {
        "age 99999",
        "name = Alice",
        NULL
    };
    updateInfo.fields = fields5;

    updateRecord(filename, &updateInfo);

    displayRecords(filename);

    // Libération des ressources
    free(updatedHeader->columns);
    free(updatedHeader);
    
    puts("------------ FIN DE PROGRAMME DE TEST -----");
    
    return 0;
}
*/

// version 2
Header* loadFileHeader(const char* filename) {
    FILE* file = fopen(filename, "rb");
    if (!file) {
        perror("Erreur : impossible d'ouvrir le fichier");
        return NULL;
    }

    // Allouer la structure Header complète
    Header* header = malloc(sizeof(Header));
    if (!header) {
        perror("Erreur : allocation mémoire pour le header");
        fclose(file);
        return NULL;
    }

    // Lire le header
    if (fread(header, sizeof(Header), 1, file) != 1) {
        perror("Erreur : lecture du header fixe");
        fclose(file);
        return NULL;
    }

    // Allouer et lire les colonnes
    header->columns = malloc(header->nbColumns * sizeof(Column));
    if (!header->columns) {
        perror("Erreur : allocation mémoire pour les colonnes");
        free(header);
        fclose(file);
        return NULL;
    }
    if (fread(header->columns, sizeof(Column), header->nbColumns, file) != (size_t)header->nbColumns) {
        perror("Erreur : lecture des colonnes");
        free(header->columns);
        free(header);
        fclose(file);
        return NULL;
    }

    fclose(file);
    
    return header;
}


/* version 1
Header* loadFileHeader(const char* filename) {
    FILE* file = fopen(filename, "rb");
    if (!file) {
        perror("Erreur : impossible d'ouvrir le fichier");
        return NULL;
    }

    // Lire le header fixe
    HeaderFixed fixedHeader;
    if (fread(&fixedHeader, sizeof(HeaderFixed), 1, file) != 1) {
        perror("Erreur : lecture du header fixe");
        fclose(file);
        return NULL;
    }

    // Allouer la structure Header complète
    Header* header = malloc(sizeof(Header));
    if (!header) {
        perror("Erreur : allocation mémoire pour le header");
        fclose(file);
        return NULL;
    }

    // Copier les données fixes dans la structure complète
    header->lastUpdate = fixedHeader.lastUpdate;
    header->nbRecords = fixedHeader.nbRecords;
    header->lengthRecord = fixedHeader.lengthRecord;
    header->nbColumns = fixedHeader.nbColumns;

    // Allouer et lire les colonnes
    header->columns = malloc(header->nbColumns * sizeof(Column));
    if (!header->columns) {
        perror("Erreur : allocation mémoire pour les colonnes");
        free(header);
        fclose(file);
        return NULL;
    }
    if (fread(header->columns, sizeof(Column), header->nbColumns, file) != (size_t)header->nbColumns) {
        perror("Erreur : lecture des colonnes");
        free(header->columns);
        free(header);
        fclose(file);
        return NULL;
    }

    fclose(file);
    
    return header;
}*/


void displayHeader(Header* header) {
    printf("Dernière mise à jour : %d/%d/%d\n", 
           header->lastUpdate.year, 
           header->lastUpdate.month, 
           header->lastUpdate.day);
    printf("Nombre d'enregistrements : %d\n", header->nbRecords);
    printf("Nombre de colonnes : %d\n", header->nbColumns);
    printf("Longueur d'un enregistrement : %d\n", header->lengthRecord);

    for (int i = 0; i < header->nbColumns; i++) {
        printf("Colonne %d: Nom='%s', Type='%s', Longueur=%d\n",
               i + 1,
               header->columns[i].name,
               header->columns[i].type,
               header->columns[i].length);
    }
}

// Fonction pour créer une table version 1
void createTable(const char* filename, Record* info) {
    puts("Bienvenue dans createTable");
    FILE* file = fopen(filename, "rb+");
    if (!file) {
        perror("Erreur : impossible de créer le fichier");
        exit(EXIT_FAILURE);
    }

    HeaderFixed fixedHeader = {0};

    struct tm* t = getCurrentDate();
    fixedHeader.lastUpdate.year = 1900 + t->tm_year;
    fixedHeader.lastUpdate.month = 1 + t->tm_mon;
    fixedHeader.lastUpdate.day = t->tm_mday;
    fixedHeader.nbRecords = 0;
    fixedHeader.lengthRecord = mesureRecord(info);
    fixedHeader.nbColumns = countColumns(info);

    Header* header = malloc(sizeof(Header));
    if (!header) {
        perror("Erreur : allocation mémoire pour le header");
        fclose(file);
        return;
    }

    header->lastUpdate = fixedHeader.lastUpdate;
    header->nbRecords = fixedHeader.nbRecords;
    header->lengthRecord = fixedHeader.lengthRecord;
    header->nbColumns = fixedHeader.nbColumns;

    header->columns = malloc(header->nbColumns * sizeof(Column));
    if (!header->columns) {
        perror("Erreur : allocation mémoire pour les colonnes");
        free(header);
        fclose(file);
        exit(EXIT_FAILURE);
    }
/*
    strcpy(header->columns[0].name, "isActive");
    strcpy(header->columns[0].type, "VARCHAR");
    header->columns[0].length = sizeof(char);
*/
    char** names = getColumnNames(info);
    char** types = getColumnTypes(info);


    for (int i = 0; i < header->nbColumns; i++) {
        strcpy(header->columns[i].name, names[i]);
        strcpy(header->columns[i].type, types[i]);
        if (strncmp(types[i], "VARCHAR", 7) == 0) {
            header->columns[i].length = getVarcharValue(types[i]);
        } else if (strcmp(types[i], "INT") == 0) {
            header->columns[i].length = sizeof(int);
        } else if (strcmp(types[i], "FLOAT") == 0) {
            header->columns[i].length = sizeof(float);
        } else {
            perror("Erreur : type invalide pour calcul de taille.");
            return;
        }
    }

    fwrite(header, sizeof(Header), 1, file);
    fwrite(header->columns, sizeof(Column), header->nbColumns, file);

    fclose(file);

    //char ttempFilename[MAX_FILENAME];
    //snprintf(ttempFilename, MAX_FILENAME, "%s.bin", filename);
    //const char* tempFilename = ttempFilename;

    // Libération des ressources
    int d = 0; // Réinitialiser l'index
    while (types[d] != NULL) {
        free(types[d]);
        d++;
    }
    free(types);

    d = 0;
    while (names[d] != NULL) {
        free(names[d]);
        d++;
    }
    free(names);

    free(header->columns);
    free(header);

    //rename(filename, tempFilename);

    printf("Table '%s' créée avec succès.\n", filename);
}
/*
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
}*/


int countColumns(Record* info) {
    int count = 0;
    while (info->fields[count] != NULL) count++;

    return count;
}


int mesureRecord(Record* info) {
    int count = 0; // Compteur de la taille totale d'un enregistrement
    int d = 0;     // Index pour parcourir les types -> ligne
    char** types = getColumnTypes(info);

    while (types[d] != NULL) {

        if (strncmp(types[d], "VARCHAR", 7) == 0) {
            count += sizeof(char) * getVarcharValue(types[d]);

        } else if (strcmp(types[d], "INT") == 0) {
            count += sizeof(int);

        } else if (strcmp(types[d], "FLOAT") == 0) {
            count += sizeof(float);

        } else {
            perror("Erreur : type non reconnu");
            return WRONG_TYPE;
        }

        d++;
    }

    // Libération des ressources
    d = 0; // Réinitialiser l'index
    while (types[d] != NULL) {
        free(types[d]);
        d++;
    }
    free(types);

    return count;
}


int getVarcharValue(char* varchar) {
    /*
    char separator[2] = "()";
    char* value = getValueBetweenSeparator(separator, varchar);
    int castValue = atoi(value);

    free(value);
*/
    return 20;
}


char* getValueBetweenSeparator(char separator[], char* string) {
    int d = 0;   // Index pour parcourir la chaîne -> ligne
    int dd = 0;  // Index pour construire la valeur -> caractère courant
    char value[12] = {0};

    // Rechercher le premier séparateur
    while (string[d] != separator[0]) {
        if (d >= MAX_LENGTH_QUERY || string[d] == '\0') {
            perror("Erreur : taille dépassée ou séparateur de début introuvable.");
            return NULL;
        }
        d++;  // Avancer jusqu'au premier séparateur
    }

    d++;  // Passer le premier séparateur

    // Rechercher le second séparateur et extraire la valeur entre eux
    while (string[d] != separator[1]) {
        //printf("Valeurs de d=%d et dd=%d  | ", d, dd);
        if (string[d] == '\0') {
            perror("Erreur : séparateur de fin introuvable.");
            return NULL;
        }
        if ((size_t)dd >= sizeof(value) - 1) {
            perror("Erreur : taille de value dépassée.");
            return NULL;
        }
        value[dd++] = string[d++];  // Ajouter le caractère à la valeur
    }

    value[dd] = '\0';  // Terminer la chaîne

    if (dd == 0) {
        perror("Erreur : aucun contenu trouvé entre les séparateurs.");
        return NULL;
    }

    return strdup(value);  // Dupliquer et retourner la valeur extraite
}


char** getColumnTypes(Record* info) {
    int columnCount = countColumns(info) + 1;
    char** types = malloc(columnCount * sizeof(char*));
    if (!types) {
        perror("Erreur : allocation mémoire échouée pour les types de colonnes");
        return NULL;
    }

    for (int i = 0; i < columnCount-1; i++) {
        types[i] = malloc(MAX_LENGTH_QUERY * sizeof(char));
        if (!types[i]) {
            perror("Erreur : allocation mémoire échouée pour un type de colonne");
            for (int j = 0; j < i; j++) free(types[j]);
            free(types);
            return NULL;
        }
        sscanf(info->fields[i], "%*s %s", types[i]); // Extraction du type après le nom
    }

    types[columnCount-1] = NULL;  // fin de struct

    return types;
}


char** getColumnNames(Record* info) {
    int columnCount = countColumns(info) + 1;
    char** names = malloc(columnCount * sizeof(char*));
    if (!names) {
        perror("Erreur : allocation mémoire échouée pour les noms de colonnes");
        return NULL;
    }

    for (int i = 0; i < columnCount-1; i++) {
        names[i] = malloc(MAX_LENGTH_QUERY * sizeof(char));
        if (!names[i]) {
            perror("Erreur : allocation mémoire échouée pour un nom de colonne");
            for (int j = 0; j < i; j++) free(names[j]);
            free(names);
            return NULL;
        }
        sscanf(info->fields[i], "%s", names[i]); // Extraction du nom avant le type
    }

    names[columnCount-1] = NULL;  // fin de struct

    return names;
}


char** getRecordValues(Record* info) {
    // Vérification de la validité de la structure info
    if (!info) {
        printf("Erreur : le pointeur info est NULL.\n");
        return NULL;
    }
    if (!info->fields) {
        printf("Erreur : le champ fields dans info est NULL.\n");
        return NULL;
    }

    // Compter le nombre de colonnes (nombre d'entrées non NULL dans info->fields)
    int numColumns = 0;
    while (info->fields[numColumns] != NULL) {
        // printf("Champ détecté : %s\n", info->fields[numColumns]);
        numColumns++;
    }
    // printf("Nombre de colonnes détectées : %d\n", numColumns);

    // Allouer de la mémoire pour stocker les valeurs extraites
    char** recordValues = malloc((numColumns + 1) * sizeof(char*));
    if (!recordValues) {
        perror("Erreur d'allocation mémoire pour recordValues");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < numColumns; i++) {
        // printf("Traitement du champ %d : %s\n", i, info->fields[i]);

        // Trouver la position du premier espace (après le nom de la colonne)
        char* firstSpace = strchr(info->fields[i], ' ');
        if (firstSpace) {
            // Extraire la sous-chaîne après le premier espace
            recordValues[i] = strdup(firstSpace + 1);
            if (!recordValues[i]) {
                perror("Erreur d'allocation mémoire pour recordValues[i]");
                exit(EXIT_FAILURE);
            }
            // printf("Valeur extraite pour la colonne %d : '%s'\n", i, recordValues[i]);
        } else {
            // Si aucune valeur n'est trouvée, stocker une chaîne vide
            recordValues[i] = strdup("");
            if (!recordValues[i]) {
                perror("Erreur d'allocation mémoire pour chaîne vide");
                exit(EXIT_FAILURE);
            }
            // printf("Aucune valeur trouvée pour la colonne %d, chaîne vide attribuée.\n", i);
        }
    }

    // Terminer la liste avec NULL pour respecter la structure
    recordValues[numColumns] = NULL;
    // printf("Tableau recordValues terminé avec succès. Nombre total de valeurs : %d\n", numColumns);

    return recordValues;
}


// Fonction pour afficher tous les enregistrements
void displayRecords(const char* filename) {
    // printf("Début de la fonction displayRecords pour le fichier : [%s]\n", filename);

    if (!filename) {
        printf("Erreur : le nom de fichier est invalide.\n");
        return;
    }

    // Ouvrir le fichier en mode lecture binaire
    FILE* file = fopen(filename, "rb");
    if (!file) {
        perror("Erreur : impossible d'ouvrir le fichier");
        return;
    }

    // Charger le header de la table
    Header* header = loadFileHeader(filename);
    if (!header) {
        printf("Erreur lors du chargement du fichier de table.\n");
        fclose(file);
        return;
    }
    // printf("Header chargé avec succès.\n");
    // printf("Nombre de colonnes : %d\n", header->nbColumns);
    // printf("Nombre d'enregistrements : %d\n", header->nbRecords);
    // printf("Longueur d'un enregistrement : %d\n", header->lengthRecord);

    // Aller à la position où commencent les enregistrements
    fseek(file, sizeof(Header) + header->nbColumns * sizeof(Column), SEEK_SET);
    // printf("Positionnement au début des enregistrements dans le fichier.\n");

    // Lire et afficher chaque enregistrement
    char record[header->lengthRecord];

    printf("Enregistrements dans la table :\n");
    for (int i = 0; i < header->nbRecords; i++) {
        // Lire un enregistrement
        size_t readSize = fread(record, header->lengthRecord, 1, file);
        if (readSize != 1) {
            printf("Erreur : lecture incomplète pour l'enregistrement %d.\n", i + 1);
            break;
        }

        // Vérifier si l'enregistrement est actif
        if (record[0] == RECORD_DELETED) {
            continue;
            printf("Enregistrement %d marqué comme supprimé, passage au suivant.\n", i + 1);
        }

        // Afficher les valeurs des colonnes
        printf("Enregistrement %d : \n", i + 1);
        
        int offset = 0;

        for (int j = 0; j < header->nbColumns; j++) {
            Column col = header->columns[j];  // renommage
            char value[256] = {0}; // Taille maximale pour afficher une valeur

            // Récupérer la valeur en fonction du type
            if (strncmp(col.type, "VARCHAR", 7) == 0) {
                strncpy(value, record + offset, col.length);
                value[col.length] = '\0'; // S'assurer que c'est une chaîne terminée
                printf("\t[%s]='%s' \n", col.name, value);
            } else if (strcmp(col.type, "INT") == 0) {
                int intValue = *(int*)(record + offset);
                printf("\t[%s]=%d \n", col.name, intValue);
            } else if (strcmp(col.type, "FLOAT") == 0) {
                float floatValue = *(float*)(record + offset);
                printf("\t[%s]=%.2f \n", col.name, floatValue);
            } else {
                printf("\t[%s]=UNKNOWN \n", col.name);
            }

            // Avancer l'offset pour la prochaine colonne
            offset += col.length;
            // printf("(Offset après la colonne '%s' : %d) \n", col.name, offset);
        }
        printf("\n");
    }

    free(header->columns);
    free(header);
    fclose(file);
}


// il faudra respecter l'ordre des colonnes
// ajoute seulement une ligne et actualise le header
void addRecord(const char* filename, Record* info, int accumulateur) {
    FILE* file = fopen(filename, "rb+");
    if (!file) {
        perror("Error: Unable to open file");
        exit(EXIT_FAILURE);
    }

    Header* header = loadFileHeader(filename);
    if (!header) {
        fprintf(stderr, "Error: Unable to load file header.\n");
        fclose(file);
        exit(EXIT_FAILURE);
    }

    char** names = getColumnNames(info);
    char** values = getRecordValues(info);
    if (!names || !values) {
        fprintf(stderr, "Error: Column names or record values are NULL.\n");
        fclose(file);
        free(header);
        exit(EXIT_FAILURE);
    }

    char record[header->lengthRecord];
    memset(record, 0, header->lengthRecord);

    fseek(file, sizeof(Header) + header->nbColumns * sizeof(Column), SEEK_SET);
    fseek(file, header->nbRecords * header->lengthRecord, SEEK_CUR);

    for (int i = 0; i < header->nbColumns; i++) {
        if (!values[i]) {
            fprintf(stderr, "Error: Value for column %s is NULL.\n", names[i]);
            fclose(file);
            free(header);
            return;
        }

        char* value_cpy = strtok(values[i], " ");
        for (int j = 0; j < accumulateur; j++) {
            value_cpy = strtok(NULL, " ");
        }

        if (!value_cpy) {
            fprintf(stderr, "Error: Value copy for column %s is NULL.\n", names[i]);
            fclose(file);
            free(header);
            return;
        }

        if (strcmp(names[i], header->columns[i].name) != 0) {
            fprintf(stderr, "Error: Column name mismatch for %s.\n", names[i]);
            fclose(file);
            free(header);
            return;
        }

        int offset = tailleDynamiqueAddRecord(header, i);

        if (strncmp(header->columns[i].type, "VARCHAR", 7) == 0) {
            strncpy(record + offset, value_cpy, header->columns[i].length);
            record[offset + header->columns[i].length - 1] = '\0';
        } else if (strcmp(header->columns[i].type, "INT") == 0) {
            int int_value = atoi(value_cpy);
            memcpy(record + offset, &int_value, sizeof(int));
        } else if (strcmp(header->columns[i].type, "FLOAT") == 0) {
            float float_value = atof(value_cpy);
            memcpy(record + offset, &float_value, sizeof(float));
        } else {
            fprintf(stderr, "Error: Unrecognized type for column %s.\n", names[i]);
            fclose(file);
            free(header);
            return;
        }
    }

    fwrite(record, header->lengthRecord, 1, file);

    header->nbRecords++;
    rewind(file);
    fwrite(header, sizeof(Header), 1, file);

    fclose(file);

    for (int d = 0; values[d] != NULL; d++) {
        free(values[d]);
    }
    free(values);

    for (int d = 0; names[d] != NULL; d++) {
        free(names[d]);
    }
    free(names);

    free(header->columns);
    free(header);
}


void insertInto(const char* filename, Record* info) {
    int accumulateur = 0;

    // calcul nombre d'enregistrement à faire
    int nb = 0;
    char** values = getRecordValues(info);
    // vérif si argu en trop -> a faire

    char *token;
    token = strtok(values[0], " ");

    // printf("Valeur du token : [%s]\n", token);
    while (token != NULL) {
        // printf("Token: %s\n", token);
        token = strtok(NULL, " ");
        nb++;
    }


    for (int i = 0; i < nb; i++) {
        addRecord(filename, info, accumulateur);
        accumulateur++;
    }

    // Libération des ressources
    int d = 0; // Réinitialiser l'index
    while (values[d] != NULL) {
        free(values[d]);
        d++;
    }
    free(values);
}


int tailleDynamiqueAddRecord(Header* header, int accumulateur) {
    // ajout un terme selon l'accumulateur
    int total = 0;
    for (int i = 0; i < accumulateur; i++) {
        total += header->columns[i].length;
    }

    return total;
}


void dropTable(const char* filename) {
    // modifier si index créé
    if (remove(filename) == 0) {
        printf("Table '%s' supprimée avec succès.\n", filename);
    } else {
        perror("Erreur lors de la suppression de la table");
    }
}


/*
pour ajouter/supprimer une colonne :
faire une copie
create table
insert into
drop table
renommer la copie

avec plus ou moins d'informations
*/

// ajouter une/plusieurs colonne(s)
void alterTableAdd(const char* filename, Record* info) {
    printf("[DEBUG] Début de alterTableAdd avec fichier : %s\n", filename);

    // Chargement de l'en-tête
    Header* header = loadFileHeader(filename);
    if (!header) {
        printf("Erreur lors du chargement de l'en-tête du fichier : %s\n", filename);
        exit(1);
    }
    printf("[DEBUG] En-tête chargé : %d colonnes existantes\n", header->nbColumns);

    // Création du fichier temporaire
    char ttempFilename[MAX_FILENAME];
    snprintf(ttempFilename, MAX_FILENAME, "temp_%s", filename);
    const char* tempFilename = ttempFilename;
    printf("[DEBUG] Nom du fichier temporaire créé : %s\n", tempFilename);

    // Récupération des données
    char** names = getColumnNames(info);
    char** types = getColumnTypes(info);
    if (!names || !types) {
        printf("Erreur : noms ou types des colonnes introuvables\n");
        free(header);
        exit(1);
    }

    printf("[DEBUG] Colonnes à ajouter récupérées\n");

    // Mise à jour du header
    int i = 0;
    while (names[i] != NULL && types[i] != NULL) {
        printf(BLUE"[DEBUG] Ajout de la colonne %s (%s)\n"RESET, names[i], types[i]);

        // Ajout d'une nouvelle colonne
        header->nbColumns++;
        Column* new_columns = realloc(header->columns, sizeof(Column) * header->nbColumns);
        if (!new_columns) {
            perror("Erreur : échec de la réallocation de mémoire pour les colonnes");
            free(header->columns);
            exit(1);
        }
        header->columns = new_columns;

        // Remplissage des données de la nouvelle colonne
        int position = header->nbColumns - 1;
        strncpy(header->columns[position].name, names[i], 19);  // mettre macro pour remplacer 20-1
        header->columns[position].name[19] = '\0'; // Sécurité

        strncpy(header->columns[position].type, types[i], 19);
        header->columns[position].type[19] = '\0'; // Sécurité

        // Gestion du type
        if (strncmp(types[i], "VARCHAR", 7) == 0) {
            header->columns[position].length = sizeof(char) * getVarcharValue(types[i]);
        } else if (strcmp(types[i], "INT") == 0) {
            header->columns[position].length = sizeof(int);
        } else if (strcmp(types[i], "FLOAT") == 0) {
            header->columns[position].length = sizeof(float);
        } else {
            printf("Erreur : type non reconnu (%s) pour la colonne %s\n", types[i], names[i]);
            exit(1);
        }

        printf(GREEN"Colonne ajoutée : %s, Type : %s, Taille : %d\n"RESET,
               header->columns[position].name,
               header->columns[position].type,
               header->columns[position].length);

        i++;
    }

    header->lengthRecord += mesureRecord(info);

    printf("[DEBUG] Mise à jour du header terminée :\n");

    // Création du fichier temporaire avec le nouvel en-tête
    Record* temp = header_to_edit(header);   // pas besoin de le free() ?
    Record* ttemp = header_to_edit(header);  // très moche mais tant pis, free() en bas
    int k = 0;
    while (temp->fields[k] != NULL) {
        ttemp->fields[k] = temp->fields[k + 1];
        k++;
    }

    createTable(tempFilename, ttemp);
    printf("[DEBUG] Fichier temporaire créé avec le nouvel en-tête : %s\n", tempFilename);

    // Mise à jour des enregistrements
    Record* temp2 = record_to_edit(filename);  // enregistrements de base

    // modification du Record* avec les nouvelles colonnes et valeurs par défaut
    for (int j = 0; names[j] != NULL; j++) {
        // Allocation de mémoire pour la nouvelle ligne (nouvelle colonne)
        char* new_field = malloc(1024); 
        if (!new_field) {
            perror("Erreur : allocation mémoire pour une nouvelle colonne");
            exit(1);
        }

        // Construction de la ligne : nom de la colonne suivi des valeurs par défaut
        snprintf(new_field, 1024, "%s", names[j]); // Ajouter le nom de la colonne

        for (int record_index = 0; temp2->fields[0] != NULL && temp2->fields[record_index] != NULL; record_index++) {
            if (strncmp(types[j], "VARCHAR", 7) == 0) {
                strcat(new_field, " none"); // Ajouter une valeur par défaut vide (string "none") pour VARCHAR
            } else if (strcmp(types[j], "INT") == 0) {
                strcat(new_field, " -1"); // Ajouter une valeur par défaut pour INT
            } else if (strcmp(types[j], "FLOAT") == 0) {
                strcat(new_field, " 0.0"); // Ajouter une valeur par défaut pour FLOAT
            } else {
                printf("Erreur : type inconnu pour la colonne %s\n", names[j]);
                free(new_field);
                exit(1);
            }
        }

        // Ajout de la nouvelle ligne dans temp2
        int temp2_length = 0;
        while (temp2->fields[temp2_length] != NULL) {
            temp2_length++;
        }

        temp2->fields = realloc(temp2->fields, sizeof(char*) * (temp2_length + 2)); // +1 pour la nouvelle colonne, +1 pour NULL
        if (!temp2->fields) {
            perror("Erreur : réallocation mémoire pour les champs de temp2");
            free(new_field);
            exit(1);
        }

        temp2->fields[temp2_length] = new_field; // Ajout du nouveau champ
        temp2->fields[temp2_length + 1] = NULL;  // Terminer par NULL
    }  // fin de modification

    insertInto(tempFilename, temp2);
    free_edit(temp2);
    printf("[DEBUG] Enregistrements mis à jour dans le fichier temporaire\n");

    // Suppression et remplacement du fichier original
    dropTable(filename);
    if (rename(tempFilename, filename) != 0) {
        perror("Erreur : impossible de renommer le fichier temporaire");
        exit(1);
    }
    printf("[DEBUG] Fichier temporaire renommé en fichier original : %s\n", filename);

    // Libération mémoire
    int d = 0;
    while (names[d] != NULL) {
        free(names[d]);
        d++;
    }
    d = 0;
    while (types[d] != NULL) {
        free(types[d]);
        d++;
    }
    free(names);
    free(types);
    free(header->columns);
    free(header);
    free_edit(ttemp);

    printf("[DEBUG] Fin de alterTableAdd pour le fichier : %s\n", filename);
}


// supprimer une colonne
void alterTableDrop(const char* filename, Record* info) {

    // Chargement de l'en-tête
    Header* header = loadFileHeader(filename);
    if (!header) {
        printf("Erreur lors du chargement du fichier de table.\n");
        exit(1);
    }

    // Création du fichier temporaire
    char ttempFilename[MAX_FILENAME];
    snprintf(ttempFilename, MAX_FILENAME, "temp_%s", filename);
    const char* tempFilename = ttempFilename;
    
    // Récupération des données
    char** names = getColumnNames(info);

    // Mise à jour du header
    int columnsToRemove = 0;  // nombre de colonne à supprimer
    for (int i = 0; names[i] != NULL; i++) {
        for (int j = 0; j < header->nbColumns; j++) {
            if (strcmp(names[i], header->columns[j].name) == 0) {
                columnsToRemove++;
                break;
            }
        }
    }

    if (columnsToRemove == 0) {
        printf("Aucune des colonnes spécifiées n'existe dans la table '%s'.\n", filename);
        free(header->columns);
        free(header);
        return;
    }

    // nouveau header sans les colonnes supprimées
    int newNbColumns = header->nbColumns - columnsToRemove;
    Column* newColumns = malloc(newNbColumns * sizeof(Column));
    if (!newColumns) {
        perror("Erreur : allocation mémoire pour les nouvelles colonnes");
        free(header->columns);
        free(header);
        return;
    }

    int newIndex = 0;
    for (int i = 0; i < header->nbColumns; i++) {
        int shouldRemove = 0;
        for (int j = 0; names[j] != NULL; j++) {
            if (strcmp(names[j], header->columns[i].name) == 0) {
                shouldRemove = 1;
                break;
            }
        }

        if (!shouldRemove) {
            newColumns[newIndex++] = header->columns[i];
        }
    }

    free(header->columns);
    header->columns = newColumns;
    header->nbColumns = newNbColumns;
    Record* htmp = header_to_edit(header);
    header->lengthRecord -= mesureRecord(htmp);
    free_edit(htmp);

    printf(GREEN);
    displayHeader(header);
    printf(RESET);


    // On écrit le nouveau header dans le fichier temporaire
    Record* temp = header_to_edit(header);   // pas besoin de le free() ?
    Record* ttemp = header_to_edit(header);  // très moche mais tant pis, free() en bas
    int k = 0;
    while (temp->fields[k] != NULL) {
        ttemp->fields[k] = temp->fields[k + 1];
        k++;
    }
    createTable(tempFilename, ttemp);

    // printf("Header mis à jour avec le nouveau nombre de colonnes : %d\n", header->nbColumns);

    // Mise à jour des enregistrements
    // on appelle insert into sans les colonnes supprimées
    Record* temp2 = record_to_edit(filename);

    // Suppression des colonnes spécifiées dans `names` des enregistrements existants
    Record* temp3 = malloc(sizeof(Record)); // Nouveau Record* pour les enregistrements filtrés
    if (!temp3) {
        perror("Erreur : allocation mémoire pour temp3");
        exit(1);
    }

    temp3->fields = NULL; // Initialiser le tableau des champs
    int temp3_length = 0;

    // Parcourir les enregistrements existants
    for (int i = 0; temp2->fields[i] != NULL; i++) {
        int shouldRemove = 0;

        // Vérifier si cette colonne doit être supprimée
        for (int j = 0; names[j] != NULL; j++) {
            if (strncmp(temp2->fields[i], names[j], strlen(names[j])) == 0) {
                shouldRemove = 1;
                break;
            }
        }

        // Si la colonne ne doit pas être supprimée, on la copie dans temp3
        if (!shouldRemove) {
            temp3->fields = realloc(temp3->fields, sizeof(char*) * (temp3_length + 2)); // +1 pour la nouvelle colonne, +1 pour NULL
            if (!temp3->fields) {
                perror("Erreur : réallocation mémoire pour temp3->fields");
                free_edit(temp3);
                exit(1);
            }

            temp3->fields[temp3_length] = strdup(temp2->fields[i]); // Copier le champ
            if (!temp3->fields[temp3_length]) {
                perror("Erreur : duplication mémoire pour un champ");
                free_edit(temp3);
                exit(1);
            }

            temp3_length++;
            temp3->fields[temp3_length] = NULL; // Terminer avec NULL
        }
    }

    // Remplacer temp2 par temp3 pour ne conserver que les colonnes restantes
    free_edit(temp2); // Libérer l'ancien Record*
    temp2 = temp3;

    insertInto(tempFilename, temp2);
    free_edit(temp2);

    // On supprime le fichier et on renomme le temporaire
    dropTable(filename);
    rename(tempFilename, filename);

    // Libération mémoire
    int d = 0;
    while (names[d] != NULL) {
        free(names[d]);
        d++;
    }
    d = 0;
    free(names);

    free(header->columns);
    free(header);
    free_edit(ttemp);
}


Record* header_to_edit(Header* header) {
    // Vérifier que le pointeur header n'est pas NULL
    if (header == NULL) {
        return NULL;
    }

    // Allouer de la mémoire pour la structure Record
    Record* edit = (Record*)malloc(sizeof(Record));
    if (edit == NULL) {
        perror("Erreur d'allocation de mémoire pour Record");
        return NULL;
    }

    // Allouer de la mémoire pour le tableau fields dans Record
    // +1 pour le NULL final
    edit->fields = (char**)malloc((header->nbColumns + 1) * sizeof(char*));
    if (edit->fields == NULL) {
        perror("Erreur d'allocation de mémoire pour les champs (fields)");
        free(edit);
        return NULL;
    }

    // Remplir les champs avec "<nom_colonne> <type_colonne>"
    for (int i = 0; i < header->nbColumns; i++) {
        // Calculer la taille nécessaire pour chaque champ
        size_t field_size = strlen(header->columns[i].name) + 1 + strlen(header->columns[i].type) + 1;
        edit->fields[i] = (char*)malloc(field_size * sizeof(char));
        if (edit->fields[i] == NULL) {
            perror("Erreur d'allocation de mémoire pour un champ individuel");
            // Libérer les ressources allouées précédemment
            for (int j = 0; j < i; j++) {
                free(edit->fields[j]);
            }
            free(edit->fields);
            free(edit);
            return NULL;
        }

        // Construire la chaîne au format "<nom_colonne> <type_colonne>"
        snprintf(edit->fields[i], field_size, "%s %s", header->columns[i].name, header->columns[i].type);
    }

    // Terminer le tableau par un pointeur NULL
    edit->fields[header->nbColumns] = NULL;

    return edit;
}


// Fonction pour libérer la mémoire allouée pour Record
void free_edit(Record* edit) {
    if (edit != NULL) {
        if (edit->fields != NULL) {
            for (int i = 0; edit->fields[i] != NULL; i++) {
                free(edit->fields[i]);
            }
            free(edit->fields);
        }
        free(edit);
    }
}


// ne prend pas la colonne isActive en compte
Record* record_to_edit(const char* filename) {
    // Ouverture du fichier en lecture binaire
    FILE* file = fopen(filename, "rb");
    if (!file) {
        perror("Erreur : impossible d'ouvrir le fichier");
        return NULL;
    }

    // Lecture du header
    Header* header = loadFileHeader(filename);
    if (!header) {
        printf("Erreur lors du chargement du header.\n");
        fclose(file);
        return NULL;
    }

    // Allouer de la mémoire pour Record
    Record* info = (Record*)malloc(sizeof(Record));
    if (!info) {
        perror("Erreur d'allocation de mémoire pour Record");
        fclose(file);
        free(header->columns);
        free(header);
        return NULL;
    }

    // Allouer de la mémoire pour le tableau fields (nbColumns - 1 pour ignorer la première colonne)
    info->fields = (char**)malloc((header->nbColumns) * sizeof(char*));
    if (!info->fields) {
        perror("Erreur d'allocation de mémoire pour les champs");
        free(info);
        fclose(file);
        free(header->columns);
        free(header);
        return NULL;
    }

    // Initialiser les champs (en excluant la première colonne)
    for (int i = 0; i < header->nbColumns; i++) {
        info->fields[i] = NULL;
    }
    info->fields[header->nbColumns - 1] = NULL; // Terminer par NULL

    // Allouer de la mémoire pour chaque champ (sauf la première colonne)
    for (int i = 0; i < header->nbColumns; i++) {
        // Taille estimée : nom de colonne + espaces + longueur totale des valeurs
        size_t field_size = strlen(header->columns[i].name) + 1 + header->nbRecords * 20;
        info->fields[i] = (char*)malloc(field_size * sizeof(char));
        if (!info->fields[i]) {
            perror("Erreur d'allocation de mémoire pour un champ individuel");
            // Libérer les champs déjà alloués
            for (int j = 0; j < i; j++) {
                free(info->fields[j]);
            }
            free(info->fields);
            free(info);
            fclose(file);
            free(header->columns);
            free(header);
            return NULL;
        }

        // Initialiser avec le nom de la colonne
        snprintf(info->fields[i - 1], field_size, "%s", header->columns[i].name);
    }

    // Lire les enregistrements
    char record[header->lengthRecord];
    fseek(file, sizeof(Header) + header->nbColumns * sizeof(Column), SEEK_SET);

    for (int rec = 0; rec < header->nbRecords; rec++) {
        fread(record, header->lengthRecord, 1, file);

        // Parcourir chaque colonne (en excluant la première colonne) pour extraire les valeurs
        for (int col = 1; col < header->nbColumns; col++) {
            // Calcul de l'offset
            int offset = tailleDynamiqueAddRecord(header, col);

            // Extraire la valeur selon le type
            char value[50] = {0};
            if (strncmp(header->columns[col].type, "VARCHAR", 7) == 0) {
                strncpy(value, record + offset, header->columns[col].length - 1);
                value[header->columns[col].length - 1] = '\0'; // Assurer une terminaison
            } else if (strcmp(header->columns[col].type, "INT") == 0) {
                int int_value;
                memcpy(&int_value, record + offset, sizeof(int));
                snprintf(value, sizeof(value), "%d", int_value);
            } else if (strcmp(header->columns[col].type, "FLOAT") == 0) {
                float float_value;
                memcpy(&float_value, record + offset, sizeof(float));
                snprintf(value, sizeof(value), "%.2f", float_value);
            }

            // Ajouter la valeur au champ correspondant
            strcat(info->fields[col], " ");
            strcat(info->fields[col], value);
        }
    }

    fclose(file);
    free(header->columns);
    free(header);

    return info;
}


/*{
    "where condition",
    NULL,
};*/
void deleteRecord(const char* filename, Record* info) {
    FILE* file = fopen(filename, "rb+");
    if (!file) {
        perror("Erreur : impossible d'ouvrir le fichier");
        exit(1);
    }

    // Lecture du header
    Header* header = loadFileHeader(filename);
    if (!header) {
        printf("[DEBUG] Erreur lors du chargement du header.\n");
        fclose(file);
        exit(1);
    }

    // Vérification de la condition WHERE
    if (info->fields[0] == NULL) {
        printf("[ERROR] La condition WHERE est vide ou non définie.\n");
        free(header->columns);
        free(header);
        fclose(file);
        return;
    }

    char* whereCondition = strdup(info->fields[0]); // Copie locale
    if (!whereCondition) {
        perror("Erreur d'allocation mémoire pour la condition WHERE");
        free(header->columns);
        free(header);
        fclose(file);
        return;
    }

    // Extraction des conditions WHERE
    char* token = strtok(whereCondition, " ");
    char* conditions[3] = {NULL, NULL, NULL};
    int i = 0;
    while (token != NULL) {
        conditions[i] = token;
        token = strtok(NULL, " ");
        i++;
    }

    // Recherche de la colonne spécifiée dans WHERE
    int pos = -1;
    char type[20];
    for (int i = 0; i < header->nbColumns; i++) {
        if (strcmp(header->columns[i].name, conditions[0]) == 0) {
            pos = i;
            strncpy(type, header->columns[i].type, sizeof(type));
            break;
        }
    }
    if (pos == -1) {
        printf("[ERROR] Colonne [%s] non trouvée dans la table.\n", conditions[0]);
        free(header->columns);
        free(header);
        fclose(file);
        return;
    }

    // Parcourir les enregistrements et conserver les valides
    char record[header->lengthRecord];
    char* validRecords = malloc(header->nbRecords * header->lengthRecord); // Mémoire temporaire
    if (!validRecords) {
        perror("Erreur d'allocation mémoire pour les enregistrements valides");
        free(header->columns);
        free(header);
        fclose(file);
        return;
    }

    int validCount = 0; // Compteur des enregistrements valides
    fseek(file, sizeof(Header) + header->nbColumns * sizeof(Column), SEEK_SET);

    for (int rec = 0; rec < header->nbRecords; rec++) {
        fread(record, header->lengthRecord, 1, file);

        int offset = tailleDynamiqueAddRecord(header, pos);
        void* value = NULL;

        // Extraction de la valeur
        if (strncmp(type, "VARCHAR", 7) == 0) {
            int dim = getVarcharValue(type);
            char* varcharValue = (char*)malloc(dim);
            if (!varcharValue) {
                perror("Erreur d'allocation pour VARCHAR");
                free(validRecords);
                exit(1);
            }
            strncpy(varcharValue, (char*)(record + offset), dim);
            value = varcharValue;
        } else if (strcmp(type, "FLOAT") == 0) {
            float floatValue;
            memcpy(&floatValue, record + offset, sizeof(float));
            value = &floatValue;
        } else if (strcmp(type, "INT") == 0) {
            int intValue;
            memcpy(&intValue, record + offset, sizeof(int));
            value = &intValue;
        } else {
            perror("Erreur : type non reconnu...");
            free(validRecords);
            exit(1);
        }

        bool condition = exeWhere(value, conditions[1], conditions[2], type);

        if (strncmp(type, "VARCHAR", 7) == 0) {
            free(value);
        }

        // Si la condition n'est pas remplie, ajouter l'enregistrement aux valides
        if (!condition) {
            memcpy(validRecords + validCount * header->lengthRecord, record, header->lengthRecord);
            validCount++;
        }
    }

    // Réécriture du fichier avec les enregistrements valides
    fseek(file, sizeof(Header) + header->nbColumns * sizeof(Column), SEEK_SET);
    fwrite(validRecords, header->lengthRecord, validCount, file);

    // Mise à jour des métadonnées
    header->nbRecords = validCount;
    rewind(file);
    fwrite(header, sizeof(Header), 1, file);

    // Nettoyage
    free(validRecords);
    free(header->columns);
    free(header);
    free(whereCondition);
    fclose(file);
}


// renvoie vrai s'il n'y a pas de condition (on supprime tous les enregistrements) - NON FAIT
// renvoie faux si la condition est invalide syntaxiquement
bool exeWhere(void* colValue, char* operateur, char* compValue, char* type) {
    if (colValue == NULL || operateur == NULL || compValue == NULL || type == NULL) {
        return false; // Paramètre invalide
    }

    if (strcmp(type, "INT") == 0) {
        int val1 = *((int*)colValue);
        int val2 = atoi(compValue);

        if (strcmp(operateur, "=") == 0) return val1 == val2;
        if (strcmp(operateur, "!=") == 0 || strcmp(operateur, "<>") == 0) return val1 != val2;
        if (strcmp(operateur, "<") == 0) return val1 < val2;
        if (strcmp(operateur, ">") == 0) return val1 > val2;
        if (strcmp(operateur, "<=") == 0) return val1 <= val2;
        if (strcmp(operateur, ">=") == 0) return val1 >= val2;

    } else if (strcmp(type, "FLOAT") == 0) {

        float val1 = *((float*)colValue);
        float val2 = atof(compValue);

        if (strcmp(operateur, "=") == 0) return val1 == val2;
        if (strcmp(operateur, "!=") == 0 || strcmp(operateur, "<>") == 0) return val1 != val2;
        if (strcmp(operateur, "<") == 0) return val1 < val2;
        if (strcmp(operateur, ">") == 0) return val1 > val2;
        if (strcmp(operateur, "<=") == 0) return val1 <= val2;
        if (strcmp(operateur, ">=") == 0) return val1 >= val2;

    } else if (strncmp(type, "VARCHAR", 7) == 0) {

        char* val1 = (char*)colValue;
        char* val2 = (char*)compValue;

        if (strcmp(operateur, "=") == 0) return strcmp(val1, val2) == 0;
        if (strcmp(operateur, "!=") == 0 || strcmp(operateur, "<>") == 0) return strcmp(val1, val2) != 0;
        if (strcmp(operateur, "<") == 0) return strcmp(val1, val2) < 0;
        if (strcmp(operateur, ">") == 0) return strcmp(val1, val2) > 0;
        if (strcmp(operateur, "<=") == 0) return strcmp(val1, val2) <= 0;
        if (strcmp(operateur, ">=") == 0) return strcmp(val1, val2) >= 0;
    }

    return false; // Si aucun cas ne correspond
}


/*{
    "nom_colonne1 value1",
    "nom_colonne2 value2",
    ...,
    "where condition",
    NULL
};*/
// update set ne fonctionne pas encore parfaitement bien...
// problèmes avec les INT et FLOAT... | comparaisons corrigées cependant
// VARCHAR se gère sans soucis
void updateRecord(const char* filename, Record* info) {
    // parcourir le fichier
    // si nom de colonne correspond changer la valeur associée
    // sinon ne rien faire
    // préciser si la colonne existe ou non
    printf("[DEBUG] Début de la fonction updateRecord.\n");

    FILE* file = fopen(filename, "rb+");
    if (!file) {
        perror("Erreur : impossible d'ouvrir le fichier");
        exit(1);
    }

    // Lecture du header
    Header* header = loadFileHeader(filename);
    if (!header) {
        printf("Erreur lors du chargement du header.\n");
        fclose(file);
        exit(1);
    }

    printf("[DEBUG] Header chargé avec succès. nbColumns=%d, nbRecords=%d, lengthRecord=%d\n",
           header->nbColumns, header->nbRecords, header->lengthRecord);

    // on va boucler dessus puis boucler le fread() -> O(n²) -> tant pis
    // mieux vérifier la validité de la requête (?)
    int temp = -1;  // pour éviter de continuer seulement avec le where
    int nbEnr = 0;  // nombre de changements à faire - 1 pour enlever le where = nb de colonnes
    while (info->fields[nbEnr] != NULL) {
        printf("[DEBUG] info->fields[%d] : %s\n", nbEnr, info->fields[nbEnr]);
        nbEnr++;
        temp++;
    }
    if (temp == 0) {
        // on n'a croisé que le where -> requête invalide
        // on n'a croisé qu'une colonne sans le where -> requête valide mais qu'on ne traite pas
        printf("Aucun changement à faire.\n");
        return;
    }

    // on décrémente l'indice pour faciliter son utilisation
    nbEnr--;  // au moins égal à 2 => 1 seul changement à faire au minimum
    printf("[DEBUG] Nombre d'enregistrements à modifier : %d\n", nbEnr);

    // étude de la condition where
    char* whereCondition = strdup(info->fields[nbEnr]);
    if (!whereCondition) {
        perror("Erreur d'allocation mémoire pour la condition WHERE");
        free(header->columns);
        free(header);
        fclose(file);
        return;
    }

    printf("[DEBUG] Condition WHERE copiée : %s\n", whereCondition);

    char* token = strtok(whereCondition, " ");  // le where se situe à nbEnr-2 (-1 est NULL)
    // 1: nom de colonne | 2: opérateur | 3: valeur de comparaison
    char* conditions[3] = {NULL, NULL, NULL};
    int k = 0;
    while (token != NULL) {
        conditions[k] = token;
        // Extraction des tokens suivants
        printf("[DEBUG] Condition[%d] : %s\n", k, token);
        token = strtok(NULL, " ");
        k++;
    }

    // récupérer les valeurs à rajouter
    char** values = getColumnTypes(info);  // ne pas prendre en compte les deux dernières lignes
    char** names = getColumnNames(info);   // idem
    printf("[DEBUG] Valeurs à insérer récupérées.\n");

    // calculs de positions/types pour correspondre aux modifications
    int positions[nbEnr];
    int position_where = 0;
    char type[nbEnr][20];
    char type_where[20];
    for (int j=0; j < nbEnr; j++) {
        // récupérer à quelle position la colonne se situe dans le header (pour l'offset)
        int pos = -1;  // position de la colonne
        // calculs pour modification et comparaison
        for (int i=1; i < header->nbColumns; i++) {
            printf("[DEBUG] Vérification colonne : %s\n", header->columns[i].name);
            if (strcmp(header->columns[i].name, names[j]) == 0) {  // type à modifier
                pos = i;  // position
                strncpy(type[j], header->columns[i].type, sizeof(type[j]));
                printf("[DEBUG] Colonne trouvée : %s, Position=%d, Type=%s\n", names[j], pos, type[j]);
            }
            if (strcmp(header->columns[i].name, conditions[0]) == 0) {  // type à comparer
                strncpy(type_where, header->columns[i].type, sizeof(type_where));
                printf("[DEBUG] Colonne where trouvée : %s, position=%d, type=%s\n", header->columns[i].name, i, type_where);
                position_where = i;
            }

        }
        if (pos == -1 || position_where == 0) {  // au cas où pas de where trouvé...
            printf(RED"Colonne [%s] non trouvée dans la table.\n"RESET, names[j]);
            free(header->columns);
            free(header);
            fclose(file);
            return;
        }

        positions[j] = pos;
    }

    // Lire/w les enregistrements et comparer
    char record[header->lengthRecord];
    fseek(file, sizeof(Header) + header->nbColumns * sizeof(Column), SEEK_SET);

    printf("[DEBUG] Début de la modification des enregistrements.\n");

    for (int i=0; i < nbEnr; i++) {

        // vérifier qu'on oublie pas des enregistrements... - NON FAIT
        for (int rec = 0; rec < header->nbRecords; rec++) {

            fread(record, header->lengthRecord, 1, file);
            printf("[DEBUG] Lecture de l'enregistrement %d.\n", rec+1);

            int offset = tailleDynamiqueAddRecord(header, position_where);
            printf("[DEBUG] Offset calculé : %d\n", offset);

            void* value = NULL;

            // récupération de l'enregistrement pour comparaison
            if (strncmp(type_where, "VARCHAR", 7) == 0) {
                int dim = getVarcharValue(type_where);
                printf("[DEBUG] Type VARCHAR, dimension : %d\n", dim);
                char* varcharValue = (char*)malloc(dim);
                if (!varcharValue) {
                    perror("Erreur d'allocation pour VARCHAR");
                    exit(1);
                }
                strncpy(varcharValue, (char*)(record + offset), dim);
                value = varcharValue;
                printf("[DEBUG] Valeur VARCHAR extraite : %s\n", varcharValue);
            } else if (strcmp(type_where, "FLOAT") == 0) {
                float floatValue;
                memcpy(&floatValue, record + offset, sizeof(float));
                value = &floatValue;
                printf("[DEBUG] Valeur FLOAT extraite : %f\n", floatValue);
            } else if (strcmp(type_where, "INT") == 0) {
                int intValue;
                memcpy(&intValue, record + offset, sizeof(int));
                value = &intValue;
                printf("[DEBUG] Valeur INT extraite : %d\n", intValue);
            } else {
                perror("Erreur : type non reconnu...");
                exit(1);
            }

            // Vérifier la condition
            bool condition = exeWhere(value, conditions[1], conditions[2], type_where);
            printf("[DEBUG] Condition WHERE évaluée : %s\n", condition ? "true" : "false");

            // Libérer la mémoire pour VARCHAR
            if (strncmp(type_where, "VARCHAR", 7) == 0) {
                free(value);
            }

            // WHERE
            if (condition) {
                // on applique les modifications si la condition est vérifiée
                offset = tailleDynamiqueAddRecord(header, positions[i]);
                if (strncmp(type[i], "VARCHAR", 7) == 0) {
                    int dim = getVarcharValue(type[i]);
                    strncpy(record + offset, values[i], dim);
                } else if (strcmp(type[i], "FLOAT") == 0) {
                    memcpy(record + offset, (float*)values[i], sizeof(float));
                } else if (strcmp(type[i], "INT") == 0) {
                    memcpy(record + offset, (int*)values[i], sizeof(int));
                }

                // écriture
                fseek(file, -header->lengthRecord, SEEK_CUR); // Revenir au début de l'enregistrement
                fwrite(record, header->lengthRecord, 1, file);
                printf("[DEBUG] Enregistrement %d modifié.\n", rec);
            }
        } // fin fread
    } // fin modifs

    rewind(file);
    fclose(file);

    // libérer la mémoire
    int d = 0;
    while (values[d] != NULL) {
        free(values[d]);
        d++;
    }
    free(values);
    free(whereCondition);
    free(header->columns);
    free(header);
}


void createIndex(const char* filename, Record* info) {
    // parcourir le fichier
    // stocker les valeurs correspondantes aux colonnes d'info dans un tableau d'entier
    /* le tableau se construit comme suit :
        index : ligne de la table
        tab[index] : nouvelle ligne dans table index selon l'ordre croissant (algo de tri)
    */
    // créer une nouvelle table et la remplir avec les données nouvelles
    // vérifier aussi si la colonne existe
    // penser à mettre en place une recherche dichotomique

}