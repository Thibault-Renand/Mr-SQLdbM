#ifndef UTILS_H
#define UTILS_H

#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>


#ifdef _WIN32
   #include <windows.h>
   #include <locale.h>

   // Code d'échappement couleur
   #define RESET L"\033[0m"
   #define BLACK L"\033[30m"
   #define RED L"\033[31m"
   #define BGRED L"\033[41m"
   #define GREEN L"\033[32m"
   #define BGGREEN L"\033[42m"
   #define BLUE L"\033[34m"
   #define CYAN L"\033[36m"
   #define GRAY L"\033[37m"
#else
   #include <unistd.h>  // Linux/Unix

   // Code d'échappement couleur
   #define RESET "\033[0m"
   #define BLACK "\033[30m"
   #define RED "\033[31m"
   #define BGRED "\033[41m"
   #define GREEN "\033[32m"
   #define BGGREEN "\033[42m"
   #define BLUE "\033[34m"
   #define CYAN "\033[36m"
   #define GRAY "\033[37m"
#endif

// Macro fonctions
#define MAX(x,y) \
   ({ typeof (x) _x = (x); \
      typeof (y) _y = (y); \
      _x > _y ? _x : _y; })
#define MIN(x,y) \
   ({ typeof (x) _x = (x); \
      typeof (y) _y = (y); \
      _x < _y ? _x : _y; })


typedef struct {

} Date;

typedef struct {
	char line;
	int column;
} Coord;


void clearInputBuffer();
void waitForKeyPress();
int findIndex(char* arr, int size, char target);
int findMax(int* arr, int size);
int findMin(int* arr, int size);
void afficher_progressivement_texte(const wchar_t *texte, int delai_ms);

#endif
