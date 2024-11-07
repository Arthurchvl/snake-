// défintion des bibliothèques
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// définition de la constate TMAX à 20, la taille max du tableau
#define TMAX 20

// définition de la structure d'un élément dans la chaine de caractère Elem, un tableau
typedef struct Elem {
    char lettre;
    struct Elem * svt;
}element;

typedef element * chaine;

typedef char typTab[TMAX]; //créer un alias chaine pour représenter un pointeur vers l'élément suivant
// sert à manipuler une chaine de caractères sous forme de liste chainée  

void init(chaine * ch); //fonction void initialise une chaine vide
bool estV(chaine ch);   //booléen qui vérifie si la chaine de caractère est bien vide
void ajT(chaine * ch, char c);  //ajoute un élément contenant le caractère c au début de la chaine
void ajQ(chaine * ch, char c);  //ajoute un élément contenant le caractère c à la fin de la chaine. Si la chaine est vide, ça devient le premier
int nbC(chaine ch); //compte et renvoie le nombre d'éléments dans la chaine
void aff(chaine ch);  //affiche la chaine de caractère récursivement   
void cat(chaine ch1, chaine ch2, chaine * ch3);
bool app(chaine ch, char c);
bool ide(chaine ch1, chaine ch2);   //compare 2 chaines de caractères et renvoie True si elles sont identiques
void inv(chaine ch1, chaine * ch2); //inverse ch1 et la stocke dans ch2, en plus il ajoute caractère par caractère les éléments de ch1 au début de ch2
bool pal(chaine ch1);   //vérifie si la chaine de caractère est un palindrome
bool ana(chaine ch1, chaine ch2);   //vérifie si la chaine de caractère est un anagramme
void ord(chaine * ch);  //trie une chaine de caractère en la coonvertissant en tableau, triant le tableau puis reconvertit en chaine de caractère

int main() //début du prgramme 
{
    chaine ch1, ch2;    //initialisation des chaine des caractères ch1 et ch2
    init(&ch1); //appel de la fcontion init qui est une chaine vide qui porte le nom ch1
    init(&ch2); //idem pour ch2

    ajQ(&ch1, 'C'); //appel de la fonction qui ajoute le caractère C à la fin de la chaine ch1
    ajQ(&ch1, 'H'); //idem mais pour H
    ajQ(&ch1, 'I'); //idem pour I
    ajQ(&ch1, 'E'); //idem pour E
    ajQ(&ch1, 'N'); //idem pour N
    aff(ch1); //appel de la fonction pour afficher ch1 - CHAINE

    ajQ(&ch2, 'N'); //appel de la fonction qui ajoute la caractère N à la fin de la chaine ch2
    ajQ(&ch2, 'I'); //idem pour I
    ajQ(&ch2, 'C'); //idem pour C
    ajQ(&ch2, 'H'); //idem pour H
    ajQ(&ch2, 'E'); //idem pour E
    aff(ch2);   //appel de la fonction qui affiche ch2
    ord(&ch2);  //appel de la fonction qui trie la chaine de caractère 
    aff(ch2);   //appel de la fonction qui affiche ch2 après tri

    return EXIT_SUCCESS;
}

void init(chaine * ch){
    *ch=NULL;
}

bool estV(chaine ch){
    return ch==NULL;
}

void ajT(chaine * ch, char c){
    element * nouveau = (element*)malloc(sizeof(element));
    nouveau->lettre = c;
    nouveau->svt = *ch;
    *ch = nouveau;
}

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

int nbC(chaine ch){
    int nb = 0;
    element * ptCourant = ch;
    while (ptCourant != NULL){
        ptCourant = ptCourant->svt;
        nb++;
    }
    return nb;
}

void aff(chaine ch){
    if (!estV(ch)){
        printf("%c", ch->lettre);
        aff(ch->svt);
    } else {
        printf("\n");
    }
}

//fonction qui prend en paramètre 2 chaines de caractères et les compare. Elle renvoie un booléen qui indique si les 2 chaines de caractères sont identiques.
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

void inv(chaine ch1, chaine * ch2){
    element * ptCourant;

    ptCourant = ch1;
    while (ptCourant != NULL){
        ajT(ch2, ptCourant->lettre);
        ptCourant = ptCourant->svt;
    }
}

bool pal(chaine ch1){
    chaine ch;

    init(&ch);
    inv(ch1, &ch);
    return ide(ch1, ch);
}

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
//fonction qui prend en paramètres 2 chaines de caractères et qui renvoie 
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

void TvL(typTab t, chaine * ch, int N){
    int i;

    for (i=0 ; i<N ; i++){
        ajQ(ch, t[i]);
    }
}

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

void ord(chaine * ch){
    typTab t;
    int N = LvT(*ch, t);
    if (N!=-1){ //initialisation de la boucle avec pour condition N! = -1
        trT(t, N);
        *ch = NULL;
        TvL(t, ch, N);
    }
}
