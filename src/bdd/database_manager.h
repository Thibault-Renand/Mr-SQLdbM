#ifndef DATABASE_MANAGER_H
#define DATABASE_MANAGER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>

#define RECORD_ACTIVE ' '    // Caractère pour un enregistrement actif
#define RECORD_DELETED '*'   // Caractère pour un enregistrement supprimé
#define MAX_FILENAME 256     // Taille maximale d'un nom de fichier
#define MAX_LENGTH_QUERY 1024  // Taille maximale requête
#define ERROR_FILE_NOT_FOUND -1
#define ERROR_INVALID_RECORD -2
#define WRONG_TYPE -3        // type non reconnu pour création de table


#include "../utils.h"  // Pour les couleurs
#include "heure.h"  // Pour avoir l'heure exacte FRANCAISE (et non celle du système)


typedef struct date {
    short year;  // Année YYYY
    short month; // Mois MM
    short day;   // Jour JJ
} Date;


typedef struct {
    char name[20];  // Nom de la colonne
    char type[20];  // Type de la colonne : FLOAT, INT, VARCHAR etc
    int length;     // Taille de l'enregistrement lorsque type est VARCHAR ou équivalent
} Column;


typedef struct {
    Date lastUpdate;  // Date de dernière modification
    int nbRecords;    // Nombre d'enregistrements actuels
    int lengthRecord; // Taille d'un enregistrement (fixe)
    int nbColumns;    // Nombre de colonnes max
    Column* columns;  // Tableau dynamique (taille majorée par nbColumns) des colonnes de la table
} Header;


// Structure rajoutée (copiée) pour éviter des erreurs de mémoire à cause de Column*
typedef struct {
    Date lastUpdate;
    int nbRecords;
    int lengthHeader;
    int lengthRecord;
    int nbColumns;
} HeaderFixed;


// Structure du parseur pour l'edition des tables
// cf. newSQL.h pour exemple de structure de parseur
// après appel de read_query()
typedef struct {
    char** fields;
} Record;


// Renvoie tout simplement le header sous son type générique
// Penser à free() le header après utilisation :)
Header* loadFileHeader(const char* filename);

// Fonctions générales pour manipuler la requête
char** getColumnTypes(Record* info);  // finit par NULL
char** getColumnNames(Record* info);  // finit par NULL
char** getRecordValues(Record* info); // finit par NULL

// Fonctions de requête
void createTable(const char* filename, Record* info);    // CREATE TABLE
//void createTable(const char* filename, Header* header);
void insertInto(const char* filename, Record* info);     // INSERT INTO
void deleteRecord(const char* filename, Record* info);   // DELETE FROM
void updateRecord(const char* filename, Record* info);   // UPDATE .. SET .. WHERE .. - NON FINI
void dropTable(const char* filename);                  // DROP TABLE 
void alterTableAdd(const char* filename, Record* info);  // ALTER TABLE .. ADD ..
void alterTableDrop(const char* filename, Record* info); // ALTER TABLE .. DROP ..

// Fonctions utilitaires
void addRecord(const char* filename, Record* info, int accumulateur); // ajoute un unique enregistrement
void displayRecords(const char* filename);  // affichage de tous les enregistrements
void displayHeader(Header* header);
int countColumns(Record* info);        // pour avoir le nombre de colonnes
int mesureRecord(Record* info);        // pour calculer la taille des enregistrements (en octets)
int getVarcharValue(char* varchar);  // pour récupérer la taille passée dans VARCHAR
char* getValueBetweenSeparator(char separator[], char* string);  // entre deux séparateurs
bool isInString(char target, char text[]);  // vérifie si un caractère est dans un texte
int tailleDynamiqueAddRecord(Header* header, int accumulateur);  // fonction auxiliaire de addRecord()
// void* compValue -> char* : problèmes de cast...
bool exeWhere(void* colValue, char* operateur, char* compValue, char* type); // implémentation du where

Record* header_to_edit(Header* header);
Record* record_to_edit(const char* filename);
void free_edit(Record* edit); 

#endif
