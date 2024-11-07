/**
* \page Général 
* 
* \author LE GUIC
* \version 1.0
* \date 13 septembre 2021
*
* Ce programme propose plusieurs opérations de manipulation
* de chaînes de caractères où les chaînes sont implémentées
* par des listes chainées de caractères.Programme de manipulation de chaînes de caractères.
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

/**
* \def TMAX
* \brief Taille maximale d’un tableau.
*
* Taille maximale d'une chaine quand on souhaite ordonner ses caractères.
*/
#define TMAX 20

/**
* \struct element
* \brief Structure d'un élément d'une liste chaînée.
*
* Un élément est composé d'un caractère et d'un pointeur sur le caractère suivant
* (ou qui vaut NULL si c'est le dernier). Cette structure de données est récursive.
*/
typedef struct Elem {
    char lettre;		/**< caractère de la chaîne */
    struct Elem * svt;  /**< pointeur sur l'élément suivant */
} element;

/**
* \typedef chaine
* \brief Pointeur sur le premier élément d'une liste.
*
* Le type chaine permet de déclarer des chaines de caractères en tant que liste chaînée.
*/
typedef element * chaine;

/**
* \typedef typTab
* \brief Type tableau de TMAX caractères.
*
* Le type typTab permet d'utiliser un tableau de stockage provisoire pour ordonner
* les éléments (caractères) d’une liste (voir fonction ord()).
*
*/
typedef char typTab[TMAX];



void init(chaine * ch);
bool estV(chaine ch);
void ajT(chaine * ch, char c);
void ajQ(chaine * ch, char c);
int nbC(chaine ch);
void aff(chaine ch);
void cat(chaine ch1, chaine ch2, chaine * ch3);
bool app(chaine ch, char c);
bool ide(chaine ch1, chaine ch2);
void inv(chaine ch1, chaine * ch2);
bool pal(chaine ch1);
bool ana(chaine ch1, chaine ch2);
void ord(chaine * ch);

/**
* \fn int main()
* \brief Programme principal.
* \return Code de sortie du programme (0 : sortie normale).
* 
* Le programme principal traite un exemple qui teste quelques fonctions :
* il crée deux chaines, les affiche, puis ordonne la deuxième et l'affiche.
*/

int main(){
    chaine ch1, ch2;
    init(&ch1);
    init(&ch2);

    ajQ(&ch1, 'C');
    ajQ(&ch1, 'H');
    ajQ(&ch1, 'I');
    ajQ(&ch1, 'E');
    ajQ(&ch1, 'N');
    aff(ch1);

    ajQ(&ch2, 'N');
    ajQ(&ch2, 'I');
    ajQ(&ch2, 'C');
    ajQ(&ch2, 'H');
    ajQ(&ch2, 'E');
    aff(ch2);
    ord(&ch2);
    aff(ch2);

    return EXIT_SUCCESS;
}

/**
* \fn void init(chaine * ch).
* \brief Fonction qui initialise la chaine.
* \param ch : paramètre de sortie qui représente la chaine à initialiser.
*
* Met à NULL la chaine passée en paramètre.
*/
void init(chaine * ch){
	/**
	 * Cette fonction permet d'initialiser une chaîne. Elle doit être appelée à chaque utilisation de chaîne.
	 */ 
    *ch=NULL;
}
/**
* \fn estV(chaine ch)
* \brief Fonction qui indique si une chaine est vide.
* \param ch : paramètre d'entrée qui représente la chaine à tester.
* \return true si la chaine est vide, false sinon.
*/
bool estV(chaine ch){
    return ch==NULL;
}

/**
* \fn void ajT(chaine * ch, char c)
* \brief Ajout d'un caractère en tête.
* \param ch : paramètre d'entrée/sortie qui représente la chaine à modifier.
* \param c: paramètre d'entrée qui représente le caractère à ajouter.
*
* Après une allocation dynamique, ajoute le nouvel élément au début de la liste chaînée.
*/
void ajT(chaine * ch, char c){
    element * nouveau = (element*)malloc(sizeof(element));
    nouveau->lettre = c;
    nouveau->svt = *ch;
    *ch = nouveau;
}

/**
* \fn void ajQ(chaine * ch, char c)
* \brief Ajout d'un caractère en queue.
* \param ch : paramètre d'entrée/sortie qui représente la chaine à modifier.
* \param c: paramètre d'entrée qui représente le caractère à ajouter.
*
* Après une allocation dynamique, ajoute le nouvel élément à la fin de la liste chaînée.
*/
void ajQ(chaine * ch, char c){
    element * ptCourant;
    element * nouveau = (element*)malloc(sizeof(element));
    nouveau->lettre = c;
    nouveau->svt = NULL;

    if (estV(*ch)){
        *ch = nouveau;
    } else {
        ptCourant = *ch;
        while (ptCourant->svt != NULL){
            ptCourant = ptCourant->svt;
        }
        ptCourant->svt = nouveau;
    }
}

/**
* \fn int nbC(chaine ch)
* \brief Calcule la taille d'une chaîne.
* \param ch : paramètre d'entrée qui représente la chaine dont on veut connaître la taille.
* \return le nombre de caractères présents dans la chaîne.
*
* Parcours complet de la liste chaînée pour compter le nombre de ses éléments.
*/
int nbC(chaine ch){
    int nb = 0;
    element * ptCourant = ch;
    while (ptCourant != NULL){
        ptCourant = ptCourant->svt;
        nb++;
    }
    return nb;
}

/**
* \fn void aff(chaine ch)
* \brief Affiche une chaîne à l'écran.
* \param ch : paramètre d'entrée qui représente la chaine à afficher.
*
* Parcours complet de la liste chaînée pour afficher chacun de ses éléments (le champ 'lettre').
*/
void aff(chaine ch){
    if (!estV(ch)){
        printf("%c", ch->lettre);
        aff(ch->svt);
    } else {
        printf("\n");
    }
}

/**
* \fn bool ide(chaine ch1, chaine ch2)
* \brief Indique si deux chaînes sont identiques.
* \param ch1 : paramètre d'entrée qui représente la première chaîne.
* \param ch2 : paramètre d'entrée qui représente la deuxième chaîne.
* \return true si les deux chaînes sont identiques, false sinon.
*
* Parcourt parallèlement les deux listes chaînées, caractère par caractère,
* tant que les caractères correspondants sont les mêmes.
*/
bool ide(chaine ch1, chaine ch2){
    element * ptCourant1 = ch1;
    element * ptCourant2 = ch2;
    bool pareil = true;

    while (pareil && ptCourant1!=NULL){
        if (ptCourant2==NULL){
            pareil = false;
        } else if (ptCourant1->lettre != ptCourant2->lettre){
            pareil= false;
        } else {
            ptCourant1 = ptCourant1->svt;
            ptCourant2 = ptCourant2->svt;
        }
    }
    return pareil && ptCourant2==NULL;

}

/**
* \fn void inv(chaine ch1, chaine * ch2)
* \brief Inverse l'ordre des caractères dans une chaîne.
* \param ch1 : paramètre d'entrée qui représente la chaîne à inverser.
* \param ch2 : paramètre de sortie qui représente la nouvelle chaîne construite.
*
* Parcourt la première liste et ajoute chaque élément en tête de la deuxième.
* ATTENTION : ch2 doit être initialisée par la procédure/fonction appelante.
*/
void inv(chaine ch1, chaine * ch2){
    element * ptCourant;

    ptCourant = ch1;
    while (ptCourant != NULL){
        ajT(ch2, ptCourant->lettre);
        ptCourant = ptCourant->svt;
    }
}

/**
* \fn bool pal(chaine ch1)
* \brief Indique si une chaîne est un palindrome.
* \param ch1 : paramètre d'entrée qui représente la chaîne à tester.
* \return true si les deux chaînes sont identiques, false sinon.
*
* Une chaîne est un palindrome si elle est identique à son inverse.
*/
bool pal(chaine ch1){
    chaine ch;

    init(&ch);
    inv(ch1, &ch);
    return ide(ch1, ch);
}

/**
* \fn bool app(chaine ch, char c)
* \brief Indique si un caractère fait partie d'une chaîne.
* \param ch : paramètre d'entrée qui représente la chaîne à tester.
* \param c : paramètre d'entrée qui représente le caractère à tester.
* \return true si le caractère est présent dans la chaîne, false sinon.
*
* Effectue une recherche séquentielle de c dans la liste chaînée ch.
*/
bool app(chaine ch, char c){
    bool trouve = false;
    element * ptCourant = ch;

    while (!trouve && ptCourant!=NULL){
        if (ptCourant->lettre==c){
            trouve = true;
        } else {
            ptCourant = ptCourant->svt;
        }
    }
    return trouve;
}

/**
* \fn bool supprimer(chaine * ch, char c)
* \brief Supprime un caractère d'une chaîne.
* \param ch : paramètre d'entrée/sortie qui représente la chaîne à modifier.
* \param c : paramètre d'entrée qui représente le caractère à enlever.
* \return true si la suppression a eu lieu (ie si le caractère est présent), false sinon.
*
* ATTENTION : cette fonction ne supprime que la première occurrence du caractère.
*/
bool supprimer(chaine * ch, char c){
    bool trouve = false;
    element * ptCourant = *ch;
    element * ptPrec = NULL;

    while (!trouve && ptCourant!=NULL){
        if (ptCourant->lettre==c){
            trouve = true;
        } else {
            ptPrec = ptCourant;
            ptCourant = ptCourant->svt;
        }
    }
    if (trouve){
        if (ptPrec==NULL) {
            *ch = ptCourant->svt;
        } else {
            ptPrec->svt = ptCourant->svt;
        }
        free(ptCourant);
        ptCourant = NULL;
    }
    return trouve;
}

/**
* \fn bool ana(chaine ch1, chaine ch2)
* \brief Indique si deux chaînes sont anagrammes l'une de l'autre.
* \param ch1 : paramètre d'entrée qui représente la première chaîne.
* \param ch2 : paramètre d'entrée qui représente le deuxième chaîne.
* \return true si les deux chaînes sont anagrammes l'une de l'autre, false sinon.
*
* La fonction teste si les deux chaînes sont composées des mêmes caractères. Elle parcourt la liste ch1.
* A chaque element, elle enlève dans ch2 le caractère courant correspondant * de ch1 s'il existe.
* S'il n'existe pas, le résultat est faux, arrêt du parcours. Mais si à la fin du parcours * de ch1,
* ch2 est vide alors c'est OK.
*
* ATTENTION, dans cette fonction la chaine ch2 est modifiée, donc plus utilisable à la fin.
*/
bool ana(chaine ch1, chaine ch2){
    bool anag = true;

	element * ptCourant = ch1;

    while (anag && ptCourant!=NULL){
        if (! supprimer(&ch2, ptCourant->lettre)){
            anag = false;
        }
        ptCourant = ptCourant->svt;
    }
    return anag && estV(ch2);
}

/**
* \fn int LvT(chaine ch, typTab t)
* \brief Recopie les caractères d'une chaîne dans un tableau.
* \param ch : paramètre d'entrée qui représente la chaîne à copier.
* \param t : paramètre d'entrée/sortie qui représente le tableau.
* \return La taille de la chaîne ou -1 si cette taille dépasse TMAX.
* \see ord()
*
* La fonction est utilisée par la fonction ord(). Elle copie les caractères d'une Liste
* dans un tableau A CONDITION que la taille de la chaîne soit inférieure ou égale à TMAX.
*/
int LvT(chaine ch, typTab t){
    int i=0;
    int N = nbC(ch);

    if (N<=TMAX){
        chaine pt = ch;
        while (pt!=NULL){
            t[i] = pt->lettre;
            i++;
            pt = pt->svt;
        }
    } else {
        N = -1;
    }
    return N;
}

/**
* \fn void TvL(typTab t, chaine * ch, int N)
* \brief Recopie les caractères d'une chaîne dans un tableau.
* \param t : paramètre d'entrée/sortie qui représente le tableau à recopier.
* \param ch : paramètre de sortie qui représente la chaîne résultat.
* \param N: paramètre d'entrée qui représente le nombre de caractères à recopier.
* \see ord()
* 
* La fonction est utilisée par la fonction ord(). Elle ajoute un à un les caractères du tableau
* à une liste (ajout en queue).
* ATTENTION : ch doit être initialisée par la procédure/fonction appelante.
*/
void TvL(typTab t, chaine * ch, int N){
    int i;

    for (i=0 ; i<N ; i++){
        ajQ(ch, t[i]);
    }
}

/**
* \fn void trT(typTab t, int N)
* \brief Trie les éléments d'un tableau de caractères.
* \param t : paramètre d'entrée/sortie qui représente le tableau à trier.
* \param N: paramètre d'entrée qui représente le nombre de caractères du tableau.
* \see ord()
*
* La fonction utilise le tri par insertion. Elle ordonne les éléments de manière croissante.
*/
void trT(typTab t, int N){
    int i, j;
    bool inf;
    char tmp;

    for(i=1;i<N;i++){
        j=i;
        inf=true;
        while (j>0 && inf){
            if(t[j] < t[j-1]){
                tmp = t[j];
                t[j] = t[j-1];
                t[j-1] = tmp;
                j--;
            }else{
                inf = false;
            }
        }
    }
}

/**
* \fn void ord(chaine * ch)
* \brief Ordonne les caractères d'une chaîne par ordre croissant.
* \param t : paramètre d'entrée/sortie qui représente la chaîne à ordonner.
*
* La fonction utilise les fonctions LvT, trT et TvL pour respectivement :
*   - copier les caractères de la liste dans un tableau,
*   - trier le tableau,
*   - et recopier les caractères du tableau dans la liste.
*
* ATTENTION : la chaine doit faire moins de TMAX caractères.
*/
void ord(chaine * ch){
    typTab t;
    int N = LvT(*ch, t);
    if (N!=-1){
        trT(t, N);
        *ch = NULL;
        TvL(t, ch, N);
    }
}
