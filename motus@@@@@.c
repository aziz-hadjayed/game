#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#define ANSI_COLOR_RESET "\x1b[0m"
#define ANSI_COLOR_RED "\x1b[31m"
#define ANSI_COLOR_GREEN "\x1b[32m"
#define ANSI_COLOR_BLUE "\x1b[34m"
//***********couleur************************************
void afficherResultat(const char *motJoueur, const char *motSecret)
{
    int LONGUEUR_MOT = strlen(motSecret);
    int c=0;
    for (int i = 0; i < LONGUEUR_MOT; i++)
    {
        if (motJoueur[i] == motSecret[i])
        {
            printf(ANSI_COLOR_GREEN "%c " ANSI_COLOR_RESET, motJoueur[i]);
        }
        else if (strchr(motSecret, motJoueur[i]))
        {
            c++;
            if(c>1)
            {
            printf(ANSI_COLOR_BLUE "%c " ANSI_COLOR_RESET, motJoueur[i]);
            }
            else if((c=1) &&( motJoueur[i] == motSecret[i]))
            {
               printf(ANSI_COLOR_GREEN "%c " ANSI_COLOR_RESET, motJoueur[i]);
            }
            else if ((c=1)&&(motJoueur[i] != motSecret[i]))
            {
                printf(ANSI_COLOR_RED "%c " ANSI_COLOR_RESET, motJoueur[i]);
            }
            else
            {
                printf(ANSI_COLOR_BLUE "%c " ANSI_COLOR_RESET, motJoueur[i]);
            }
        }
        else
        {
            printf(ANSI_COLOR_RED "%c " ANSI_COLOR_RESET, motJoueur[i]);
        }
    }
    printf("\n");

}

//----****** mots par defaut******-------------------------------------

void MotsAleatoire(FILE* fichier)
{
    const char* motsSimples[] = {
        "chat", "loup","lion","cerf"
    };
    const char* motsNormaux[] = {
        "chine", "maroc","tchad","italie"
    };
    const char* motsDifficiles[] = {
        "poireau", "carotte","fenouil","pasteque"
    };
    int nombreMots = sizeof(motsSimples) / sizeof(motsSimples[0]);

    for (int i = 0; i < nombreMots; i++)
        fprintf(fichier, "%s\n", motsSimples[i]);
    for (int i = 0; i < nombreMots; i++)
        fprintf(fichier, "%s\n", motsNormaux[i]);
    for (int i = 0; i < nombreMots; i++)
        fprintf(fichier, "%s\n", motsDifficiles[i]);
}
//----******-partie joueur--*****------------------------------------------

int jouerPartie(FILE* fichier, int joueur )
{
//--------------------si le fichier est vide -------------------------------------
    int NbMot = 0;
    char mot[10];
    while (fscanf(fichier, "%s", mot) != EOF)
    {
        NbMot++;
    }
    rewind(fichier);

    if (NbMot == 0)
    {
        fclose(fichier);
        fichier = fopen("my_file.txt", "a");
        if (fichier != NULL)
        {
            MotsAleatoire(fichier);
            fclose(fichier);
        }
        fichier = fopen("my_file.txt", "r");
    }
  //------------------niveau de jeux-------------------------------------------------
    int niveau;
    printf("Joueur %d, choisissez le niveau:  \n0pour facile(des animaux) \n1 pour normal(des pays) \n2 pour difficile(des legume et des fruits) : ", joueur);
    scanf("%d", &niveau);

    int longueurMin, longueurMax;
    if (niveau == 0) {
        longueurMin = 3;
        longueurMax = 4;
    } else if (niveau == 1) {
        longueurMin = 5;
        longueurMax = 6;
    } else if (niveau == 2) {
        longueurMin = 7;
        longueurMax = 8;
    } else {
        printf("Choix de niveau non valide. Veuillez choisir: \n0pour facile(des animaux) \n1 pour normal(des pays) \n2 pour difficile(des legume et des fruits) ");
        return 0;
    }
//---------------verification ---------------------------------------------------------------------------
    int motsSimple = 0, motsNormal = 0, motsDifficile = 0;

    rewind(fichier);

    while (fscanf(fichier, "%s", mot) != EOF)
    {
        int motLength = strlen(mot);

        if (motLength >= 3 && motLength <= 4)
        {
            motsSimple++;
        }
        else if (motLength >= 5 && motLength <= 6)
        {
            motsNormal++;
        }
        else if (motLength >= 7 && motLength <= 8 )
        {
            motsDifficile++;
        }
    }
        if (motsSimple == 0 || motsNormal == 0 || motsDifficile == 0) {
        printf("Le fichier ne contient pas suffisamment de mots de chaque niveau. L'administrateur doit ajouter des mots de chaque niveau.\n");
        return 0;
    }
//-----------mot al�a---------------------------------------------------------------------------------
    do {
        rewind(fichier);
        int indiceAleatoire = rand() % NbMot;
        for (int i = 0; i <= indiceAleatoire; i++)
        {
            fscanf(fichier, "%s", mot);
        }
    }
    while (strlen(mot) < longueurMin || strlen(mot) > longueurMax);
//------debut de jeux----------------------------------------------------------------------------------
    char tab2[50];
    strcpy(tab2, mot);
    int LONGUEUR_MOT = strlen(mot);
    char* motSecret = tab2;
    int tentative = 0;
    printf("\nJoueur %d, voici le mot a deviner : %c", joueur, tab2[0]);
    for (int f = 1; f < LONGUEUR_MOT; f++)
        printf("-");
    while (1)
    {
        char motJoueur[LONGUEUR_MOT];
        printf("\nJoueur %d, entrez un mot de %d lettres :\n ", joueur, LONGUEUR_MOT);
        scanf("%s", motJoueur);

        if (strlen(motJoueur) != LONGUEUR_MOT)
        {
            printf("Le mot doit avoir %d lettres.\n", LONGUEUR_MOT);
            continue;
        }

        tentative++;
        afficherResultat(motJoueur, motSecret);

        if (strcmp(motJoueur, motSecret) == 0)
        {
            printf("felicitation , Joueur %d a devine le mot en %d essais: %s\n", joueur, tentative, motSecret);
            return tentative;
        }
        else
        {
            printf("Le mot %s n'est pas le mot secret.\n", motJoueur);
        }
    }
}

void doubleJouerPartie(FILE* fichier, int* score1,int* socre2 )
{
//--------------------si le fichier est vide -------------------------------------
    int NbMot = 0;
    char mot[10];
    while (fscanf(fichier, "%s", mot) != EOF)
    {
        NbMot++;
    }
    rewind(fichier);

    if (NbMot == 0)
    {
        fclose(fichier);
        fichier = fopen("my_file.txt", "a");
        if (fichier != NULL)
        {
            MotsAleatoire(fichier);
            fclose(fichier);
        }
        fichier = fopen("my_file.txt", "r");
    }
  //------------------niveau de jeux-------------------------------------------------
    int niveau;
    printf("Joueur, choisissez le niveau:  \n0pour facile(des animaux) \n1 pour normal(des pays) \n2 pour difficile(des legume et des fruits) : ");
    scanf("%d", &niveau);

    int longueurMin, longueurMax;
    if (niveau == 0) {
        longueurMin = 3;
        longueurMax = 4;
    } else if (niveau == 1) {
        longueurMin = 5;
        longueurMax = 6;
    } else if (niveau == 2) {
        longueurMin = 7;
        longueurMax = 8;
    } else {
        printf("Choix de niveau non valide. Veuillez choisir: \n0pour facile(des animaux) \n1 pour normal(des pays) \n2 pour difficile(des legume et des fruits) \n");
        return 0;
    }
//---------------verification ---------------------------------------------------------------------------
    int motsSimple = 0, motsNormal = 0, motsDifficile = 0;

    rewind(fichier);

    while (fscanf(fichier, "%s", mot) != EOF)
    {
        int motLength = strlen(mot);

        if (motLength >= 3 && motLength <= 4)
        {
            motsSimple++;
        }
        else if (motLength >= 5 && motLength <= 6)
        {
            motsNormal++;
        }
        else if (motLength >= 7 && motLength <= 8 )
        {
            motsDifficile++;
        }
    }
        if (motsSimple == 0 || motsNormal == 0 || motsDifficile == 0) {
        printf("Le fichier ne contient pas suffisamment de mots de chaque niveau. L'administrateur doit ajouter des mots de chaque niveau.\n");
        return 0 ;
    }
//-----------mot al�a---------------------------------------------------------------------------------
    do {
        rewind(fichier);
        int indiceAleatoire = rand() % NbMot;
        for (int i = 0; i <= indiceAleatoire; i++)
        {
            fscanf(fichier, "%s", mot);
        }
    }
    while (strlen(mot) < longueurMin || strlen(mot) > longueurMax);
//------debut de jeux----------------------------------------------------------------------------------
    char tab2[50];
    strcpy(tab2, mot);
    int LONGUEUR_MOT = strlen(mot);
    char* motSecret = tab2;
    int tentative1 = 0;
    int tentative2 = 0;
    printf("\nJoueurs, voici le mot a deviner : %c", tab2[0]);
    for (int f = 1; f < LONGUEUR_MOT; f++)
        printf("-");
    int k=0;
    while (k<=6)
    {

        k++;
        char motJoueur1[LONGUEUR_MOT];
        char motJoueur2[LONGUEUR_MOT];
        printf("\nJoueur 1, entrez un mot de %d lettres :\n ",LONGUEUR_MOT);
        scanf("%s", motJoueur1);

        if (strlen(motJoueur1) != LONGUEUR_MOT)
        {
            printf("Le mot doit avoir %d lettres.\n", LONGUEUR_MOT);
            continue;
        }

        tentative1++;
        afficherResultat(motJoueur1, motSecret);

        if (strcmp(motJoueur1, motSecret) == 0)
        {
            printf("felicitation , Joueur 1 a devine le mot en %d essais: %s\n", tentative1, motSecret);
            *score1 =6- tentative1;
            break;
        }
        else
        {
            printf("Le mot '%s' n'est pas le mot secret.\n", motJoueur1);
        }
       replay:{ printf("\nJoueur 2 , entrez un mot de %d lettres :\n ", LONGUEUR_MOT);}
        scanf("%s", motJoueur2);
if (strlen(motJoueur2) != LONGUEUR_MOT)
        {
            printf("Le mot doit avoir %d lettres.\n", LONGUEUR_MOT);
            goto replay;
            continue;

        }
         tentative2++;
        afficherResultat(motJoueur2, motSecret);
        if (strcmp(motJoueur2, motSecret) == 0)
        {
            printf("felicitation , Joueur 2 a devine le mot en %d essais: %s\n", tentative2, motSecret);
            *socre2 = 6-tentative2;
            break;
        }
         else
        {
            printf("Le mot %s n'est pas le mot secret.\n", motJoueur2);
        }
    }
}
//******admin*****************************************************
void admin(FILE* fichier)
{
    int n;
    int reglage;
    printf("choisir: \n0 pour ajouter \n1  pour supprimer : ");
    scanf("%d",&reglage);
    if(reglage==0){
    printf("Combien de mots souhaitez-vous ajouter ? : ");
    scanf("%d", &n);

    fichier = fopen("my_file.txt", "a");
      for (int h = 0; h < n; h++) {
            char motAdmin[10];
            printf("Mode administrateur - Ajouter un mot : ");
            scanf("%s", motAdmin);
            fprintf(fichier, "\n%s\n", motAdmin);
        }

        fclose(fichier);
        printf("Mots ajoutes avec succes.\n");}
    else
        if(reglage==1)
        {
        printf("Liste des mots existants :\n");
        char motLecture[50];
        rewind(fichier);

        while (fscanf(fichier, "%s", motLecture) != EOF)
        {
            printf("%s\n", motLecture);
        }
        printf("Entrez le mot que vous souhaitez supprimer : ");
        char motASupprimer[50];
        scanf("%s", motASupprimer);
        FILE* fichierTemp = fopen("my_file_temp.txt", "w");
        if (fichierTemp != NULL)
        {
            rewind(fichier);
            while (fscanf(fichier, "%s", motLecture) != EOF) {
                if (strcmp(motLecture, motASupprimer) != 0) {
                    fprintf(fichierTemp, "%s\n", motLecture);
                }
            }
            fclose(fichier);
            fclose(fichierTemp);
            remove("my_file.txt");
            rename("my_file_temp.txt", "my_file.txt");
            printf("Mot supprime avec succes.\n");
        }
    }
}


int main()
{
    srand(time(0));

    FILE *fichier = fopen("my_file.txt", "r");
    if (fichier == NULL)
    {
        printf("Impossible d'ouvrir le fichier.\n");
        return 1;
    }

    int scoreJoueur1 = 0;
    int scoreJoueur2 = 0;

    etiquette:{printf("Choisissez le mode \n0 pour administrateur \n1 pour jouer seul \n2 pour jouer a deux : ");}
    int mode;
    scanf("%d", &mode);

    if (mode == 0)
    {
       admin(fichier);
       goto etiquette;
    }
    else if (mode == 1)
    {
        int nombreDeParties;
        printf("Combien de parties souhaitez-vous jouer en solo ? : ");
        scanf("%d", &nombreDeParties);

        for (int i = 0; i < nombreDeParties; i++)
        {
            printf("\nPartie %d - Joueur 1 (Solo)\n", i + 1);
            scoreJoueur1 += jouerPartie(fichier, 1);
            printf("\nScore apres la partie %d - Joueur 1 (Solo) : %d\n", i + 1, scoreJoueur1);
        }
    }
    else if (mode == 2)
    {
        int nombreDeParties;
        printf("Combien de parties souhaitez-vous jouer a deux ? : ");
        scanf("%d", &nombreDeParties);

        for (int i = 0; i < nombreDeParties; i++)
        {
            printf("\nPartie %d - Joueur 1 vs. Joueur 2\n", i + 1);
            doubleJouerPartie(fichier,&scoreJoueur1,&scoreJoueur2);

            printf("\nScore apres la partie %d - Joueur 1 : %d, Joueur 2 : %d\n", i + 1, scoreJoueur1, scoreJoueur2);
        }
        if (scoreJoueur1<scoreJoueur2)
        {
            printf("\n felicitation, joueur 2 gagne \n");
        }
        else if (scoreJoueur1>scoreJoueur2)
        {
            printf("\n felicitation, joueur 1 gagne \n");
        }
        else if (scoreJoueur1==scoreJoueur2)
        {
            printf("\n la partie se termine en egalite \n");
        }

    }
    else
    {
        printf("Choix de mode non valide. Veuillez choisir 0 pour administrateur, 1 pour jouer seul, ou 2 pour jouer a deux.\n");
    }
    goto etiquette;

    fclose(fichier);

    return 0;
}

