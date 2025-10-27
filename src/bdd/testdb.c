#include "testdb.h"
void vider_buffer() {
    int c;
    // Lire et ignorer tous les caractères jusqu'à la fin de la ligne
    while ((c = getchar()) != '\n' && c != EOF);
}
int uidfinder(const char* filename) {
    FILE* file = fopen(filename, "rb");
    if (!file) {
        printf("Erreur d'ouverture fichier\n");
        return -1;
    }
    users din;
    int uid = 1;
    while (fread(&din, sizeof(users), 1, file) == 1) {
        if (din.uid == uid++) {}
        else {
            break; // uid trouvé
        }
    }
    fclose(file);
    return uid;
}

int saisieuser(const char* filename, users user) {
    FILE* file = fopen(filename, "ab+"); // Ouvrir en mode ajout binaire
    users din;
    int username_exists = 0,index=0; // Variable pour vérifier si le nom d'utilisateur existe déjà
    char q;
    if (!file) {
        printf("Erreur d'ouverture du fichier\n");
        return 0;
    }
    do{
        username_exists=0;
        user.username[0] = '\0'; // Initialiser la chaîne à une chaîne vide
        printf("Saisir votre nom d'utilisateur:\n");
        // Réinitialiser le fichier à son début pour vérifier tous les utilisateurs
        while((q=getchar())!='\n' && strlen(user.username)<maxlen-1){
            strncat(user.username,&q,1);
            index++;
        }
        if(index>=49){  
            while(getchar()!='\n');
        }
        //vider_buffer();
        user.username[index]='\0';
        rewind(file);
        while(fread(&din,sizeof(users),1,file)==1){
            if (strcmp(din.username, user.username) == 0) {
                printf("Nom d'utilisateur déjà existant, veuillez en choisir un autre.\n");
                username_exists = 1;
                break;
            }
        }
    }while(username_exists!=0);
    index=0;//réinit de index pour mdp
    printf("Saisissez votre mot de passe:\n");
    user.mdp[0]='\0';
    while((q=getchar())!='\n'&& strlen(user.mdp)<maxlen-1){
        strncat(user.mdp,&q,1);
        index++;
    }
    if(index>=49){  
        while(getchar()!='\n');
    }
    //vider_buffer();
    user.mdp[index]='\0';
   // Initialiser les statistiques
    user.parties_jouees = 0; // Initialiser le nombre de parties jouées
    user.victoire = 0;       // Initialiser le nombre de victoires
    user.defaites = 0;       // Initialiser le nombre de défaites
    // Mettre la sauvegarde dans un fichier user ici
    user.uid = uidfinder(filename);
    fwrite(&user, sizeof(users), 1, file);
    fclose(file);

    return 1;
}//creation de compte

void afficherUtilisateurs(const char* filename) {
    FILE* file = fopen(filename, "rb"); // Ouvrir en mode lecture binaire
    if (!file) {
        printf("Erreur d'ouverture du fichier\n");
        return;
    }

    users din;
    printf("--------------------------\n");
    while (fread(&din, sizeof(users), 1, file) == 1) {
        // Afficher les informations de l'utilisateur
        printf("UID: %d\n", din.uid);
        printf("Nom d'utilisateur: %s\n", din.username);
        printf("Mot de passe: %s\n", din.mdp);
        if(din.parties_jouees>0){
            printf("Ratio(WR): %.2f\n", (double) din.victoire / din.parties_jouees);//arrondi à 2 décimales pour la lisibilité
            printf("WINS:%d\n",din.victoire);
            printf("LOSES:%d\n",din.parties_jouees-din.victoire);
            printf("nombre de parties jouees:%d\n",din.parties_jouees);
        }
        printf("--------------------------\n");
    }

    fclose(file);
}
int connectionuser(users *user, const char* filename) {
    FILE* file = fopen(filename, "rb"); // Ouvrir en mode lecture binair
    if (!file) {
        printf("Erreur d'ouverture du fichier\n");
        return 0;
        }
        users din;
        char q;
        while (fread(&din, sizeof(users), 1, file) == 1){
            if (strcmp(din.username, user->username) == 0 && strcmp(din.mdp, user->mdp)!=0){
                printf("Mot de passe incorrect\n");
                return 0;
            }
            if (strcmp(din.username, user->username) == 0 && strcmp(din.mdp, user->mdp) == 0){
                printf("Vous êtes connecté\n");
                user->defaites=din.defaites;
                user->parties_jouees=din.parties_jouees;
                user->victoire=din.victoire;
                user->uid=din.uid;
                return 1;
            }
        }
        return 0;
        fclose(file);
        printf("Identifiant ou mot de passe incorrect\n");
}//faire une option pour jouer en état connecté ou déco(active désactive les logs)
int add_game_record(const char* filename, GameRecord* record) {
    FILE* file = fopen(filename, "ab");
    if (!file) {
        printf("fichier non trouvé\n");
        return -1;
    }
    fwrite(record, sizeof(GameRecord), 1, file);
    fclose(file);
    return 0;
}//ajouter des logs dans le déroulement de la partie pour ensuite les ajouter avec cette fonction
int saisieuserlogins(users *user) {
    int index=0; // Variable pour vérifier si le nom d'utilisateur existe déjà
    char q;
    user->username[0] = '\0'; // Initialiser la chaîne à une chaîne vide
    printf("Saisir votre nom d'utilisateur:\n");
    // Réinitialiser le fichier à son début pour vérifier tous les utilisateurs
    while((q=getchar())!='\n' && strlen(user->username)<maxlen-1){
        strncat(user->username,&q,1);
        index++;
    }
    if(index>=49){  
        while(getchar()!='\n');
    }
    //vider_buffer();
    user->username[index]='\0';
    index=0;//réinit de index pour mdp
    printf("Saisissez votre mot de passe:\n");
    user->mdp[0]='\0';
    while((q=getchar())!='\n'&& strlen(user->mdp)<maxlen-1){
        strncat(user->mdp,&q,1);
        index++;
    }
    if(index>=49){  
        while(getchar()!='\n');
    }
    //vider_buffer();
    user->mdp[index]='\0';
   // Initialiser les statistiques
    user->parties_jouees = 0; // Initialiser le nombre de parties jouées
    user->victoire = 0;       // Initialiser le nombre de victoires
    user->defaites = 0;       // Initialiser le nombre de défaites
    // Mettre la sauvegarde dans un fichier user ici
    user->uid =1;
    return 1;
}//creation de compte
