#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <time.h>
#include <termios.h>
#include <fcntl.h>

/**
 * @file snake_game.c
 * @brief Jeu du serpent en mode console.
 * @author [Votre Nom]
 * @version 1.0
 * @date [Date du jour]
 *
 * Ce programme implémente un jeu du serpent. Le joueur contrôle un serpent qui se déplace
 * sur un plateau. Le but est de manger un certain nombre de pommes tout en évitant les obstacles
 * et les collisions avec les bordures (sauf les issues) ou avec le corps du serpent.
 * Le jeu se termine en cas de collision, de victoire (toutes les pommes mangées),
 * ou si le joueur déclare forfait.
 */

/* Définition des constantes */
#define LARGEURMAX 80
#define HAUTEURMAX 40
const int TAILLESERPENT = 10;
const int NBREPAVE = 4;
const int TAILLEPAVE = 5;
const int TEMPORISATION = 200000;
const int NBREPOMMESFINJEU = 10;
const int STARTSAFEZONEX = 15;
const int ENDSAFEZONEX = 48;
const int STARTSAFEZONEY = 5;
const int ENDSAFEZONEY = 7;
const int MAXTAILLESERPENT = 100;

/* Définition des caractères utilisés */
const char TETE = 'O';
const char CORPS = 'X';
const char POMME = '6';
const char ARRET = 'a';
const char DROITE = 'd';
const char GAUCHE = 'q';
const char HAUT = 'z';
const char BAS = 's';

/* Plateau de jeu */
char plateau[HAUTEURMAX][LARGEURMAX];
int tailleSerpent = TAILLESERPENT;

/* Position de la pomme */
int posX_pomme = -1, posY_pomme = -1;

/* Déclaration des fonctions */
void disableEcho();
void enableEcho();
int kbhit();
void dessinerPlateau(int lesX[], int lesY[]);
void initPlateau();
void placerPaves();
void ajouterPomme();
void progresser(int lesX[], int lesY[], char direction, bool *collision, bool *pommeMangee);

/**
 * @brief Entrée principale du programme.
 * @return EXIT_SUCCESS si le programme se termine correctement.
 */
int main() {
    int lesX[MAXTAILLESERPENT] = {40, 39, 38, 37, 36, 35, 34, 33, 32, 31};
    int lesY[MAXTAILLESERPENT] = {20, 20, 20, 20, 20, 20, 20, 20, 20, 20};
    char direction = DROITE;
    bool collision = false;
    bool pommeMangee = false;
    bool forfait = false;
    int pommesMangees = 0;

    initPlateau();
    placerPaves();
    ajouterPomme();
    dessinerPlateau(lesX, lesY);

    disableEcho();

    while (!collision && pommesMangees < NBREPOMMESFINJEU) {
        if (kbhit()) {
            char touche = getchar();
            if ((touche == DROITE && direction != GAUCHE) ||
                (touche == GAUCHE && direction != DROITE) ||
                (touche == HAUT && direction != BAS) ||
                (touche == BAS && direction != HAUT)) {
                direction = touche;
            }
            if (touche == ARRET) {
                forfait = true;
                break;
            }
        }
        progresser(lesX, lesY, direction, &collision, &pommeMangee);
        if (pommeMangee) {
            pommesMangees++;
            ajouterPomme();
        }
        dessinerPlateau(lesX, lesY);
        usleep(TEMPORISATION - pommesMangees * 10000); /** Augmenter la vitesse progressivement */
    }

    enableEcho();

    if (collision) {
        system("clear");
        printf("Collision détectée. Vous avez perdu.\n");
    } else if (pommesMangees >= NBREPOMMESFINJEU) {
        system("clear");
        printf("Vous avez gagné. Félicitations !\n");
    } else if (forfait) {
        system("clear");
        printf("Vous avez déclaré forfait. Dommage !\n");
    }

    return EXIT_SUCCESS;
}

/**
 * @brief Initialise le plateau avec les bordures et les issues.
 */
void initPlateau() {
    for (int i = 0; i < HAUTEURMAX; i++) {
        for (int j = 0; j < LARGEURMAX; j++) {
            if (i == 0 || i == HAUTEURMAX - 1) {
                plateau[i][j] = (j == LARGEURMAX / 2) ? ' ' : '#';
            } else if (j == 0 || j == LARGEURMAX - 1) {
                plateau[i][j] = (i == HAUTEURMAX / 2) ? ' ' : '#';
            } else {
                plateau[i][j] = ' ';
            }
        }
    }
}

/**
 * @brief Place une pomme sur une case vide aléatoire.
 */
void ajouterPomme() {
    srand(time(NULL));
    while (plateau[posY_pomme][posX_pomme] != ' ') {
        posX_pomme = rand() % (LARGEURMAX - 2) + 1;
        posY_pomme = rand() % (HAUTEURMAX - 2) + 1;
    }
    plateau[posY_pomme][posX_pomme] = POMME;
}

/**
 * @brief Place des pavés d'obstacles sur le plateau en dehors de la zone de sécurité.
 */
void placerPaves() {
    srand(time(NULL));
    for (int k = 0; k < NBREPAVE; k++) {
        int x, y;
        do {
            x = rand() % (LARGEURMAX - 10) + 2;
            y = rand() % (HAUTEURMAX - 10) + 2;
        } while (x > STARTSAFEZONEX && x < ENDSAFEZONEX &&
                 y > STARTSAFEZONEY && y < ENDSAFEZONEY);

        for (int i = 0; i < TAILLEPAVE; i++) {
            for (int j = 0; j < TAILLEPAVE; j++) {
                plateau[y + i][x + j] = '#';
            }
        }
    }
}

/**
 * @brief Dessine le plateau entier avec le serpent et les obstacles.
 * @param lesX Tableau des coordonnées x du serpent.
 * @param lesY Tableau des coordonnées y du serpent.
 */
void dessinerPlateau(int lesX[], int lesY[]) {
    for (int i = 0; i < tailleSerpent; i++) {
        plateau[lesY[i]][lesX[i]] = (i == 0) ? TETE : CORPS;
    }
    system("clear");
    for (int i = 0; i < HAUTEURMAX; i++) {
        for (int j = 0; j < LARGEURMAX; j++) {
            putchar(plateau[i][j]);
        }
        putchar('\n');
    }
}

/**
 * @brief Fait progresser le serpent d'une étape.
 * @param lesX Tableau des coordonnées x du serpent.
 * @param lesY Tableau des coordonnées y du serpent.
 * @param direction Direction du mouvement.
 * @param collision Indique si une collision a eu lieu.
 * @param pommeMangee Indique si une pomme a été mangée.
 */
void progresser(int lesX[], int lesY[], char direction, bool *collision, bool *pommeMangee) {
    int X = lesX[tailleSerpent - 1];
    int Y = lesY[tailleSerpent - 1];
    plateau[Y][X] = ' ';

    for (int i = tailleSerpent - 1; i > 0; i--) {
        lesX[i] = lesX[i - 1];
        lesY[i] = lesY[i - 1];
    }

    if (direction == DROITE) lesX[0]++;
    if (direction == GAUCHE) lesX[0]--;
    if (direction == HAUT) lesY[0]--;
    if (direction == BAS) lesY[0]++;

    if (lesX[0] == 0 && lesY[0] == HAUTEURMAX / 2) lesX[0] = LARGEURMAX - 2;
    else if (lesX[0] == LARGEURMAX - 1 && lesY[0] == HAUTEURMAX / 2) lesX[0] = 1;
    else if (lesY[0] == 0 && lesX[0] == LARGEURMAX / 2) lesY[0] = HAUTEURMAX - 2;
    else if (lesY[0] == HAUTEURMAX - 1 && lesX[0] == LARGEURMAX / 2) lesY[0] = 1;

    *collision = plateau[lesY[0]][lesX[0]] == '#' || plateau[lesY[0]][lesX[0]] == CORPS;
    *pommeMangee = (lesX[0] == posX_pomme && lesY[0] == posY_pomme);
}

/**
 * @brief Désactive l'écho des touches du clavier.
 */
void disableEcho() {
    struct termios tty;
    tcgetattr(STDIN_FILENO, &tty);
    tty.c_lflag &= ~ECHO;
    tcsetattr(STDIN_FILENO, TCSANOW, &tty);
}

/**
 * @brief Réactive l'écho des touches du clavier.
 */
void enableEcho() {
    struct termios tty;
    tcgetattr(STDIN_FILENO, &tty);
    tty.c_lflag |= ECHO;
    tcsetattr(STDIN_FILENO, TCSANOW, &tty);
}

/**
 * @brief Vérifie si une touche a été pressée.
 * @return 1 si une touche est pressée, 0 sinon.
 */
int kbhit() {
    struct termios oldt, newt;
    int ch, oldf;

    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);

    oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);

    ch = getchar();

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    fcntl(STDIN_FILENO, F_SETFL, oldf);

    if (ch != EOF) {
        ungetc(ch, stdin);
        return 1;
    }
    return 0;
}