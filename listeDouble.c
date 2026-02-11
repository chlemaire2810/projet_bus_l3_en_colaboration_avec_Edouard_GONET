#include <stdio.h>
#include <stdlib.h>

#include "listeDouble.h"

// initListe ne fait pas de malloc, juste une initialisation � NULL du pointeur de liste
void initListe(T_liste *l)
{
    *l = NULL;
}

bool listeVide(T_liste l)
{
    return (l == NULL);
}

void afficheListe(T_liste l)
{
    T_liste courant = l;

    while (courant != NULL)
    {
        afficheStation(courant->pdata); // fourni par types.h
        // printf("next = %p,prev = %p,cur=%p\n\n", getptrNextCell(courant), getptrLastCell(courant), courant);
        courant = courant->suiv;
    }
}

T_liste ajoutEnTete(T_liste l, Tstation *mydata)
{
    T_liste nouv = (T_liste)malloc(sizeof(struct T_cell));

    // CHANGEMENT par rapport � la partie2 d'updago
    // nouv->pdata = (int*)malloc(sizeof(int)); ATTENTION PLUS DE RECOPIE de mydata, donc plus d'allocation m�moire sur ce champ

    // SIMPLE BRANCHEMENT EN MEMOIRE ENTRE POINTEURS (SUR LA STATION EXISTANTE)
    nouv->pdata = mydata;

    if (l == NULL) // on cree en fait la premiere cellule de la liste
    {
        nouv->suiv =
            NULL; // s�curise la pr�sence de NULL sur le champ suiv, au cas o� le ptr l n'aurait pas �t�  initialis� correctement via initListe
        nouv->prec = NULL;
    }
    else // la lste n'etait pas vide, on doit donc faire les branchements
    {
        nouv->suiv = l;
        nouv->prec = NULL;
        l->prec = nouv;
    }
    return nouv;
}

T_liste ajoutEnFin(T_liste l, Tstation *mydata)
{
    T_liste nouv, courant = l; // remarque: pas de malloc si on appelle ajoutEnTete

    if (l == NULL) // on cree en fait la premiere cellule de la liste
    {
        return ajoutEnTete(l, mydata);
    }
    else // la liste n'etant pas vide,d�placment sur la derniere cellule, malloc et branchements
    {
        while (courant->suiv != NULL)
        {
            courant = courant->suiv;
        }
        nouv = (T_liste)malloc(sizeof(struct T_cell));
        nouv->pdata = mydata;

        nouv->suiv = NULL;
        nouv->prec = courant;
        courant->suiv = nouv;
    }
    return l; // la t�te d'origine, qui n'a pas chang�
}

Tstation *getPtrData(T_liste l)
{
    if (l == NULL)
    {
        printf("\nERREUR ptr pdata non allou�");
        return NULL;
    }
    else
    {
        return l->pdata;
    }
}

T_liste getNextCell(T_liste l)
{
    if (l == NULL)
    {
        return NULL; // convention
    }
    else
    {
        return l->suiv;
    }
}

T_liste getPrevCell(T_liste l)
{
    if (l == NULL)
    {
        return NULL; // convention
    }
    else
    {
        return l->prec;
    }
}

T_liste add_n(T_liste l, int pos, Tstation *val)
{
    if (pos == 0)
    {
        return (ajoutEnTete(l, val));
    }
    else if (pos > 0)
    {
        T_liste p = l;
        int i = 0;
        while (pos > i++ && !listeVide(p))
        {
            p = p->suiv;
        }
        if (listeVide(p))
        {
            printf("position out of bounds\n");
            return l;
        }
        else if (listeVide(p->suiv))
        {
            return (ajoutEnFin(l, val));
        }
        else
        {
            T_liste c = (T_liste)malloc(sizeof(T_cellule));
            c->prec = p->prec;
            p->prec->suiv = c;
            p->prec = c;
            c->suiv = p;
            c->pdata = val;
            return l;
        }
    }
    else
    {
        printf("position out of bounds\n");
        return l;
    }
}

T_liste free_head(T_liste l)
{
    if (!listeVide(l))
    {
        afficheStation(l->pdata);
        free(l->pdata);
        T_liste r = l->suiv;
        free(l);
        return r;
    }
    else
    {
        free(l);
        return NULL;
    }
}
T_liste free_last(T_liste l)
{
    T_liste p = l;
    while (!listeVide(p->suiv))
    {
        p = p->suiv;
    }
    p->prec->suiv = NULL;
    free_head(p);
    return l;
}

T_liste free_nth(T_liste l, int pos)
{
    if (pos == 0)
    {
        l = free_head(l);
    }
    else if (pos < 0)
    {
        printf("index out of bounds");
    }
    else
    {
        T_liste p = l;
        int i = 0;
        while (pos > i++ && !listeVide(p))
        {
            p = p->suiv;
        }
        if (listeVide(p))
        {
            printf("index out of bounds\n");
        }
        else
        {
            if (!listeVide(p->suiv))
                p->suiv->prec = p->prec;
            if (!listeVide(p->prec))
                p->prec->suiv = p->suiv;
            free(p->pdata);
            free(p);
        }
    }

    return l;
}

T_liste getptrFirstCell(T_liste l)
{
    T_liste p = l;
    while (!listeVide(p->prec))
    {
        p = p->prec;
    }
    return p;
}

T_liste getptrLastCell(T_liste l)
{
    T_liste p = l;
    while (!listeVide(p->suiv))
    {
        p = p->suiv;
    }
    return p;
}
inline T_liste getptrNextCell(T_liste l)
{
    return l->suiv;
}
inline void setPtrNextCell(T_liste l, T_liste next)
{
    l->suiv = next;
}
inline T_liste getptrPrevCell(T_liste l)
{
    return l->prec;
}
inline void setPtrPrevCell(T_liste l, T_liste prev)
{
    l->prec = prev;
}

inline void swapPtrData(T_liste source, T_liste destination)
{
    Tstation *dat = getPtrData(source);
    source->pdata = destination->pdata;
    destination->pdata = dat;
}

int getNbreCell(T_liste l)
{
    T_liste p = l;
    int i = 0;
    while (!listeVide(p))
    {
        p = p->suiv;
        i++;
    }
    return i;
}

inline int getSizeBytes(T_liste l)
{
    return (getNbreCell(l) * sizeof(T_cellule));
}

// modification !!
T_liste creatNewListeFromFusion(T_liste l1, T_liste l2)
{
    T_liste nouvelle;
    initListe(&nouvelle);
    T_liste tmp = l1;

    while (tmp != NULL)
    {
        nouvelle = ajoutEnFin(nouvelle, (tmp->pdata));
        tmp = getptrNextCell(tmp);
    }

    tmp = l2;
    while (tmp != NULL)
    {
        nouvelle = ajoutEnFin(nouvelle, (tmp->pdata));
        tmp = getptrNextCell(tmp);
    }

    return nouvelle;
}

inline void addBehind(T_liste debut, T_liste suite)
{
    T_liste p = getptrLastCell(debut);
    p->suiv = suite;
    suite->prec = p;
}

T_liste findCell(T_liste l, int data)
{
    T_liste p = l->suiv;
    int f = (*(l->pdata)).idLigneBus;
    while (!(listeVide(p)) && f != data)
    {
        f = (*(l->pdata)).idLigneBus;
        p = p->suiv;
    }
    return p->prec;
}

int getOccurences(T_liste l, int data)
{
    T_liste p = l;
    int i = 0;
    while (!(listeVide(p)))
    {
        if ((*(l->pdata)).idLigneBus == data)
        {
            i++;
        }
        p = p->suiv;
    }
    return i;
}

void print_list_V2(T_liste l)
{
    T_liste courant = getptrFirstCell(l);
    while (!listeVide(courant))
    {
        afficheStation(courant->pdata);
        courant = getptrNextCell(courant);
    }
}

T_liste duplicate(T_liste l)
{
    T_liste temp = getptrLastCell(l);
    T_liste res;
    initListe(&res);
    while (!listeVide(temp))
    {
        res = ajoutEnTete(res, getPtrData(temp));
        temp = getptrPrevCell(temp);
    }
    return res;
}
