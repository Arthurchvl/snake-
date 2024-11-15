/**
* @file version3.c
* @brief code de la version 3 du projet de SAE1.01, un jeu snake
* @author Arthur CHAUVEL
* @version 3.4.2
* @date 15/11/24
*
* troisième version du programme du snake permettant d'afficher les bordures ainsi que des "pavés" au niveau de la zone de jeu,
*
* les colisions sont maintenant prises en compte, ainsi une collision de la tete du serpent avec sa queue, la bordure ou un pavé
*
* arretera le jeu. Le jeu peut toujours etre arreté avec 'a' et le serpent se déplace initialement vers la droite  jusqu'a appui sur une touche directionnelle.
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>
#include <stdbool.h>
#include <time.h>

const int COORD_MIN = 1; 
const int LARGEUR_MAX  = 80; 
const int LONGUEUR_MAX = 40; 
const int TAILLE_PAVE = 5; 
const int NBRE_PAVES = 4;    
const int COORD_X_DEPART = 40; 
const int COORD_Y_DEPART = 20;
const int TAILLE_SERPENT = 10;         
const int TEMPORISATION = 200000; 
const char CAR_BORDURE = '#';    
const char VIDE = ' ';     
const char TETE = 'O';
const char CORPS = 'X';
const char HAUT = 'z';
const char BAS = 's';
const char GAUCHE = 'q';
const char DROITE = 'd';
const char FIN_JEU = 'a';

typedef char plateau_de_jeu[LARGEUR_MAX + 1][LONGUEUR_MAX + 1];

void afficher(int x, int y, char c);
void effacer(int x, int y);
void initPlateau(plateau_de_jeu tableau);
void initPaves(plateau_de_jeu tableau);
void generationPavés(int x, int y);
void affichagePlateau(plateau_de_jeu tableau);
char definirDirection(char touche, char diection);
void dessinerSerpent(int lesX[], int lesY[]);
void progresser(int lesX[], int lesY[], char direction, bool *colision, plateau_de_jeu tableau);
void gotoXY(int x, int y);
int kbhit(void);
void disableEcho();
void enableEcho();


int main(){

    srand(time(NULL));
    plateau_de_jeu plateau;
    int x, y;
    int lesX[TAILLE_SERPENT], lesY[TAILLE_SERPENT];
    char touche = DROITE;
    char direction = DROITE;

    initPlateau(plateau);
    system("clear");
    disableEcho();

    bool collision = false;

    x = COORD_X_DEPART;
    y = COORD_Y_DEPART;
    for (int i = 0; i < TAILLE_SERPENT; i++)
    {
        lesX[i] = x--;
        lesY[i] = y;
    }
    affichagePlateau(plateau);

    do
    {
        if (kbhit())
        {
            touche = getchar();
        }

        if (touche == HAUT && direction != BAS) {
            direction = HAUT;
        }

        else if ((touche == BAS) && (direction != HAUT)) {
            direction = BAS;
        }

        else if ((touche == GAUCHE) && (direction != DROITE)) {
            direction = GAUCHE;
        }

        else if ((touche == DROITE) && (direction != GAUCHE)) {
            direction = DROITE;
        }

        progresser(lesX, lesY, direction, &collision, plateau);
        usleep(TEMPORISATION);

    } while ((touche != FIN_JEU) && (collision != true));

    enableEcho();
    system("clear");
    return EXIT_SUCCESS;
}

void afficher(int x, int y, char c)
{

    if (((y >= COORD_MIN) && (y <= LONGUEUR_MAX + 1)) && ((x >= COORD_MIN) && (x <= LARGEUR_MAX + 1))) {
        gotoXY(x, y);
        printf("%c", c);
    }
}

void effacer(int x, int y)
{
    afficher(x, y, VIDE);
}
void initPlateau(plateau_de_jeu plateau)
{
    for (int lig = 0; lig <= LARGEUR_MAX; lig++)
    {
        for (int col = 0; col <= LONGUEUR_MAX; col++)
        {
            if (((lig == COORD_MIN) || (lig == LARGEUR_MAX)) || ((col == COORD_MIN) || (col == LONGUEUR_MAX)))
            {
                plateau[lig][col] = CAR_BORDURE;
            }
            else
            {
                plateau[lig][col] = VIDE;
            }
        }
    }
    for (int i = 0; i < NBRE_PAVES; i++)
    {
        initPaves(plateau);
    }
}
void initPaves(plateau_de_jeu plateau)
{
    int x, y;

    // Générer une position aléatoire pour le pavé, en évitant les bords et la zone du serpent
    x = rand() % (LARGEUR_MAX - TAILLE_PAVE - 6) + 3;  // Placer à au moins 3 cases des bords
    y = rand() % (LONGUEUR_MAX - TAILLE_PAVE - 6) + 3; // Placer à au moins 3 cases des bords

    // Placer le pavé
    for (int i = y; i < TAILLE_PAVE + y; i++)
    {
        for (int j = x; j < TAILLE_PAVE + x; j++)
        {
            plateau[j][i] = CAR_BORDURE;
        }
    }
}
void affichagePlateau(plateau_de_jeu plateau)
{
    for (int lig = 1; lig <= LARGEUR_MAX; lig++)
    {
        for (int col = 1; col <= LONGUEUR_MAX; col++)
        {
            afficher(lig, col, plateau[lig][col]);
        }
    }
}

void dessinerSerpent(int lesX[], int lesY[])
{
    afficher(lesX[0], lesY[0], TETE);
    for (int i = 1; i < TAILLE_SERPENT; i++)
    {
        afficher(lesX[i], lesY[i], CORPS);
    }
    fflush(stdout);
}

void progresser(int lesX[], int lesY[], char direction, bool *colision, plateau_de_jeu plateau)
{
    effacer(lesX[TAILLE_SERPENT - 1], lesY[TAILLE_SERPENT - 1]); 

    for (int i = TAILLE_SERPENT - 1; i > 0; i--)
    {
        lesX[i] = lesX[i - 1];
        lesY[i] = lesY[i - 1];
    }

    if (direction == DROITE)
    {
        lesX[0]++;
    }
    else if (direction == GAUCHE)
    {
        lesX[0]--;
    }
    else if (direction == HAUT)
    {
        lesY[0]--;
    }
    else if (direction == BAS)
    {
        lesY[0]++;
    }

    if (plateau[lesX[0]][lesY[0]] == CAR_BORDURE)
    {
        *colision = true;
    }

    for (int i = 1; i < TAILLE_SERPENT; i++)
    {
        if ((lesX[0] == lesX[i]) && (lesY[0] == lesY[i]))
        {
            *colision = true;
        }
    }
    dessinerSerpent(lesX, lesY);
}

/**
* Les procédures/fonction qui suivent sont des "boites noires" données dans l'énoncé de chaque version en nécéssitant l'usage,
* il n'y a donc pas de commentaires car il n'est pas nécéssaire de comprendre ce qu'elles font.
*/
void gotoXY(int x, int y)
{
    printf("\033[%d;%df", y, x);
}

int kbhit()
{
    int unCaractere = 0;
    struct termios oldt, newt;
    int ch;
    int oldf;

    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);

    ch = getchar();

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    fcntl(STDIN_FILENO, F_SETFL, oldf);

    if (ch != EOF)
    {
        ungetc(ch, stdin);
        unCaractere = 1;
    }
    return unCaractere;
}
void disableEcho()
{
    struct termios tty;

    if (tcgetattr(STDIN_FILENO, &tty) == -1)
    {
        perror("tcgetattr");
        exit(EXIT_FAILURE);
    }

    tty.c_lflag &= ~ECHO;

    if (tcsetattr(STDIN_FILENO, TCSANOW, &tty) == -1)
    {
        perror("tcsetattr");
        exit(EXIT_FAILURE);
    }
}

void enableEcho()
{
    struct termios tty;

    if (tcgetattr(STDIN_FILENO, &tty) == -1)
    {
        perror("tcgetattr");
        exit(EXIT_FAILURE);
    }

    tty.c_lflag |= ECHO;

    if (tcsetattr(STDIN_FILENO, TCSANOW, &tty) == -1)
    {
        perror("tcsetattr");
        exit(EXIT_FAILURE);
    }
} 