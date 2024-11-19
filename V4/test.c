/**
 * @file version4.c
 * @brief Implémentation de la version 4 du jeu Snake pour le projet SAE1.01.
 * 
 * Ce fichier contient le code de la quatrième version du jeu Snake. 
 * Les fonctionnalités incluent :
 * - Affichage des bordures avec issues.
 * - Placement de pavés aléatoires dans la zone de jeu.
 * - Gestion des collisions (bordures, pavés, serpent lui-même).
 * - Déplacement initial du serpent vers la droite jusqu'à appui sur une touche directionnelle.
 * - Gestion des pommes, allongement du serpent, et augmentation de la vitesse.
 * - Traversée des issues pour réapparaître de l'autre côté du plateau.
 * 
 * Le jeu se termine si le joueur appuie sur 'a' ou si une collision est détectée.
 * 
 * @author Arthur CHAUVEL
 * @version 4
 * @date 15/11/24
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>
#include <stdbool.h>
#include <time.h>

/** @brief Constantes globales. */

const int COORDMIN = 1;          /**< Coordonnée minimale sur le plateau. */
const int LARGEURMAX = 80;       /**< Largeur maximale du plateau. */
const int LONGUEURMAX = 40;      /**< Longueur maximale du plateau. */
const int TAILLEPAVE = 5;        /**< Taille des pavés (carrés). */
const int NBREPAVES = 4;         /**< Nombre de pavés à placer sur le plateau. */
const int COORDXDEPART = 40;    /**< Position de départ en X du serpent. */
const int COORDYDEPART = 20;    /**< Position de départ en Y du serpent. */
const int TAILLESERPENT = 10;    /**< Taille initiale du serpent. */
const int COMPTEURFINJEU = 10;   /**< Nombre de pommes à manger pour gagner. */
const char POMME = '6';          /**< Caractère représentant une pomme. */
const char CARBORDURE = '#';     /**< Caractère utilisé pour afficher les bordures. */
const char VIDE = ' ';           /**< Caractère représentant une case vide. */
const char TETE = 'O';           /**< Caractère représentant la tête du serpent. */
const char CORPS = 'X';          /**< Caractère représentant le corps du serpent. */
const char HAUT = 'z';           /**< Touche pour déplacer le serpent vers le haut. */
const char BAS = 's';            /**< Touche pour déplacer le serpent vers le bas. */
const char GAUCHE = 'q';         /**< Touche pour déplacer le serpent à gauche. */
const char DROITE = 'd';         /**< Touche pour déplacer le serpent à droite. */
const char FINJEU = 'a';         /**< Touche pour arrêter le jeu. */

int TEMPORISATION = 200000; /**< Temporisation entre les déplacements en microsecondes. */
int compteurPomme = 0; /**< Compteur de pommes mangées. */
int tailleSerpent = TAILLESERPENT; /**< Taille actuelle du serpent. */

/** @typedef plateau_de_jeu
 * @brief Définition du plateau de jeu comme une matrice de caractères.
 */
typedef char plateau_de_jeu[LARGEURMAX + 1][LONGUEURMAX + 1];

plateau_de_jeu plateau; /**< Plateau de jeu global. */

/* Prototypes des fonctions */
void afficher(int x, int y, char c);
void effacer(int x, int y);
void initPlateau();
void initPaves(int lesX[], int lesY[]);
void affichagePlateau(plateau_de_jeu plateau);
void dessinerSerpent(int lesX[], int lesY[]);
void progresser(int lesX[], int lesY[], char direction, bool *collision, bool *mangerPomme);
void ajouterPomme();
void gotoXY(int x, int y);
int kbhit(void);
void disableEcho();
void enableEcho();

/**
 * @brief Programme principal du jeu Snake.
 * 
 * Initialise le plateau de jeu, place le serpent et les pavés, puis lance la boucle principale.
 * 
 * @return EXIT_SUCCESS en cas de succès.
 */
int main() {
    srand(time(NULL));
    int lesX[LARGEURMAX], lesY[LONGUEURMAX];
    char touche = DROITE;
    char direction = DROITE;
    bool collision = false, mangerPomme = false;

    initPlateau();
    system("clear");
    disableEcho();

    // Initialisation du serpent
    for (int i = 0; i < tailleSerpent; i++) {
        lesX[i] = COORDXDEPART - i;
        lesY[i] = COORDYDEPART;
    }

    affichagePlateau(plateau);
    ajouterPomme();

    // Boucle principale du jeu
    do {
        if (kbhit()) {
            touche = getchar();
        }

        // Mise à jour de la direction
        if (touche == HAUT && direction != BAS) direction = HAUT;
        if (touche == BAS && direction != HAUT) direction = BAS;
        if (touche == GAUCHE && direction != DROITE) direction = GAUCHE;
        if (touche == DROITE && direction != GAUCHE) direction = DROITE;

        progresser(lesX, lesY, direction, &collision, &mangerPomme);
        usleep(TEMPORISATION);

    } while (touche != FINJEU && !collision && compteurPomme < COMPTEURFINJEU);

    enableEcho();
    system("clear");

    // Affichage de fin de partie
    if (compteurPomme >= COMPTEURFINJEU) {
        printf("Félicitations ! Vous avez gagné en mangeant %d pommes !\n", compteurPomme);
    } else {
        printf("Game Over !\n");
    }

    return EXIT_SUCCESS;
}

/**
 * @brief Initialise le plateau avec des bordures et des issues.
 */
void initPlateau() {
    for (int lig = 0; lig <= LARGEURMAX; lig++) {
        for (int col = 0; col <= LONGUEURMAX; col++) {
            if ((lig == COORDMIN || lig == LARGEURMAX) && col == LONGUEURMAX / 2) {
                plateau[lig][col] = VIDE; // Issue en haut et en bas
            } else if ((col == COORDMIN || col == LONGUEURMAX) && lig == LARGEURMAX / 2) {
                plateau[lig][col] = VIDE; // Issue à gauche et à droite
            } else if (lig == COORDMIN || lig == LARGEURMAX || col == COORDMIN || col == LONGUEURMAX) {
                plateau[lig][col] = CARBORDURE;
            } else {
                plateau[lig][col] = VIDE;
            }
        }
    }
}

/**
 * @brief Place une pomme à une position aléatoire non occupée.
 */
void ajouterPomme() {
    int x, y;
    bool emplacementValide;

    do {
        x = rand() % (LARGEURMAX - 1) + 1;
        y = rand() % (LONGUEURMAX - 1) + 1;
        emplacementValide = (plateau[x][y] == VIDE);
    } while (!emplacementValide);

    plateau[x][y] = POMME;
    afficher(x, y, POMME);
}

/**
 * @brief Met à jour la position du serpent et vérifie les collisions.
 */
void progresser(int lesX[], int lesY[], char direction, bool *collision, bool *mangerPomme) {
    effacer(lesX[tailleSerpent - 1], lesY[tailleSerpent - 1]);

    // Déplacement du corps
    for (int i = tailleSerpent - 1; i > 0; i--) {
        lesX[i] = lesX[i - 1];
        lesY[i] = lesY[i - 1];
    }

    // Déplacement de la tête
    if (direction == DROITE) lesX[0]++;
    if (direction == GAUCHE) lesX[0]--;
    if (direction == HAUT) lesY[0]--;
    if (direction == BAS) lesY[0]++;

    // Traversée des issues
    if (lesX[0] < COORDMIN) lesX[0] = LARGEURMAX - 1;
    if (lesX[0] >= LARGEURMAX) lesX[0] = COORDMIN;
    if (lesY[0] < COORDMIN) lesY[0] = LONGUEURMAX - 1;
    if (lesY[0] >= LONGUEURMAX) lesY[0] = COORDMIN;

    // Vérification des collisions
    if (plateau[lesX[0]][lesY[0]] == CARBORDURE || plateau[lesX[0]][lesY[0]] == CORPS) {
        *collision = true;
    }

    // Gestion des pommes
    if (plateau[lesX[0]][lesY[0]] == POMME) {
        *mangerPomme = true;
        plateau[lesX[0]][lesY[0]] = VIDE;
        ajouterPomme();
    }

    if (*mangerPomme) {
        compteurPomme++;
        tailleSerpent++;
        TEMPORISATION -= 10000; // Augmentation de la vitesse
        *mangerPomme = false;
    }

    dessinerSerpent(lesX, lesY);
}