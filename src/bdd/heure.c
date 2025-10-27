#include "heure.h"


/*
    source : https://c.developpez.com/faq/?page=Gestion-des-dates-et-heures
    compilation : gcc -Wall heure.c -o hr
*/
  
const char * NomJourSemaine[] = {"Dimanche", "Lundi", "Mardi", "Mercredi", "Jeudi", "Vendredi", "Samedi"}; 
  
const char * NomMois[] = {"janvier", "fevrier", "mars"     , "avril"  , "mai"     , "juin"    , 
                          "juillet", "aout"   , "septembre", "octobre", "novembre", "decembre"}; 

struct tm* getCurrentDate() {
    time_t timestamp; 
    struct tm * t; 
  
    timestamp = time(NULL); 
    t = localtime(&timestamp);

    return t;
}

/*
int main(void) 
{ 

    struct tm* t = getCurrentDate();

    // Affiche la date et l'heure courante (format francais) 
    
    printf("Nous sommes %s, ", NomJourSemaine[t->tm_wday]); 
    printf("le %02u %s %04u, ", t->tm_mday, NomMois[t->tm_mon], 1900 + t->tm_year); 
    printf("et il est %02uh %02umin %02usec.\n", t->tm_hour, t->tm_min, t->tm_sec); 
  
    return 0; 
}
*/
