#include "ligneBus.h"
#include "lectureFichier.h"
#include "listeDouble.h"
#include "tri.h"
#include "types.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#define VITESSE 25 // vitesse de 25 km/h

t_date generateur_dateDerniereMaintenance(void)
{
    t_date date;
    date.mois = rand() % 12 + 1;
    if (date.mois == 2)
    {
        date.jour = rand() % 29 + 1;
    }
    else if (date.mois == 4 || date.mois == 6 || date.mois == 9 || date.mois == 11)
    {
        date.jour = rand() % 31 + 1;
    }
    else
        date.jour = rand() % 31 + 1;
    date.annee = rand() % 31 + 2000;
    return (date);
}

int generateur_int(void)
{
    int num = rand() % 91 + 10;
    return num;
}

Tbus creeBus(int idBus, TlisteStation start)
{
    Tbus myBus = (Tbus)malloc(sizeof(Typebus));
    myBus->idBus = idBus;
    // printf("start correspond à :\n");
    // afficheStation(getPtrData(start));
    busSurStation(myBus, start, depart_vers_arrivee);
    busSurLigneX(myBus, getIdLigneTroncon(getPtrData(getNextTroncon(start))));
    return myBus;
}

Tstation *creeArret(int posX, int posY, char *nomStation, int idStation)
{
    Tstation *newStation = (Tstation *)malloc(sizeof(Tstation));

    newStation->arret_ou_troncon = ARRET;

    // champs utiles si ARRET
    newStation->posX = posX;
    newStation->posY = posY;
    strcpy(newStation->nomStation, nomStation);
    newStation->idStation = idStation;

    // champs non utiles si ARRET
    newStation->depart = NULL;
    newStation->arrivee = NULL;
    newStation->coutTemps = 0;
    newStation->coutDistance = 0;
    newStation->idLigneBus = -1; // id inexistant
    int num = generateur_int();
    set_cout_maintenance(newStation, num);
    set_date_derniere_maintenance(newStation, generateur_dateDerniereMaintenance());
    return newStation;
}

Tstation *creeTroncon(int idLigneBus, Tstation *depart, Tstation *arrivee)
{
    Tstation *newStation = (Tstation *)malloc(sizeof(Tstation));
    int distance = distance_entre_2_stations(depart, arrivee);

    newStation->arret_ou_troncon = TRONCON;

    // champs non utiles si TRONCON
    newStation->posX = 0;
    newStation->posY = 0;
    strcpy(newStation->nomStation, "");
    newStation->idStation = -1; // id inexistant
    // champs utiles si TRONCON
    newStation->idLigneBus = idLigneBus;
    newStation->depart = depart;
    newStation->arrivee = arrivee;
    newStation->coutTemps = temps_trajet(VITESSE, distance);
    newStation->coutDistance = distance;
    int num = generateur_int();
    set_cout_maintenance(newStation, num);
    set_date_derniere_maintenance(newStation, generateur_dateDerniereMaintenance());
    return newStation;
}

void afficheConsoleLigneBus(TlisteStation l)
{
    afficheListe(l);
}

TlisteStation getNextStation(TlisteStation l)
{
    if (ligneBusVide(l))
        return NULL;
    else
    {
        if (getTypeNoeud(getPtrData(l)) == ARRET)
        {
            if (ligneBusVide(getNextCell(l)))
                return NULL; // nous sommes sur un terminus
            else if (!ligneBusVide(getNextCell(getNextCell(l))))
            {
                return getNextCell(getNextCell(
                    l)); // le prochain arret est dans 2 cellules dans la liste, la cellule suivaante �tant un TRONCON
            }
            else
            {
                // on est sur un terminus qui retourne au debut de la ligne de bus

                return getptrFirstCell(l); // on retourne donc le debut de la ligne de bus
            }
        }
        else
            return getNextCell(
                l); // si on est sur un TRONCON alors la prochaine cellule est la prochaine station (ARRET)
    }
}

TlisteStation getNextTroncon(TlisteStation l)
{
    if (ligneBusVide(l))
        return NULL;
    else
    {
        if (getTypeNoeud(getPtrData(l)) == ARRET)
        {
            return getNextCell(
                l); // la prochaine cellule est un TRONCON, peut �tre �gale � NULL si on est sur un terminus
        }
        else
        {
            printf("\n(getNextTroncon) erreur algo, on est deja sur un troncon");
            return l; // on retourne le troncon actuel
        }
    }
}

TlisteStation getPreviousStation(TlisteStation l)
{
    if (ligneBusVide(l))
        return NULL;
    else
    {
        if (getTypeNoeud(getPtrData(l)) == ARRET)
        {
            if (ligneBusVide(getPrevCell(l)))
            {
                // nous sommes sur un terminus
                if (getTypeNoeud(getPtrData(getptrLastCell(l))) == TRONCON)
                {
                    return getPrevCell(getptrLastCell(l)); // si le dernier noeud est un troncon,
                    // on a une ligne circulaire, on prend donc la station avent le troncon trouve, cad un arret
                }
                return NULL; // si on est la, cad que le dernier element de la liste n'est pas un troncon
                // donc la ligne de bus n'est pas circulaire
            }
            return getPrevCell(getPrevCell(
                l)); // le prochain arret est dans 2 cellules dans la liste, la cellule suivaante �tant un TRONCON
        }
        else
            return getPrevCell(
                l); // si on est sur un TRONCON alors la prochaine cellule est la prochaine station (ARRET)
    }
}

bool ligneBusVide(TlisteStation l)
{
    return listeVide(l);
}

int getPosXListeStation(TlisteStation myStationInListe)
{
    return getPosXStation(getPtrData(myStationInListe));
}

int getPosYListeStation(TlisteStation myStationInListe)
{
    return getPosYStation(getPtrData(myStationInListe));
}

void deplaceBus(Tbus myBus, TsensParcours sens_deplacement, int *incXSprite, int *incYSprite)
{
    TlisteStation dest;
    int xdep, ydep, xarr, yarr, pas;
    float ratio;
    pas = 2; // �quivalent au parametre d'erreur dans le trac� de Segment de Bresenham
    // https://fr.wikipedia.org/wiki/Algorithme_de_trac%C3%A9_de_segment_de_Bresenham

    if (sens_deplacement == depart_vers_arrivee)
        dest = getNextStation(getActualStation(myBus));
    else
        dest = getPreviousStation(getActualStation(myBus));

    // par d�faut: pas de d�placement du bus
    *incXSprite = 0;
    *incYSprite = 0;

    // si un prochain arret existe?
    if (ligneBusVide(dest))
    {
        /*printf("\nBus id%d est au terminus de la ligne %d sur la station id=%d %s",
               getIdBus(myBus),
               getIdLigneActuelleDuBus(myBus),
               getIdStation(getPtrData(getActualStation(myBus))),
               getNomStation(getPtrData(getActualStation(myBus))));*/
    }
    else // s'il existe:
    {
        // soit on est arriv�, soit on est en cours de trajet
        // r�cup�ration des coordonn�es
        xdep = getPosXBus(myBus);
        ydep = getPosYBus(myBus);
        xarr = getPosXListeStation(dest);
        yarr = getPosYListeStation(dest);

        // sommes-nous arriv�s?
        if ((abs(xdep - xarr) <= pas) && (abs(ydep - yarr) <= pas))
        {
            // alors mise � jour de la station actuelle du bus
            setActualStation(myBus, dest);
            printf("\nLe Bus id%d arrive sur la station id%d %s",
                   getIdBus(myBus),
                   getIdStation(getPtrData(getActualStation(myBus))),
                   getNomStation(getPtrData(getActualStation(myBus))));
        }
        else
        {
            // sinon on calcule les offsets de d�placements pour la boucle main qui r�alise les affichages
            ratio = (float)(abs(yarr - ydep)) / (float)(abs(xarr - xdep)); // div euclydienne

            // algo de Bresenham "simplifi� de fa�on ad hoc" et non optimis� (� cause des floats)
            if (xarr > xdep)
                *incXSprite = pas;
            else if (xarr < xdep)
                *incXSprite = -pas;

            if (yarr > ydep)
                *incYSprite = (int)((float)(pas)*ratio); // pour un d�placement proportionnel en Y selon Y/X
            else if (yarr < ydep)
                *incYSprite = (int)((float)(-pas) * ratio);

            // remmarque: si xarr==ydep alors *incXSprite reste � 0, idem en Y

            // maj des coord du bus
            setPosXBus(myBus, getPosXBus(myBus) + *incXSprite);
            setPosYBus(myBus, getPosYBus(myBus) + *incYSprite);
        }
    }
}

// Setteurs
void setActualStation(Tbus myBus, TlisteStation arrivalStation);
void setPosXBus(Tbus myBus, int newX);
void setPosYBus(Tbus myBus, int newY);
void setIdLigneBus(Tbus myBus, int idLigne);
void setSensParcours(Tbus myBus, TsensParcours sens);

void setPositionSurLaLigneDeBus(Tbus myBus, TlisteStation myStation);

void busSurStation(Tbus myBus, TlisteStation myStation, TsensParcours sens)
{
    // Exemple encapsulation: on a pr�f�r� les fonctions setteurs � un acc�s direct aux champs

    setPositionSurLaLigneDeBus(myBus, myStation);
    // myBus->positionSurLaLigneDeBus = myStation;  //interdit

    setSensParcours(myBus, sens);
    // myBus->sensParcours = sens;

    setPosXBus(myBus, getPosXListeStation(myStation));
    // myBus->posXBus = getPosXListeStation( myStation );

    setPosYBus(myBus, getPosYListeStation(myStation));
    // myBus->posYBus = getPosYListeStation( myStation );

    // idem : on passe ci-dessous par les getteurs et non par un acc�s direct via les champs
    printf("\nBus id%d est en (x = %d, y = %d) sur la ligne %d, station %s",
           getIdBus(myBus),
           getPosXBus(myBus),
           getPosYBus(myBus),
           getIdLigneActuelleDuBus(myBus),
           getNomStation(getPtrData(myStation)));
}

void busSurLigneX(Tbus myBus, int idLigneX)
{
    setIdLigneBus(myBus, idLigneX);
}

TlisteStation stationSuivantePourLeBus(Tbus myBus)
{
    TlisteStation prochaineStation;
    if (getSensParcours(myBus) == depart_vers_arrivee)
        prochaineStation = getNextStation(getActualStation(myBus));
    else
        prochaineStation = getPreviousStation(getActualStation(myBus));
    return prochaineStation;
}

void afficheCoordonneesBus(Tbus myBus)
{
    char *nomProchaineStation;
    TlisteStation prochaineStation = stationSuivantePourLeBus(myBus);

    if (ligneBusVide(prochaineStation))
        printf("\nBus id: %d est en (x = %d, y = %d) ligne %d, derniere station %s (Terminus)",
               getIdBus(myBus),
               getPosXBus(myBus),
               getPosYBus(myBus),
               getIdLigneActuelleDuBus(myBus),
               getNomStation(getPtrData(getActualStation(myBus))));
    else
    {
        nomProchaineStation = getNomStation(getPtrData(prochaineStation));
        printf("\nBus id: %d est en (x = %d, y = %d) ligne %d, derniere station %s, prochaine station %s",
               getIdBus(myBus),
               getPosXBus(myBus),
               getPosYBus(myBus),
               getIdLigneActuelleDuBus(myBus),
               getNomStation(getPtrData(getActualStation(myBus))),
               nomProchaineStation);
    }
}

// Creer ci-dessous vos fonctions
void tri_par_cout_maintenance(TlisteStation l)
{
    tri_selection_base(l, plus_petit_cout_maintenance);
}

void tri_par_date_derniere_maintenance(TlisteStation l)
{
    tri_selection_base(l, date_derniere_maintenance_recent);
}

int temps_trajet(int vitesse, int distance)
{
    return distance / vitesse;
}

int distance_entre_2_stations(Tstation *position1,
                              Tstation *position2) // calcul de distance entre 2 stations (position 1 et 2).
{
    return round(sqrt(pow(getPosXStation(position2) - getPosXStation(position1), 2) + pow(getPosYStation(position2) - getPosYStation(position1), 2)));
}

void fusion_ligne_de_bus(T_tab_ligne *ligne, int l1, int l2) // penser à remplacer par get et set
{
    T_liste nouvelle;
    initListe(&nouvelle);
    T_liste tmp = get_ligne_bus_from_tab_ligne(ligne, l1);
    Tstation *derniereSationRajoutee;
    derniereSationRajoutee = malloc(sizeof(Tstation));
    int nombre_station = getNbreCell(get_ligne_bus_from_tab_ligne(ligne, l1));
    if (nombre_station == 1)
    {
        nouvelle = ajoutEnFin(nouvelle, getPtrData(tmp));
    }
    else if (nombre_station > 1)
    {
        while (getptrNextCell(tmp) != NULL)
        {
            Tstation *station_rajout;
            station_rajout = calloc(sizeof(Tstation), 1);
            *station_rajout = *getPtrData(tmp);
            if (getTypeNoeud(tmp->pdata) == TRONCON)
            {
                tmp = getptrNextCell(tmp);

                nouvelle = ajoutEnFin(nouvelle, creeTroncon(get_num_ligne(ligne) + 1, derniereSationRajoutee, station_rajout));
                nouvelle = ajoutEnFin(nouvelle, getPtrData(tmp));
            }
            else
            {
                nouvelle = ajoutEnFin(nouvelle, getPtrData(tmp));
            }
            derniereSationRajoutee = tmp->pdata;
            if (getptrNextCell(tmp) != NULL)
            {
                tmp = getptrNextCell(tmp);
            }
        }
    }
    // printf("rajout premiere ligne dans nouvelle ligne bon\n");
    nouvelle = ajoutEnFin(nouvelle, creeTroncon(get_num_ligne(ligne) + 1, tmp->pdata, (get_ligne_bus_from_tab_ligne(ligne, l1))->pdata));

    tmp = get_ligne_bus_from_tab_ligne(ligne, l2);
    while (getptrNextCell(tmp) != NULL)
    {
        Tstation *station_rajout;
        station_rajout = calloc(sizeof(Tstation), 1);
        *station_rajout = *getPtrData(tmp);
        if (getTypeNoeud(tmp->pdata) == TRONCON)
        {
            tmp = getptrNextCell(tmp);

            nouvelle = ajoutEnFin(nouvelle, creeTroncon(get_num_ligne(ligne) + 1, derniereSationRajoutee, station_rajout));
            nouvelle = ajoutEnFin(nouvelle, getPtrData(tmp));
        }
        else
        {
            nouvelle = ajoutEnFin(nouvelle, getPtrData(tmp));
        }
        derniereSationRajoutee = tmp->pdata;
        if (getptrNextCell(tmp) != NULL)
        {
            tmp = getptrNextCell(tmp);
        }
    }
    // printf("rajout deuxieme ligne dans nouvelle ligne bon\n");
    ajout_ligne_tab_ligne(ligne, nouvelle);
}

int supr_station_de_ligne(T_tab_ligne *tab_ligne, int id_ligne, int index_supr)
{
    if (index_supr > 0)
    {
        TlisteStation temp;
        temp = get_ligne_bus_from_tab_ligne(tab_ligne, id_ligne);
        // printf("ligne de bus avent modification : \n");
        // afficheListe(temp);
        for (int i = 0; i < index_supr; i++)
        {
            if (!listeVide(temp))
            {
                if (!listeVide(getptrNextCell(temp)))
                {
                    temp = getptrNextCell(getptrNextCell(temp));
                }
                else
                {
                    return -1;
                }
            }
            else
            {
                return -1;
            }
        }
        if (getNextCell(temp) == NULL)
        {
            temp = getPreviousStation(temp);
            free(getNextStation(temp));
            temp = free_last(temp);
            set_ligne_in_tab_ligne(tab_ligne, id_ligne, getptrFirstCell(temp));
            return 0;
        }
        else
        {
            TlisteStation apres = getNextStation(temp);
            TlisteStation avant = getPreviousStation(temp);
            /*
            printf("\n\n prev station is in %p\n", avant);
            printf("next station is in %p\n", apres);
            printf("current station is in %p\n", temp);*/
            *if (!listeVide(getptrPrevCell(temp)))
            {
                temp = getptrPrevCell(temp);
            }

            temp = free_head(temp);

            temp = getptrNextCell(temp);

            free(getptrPrevCell(temp));

            temp = free_head(temp);

            // printf("temp is now at pointer %p\n", temp);
            setPtrNextCell(avant, apres);
            // printf("avant->next = %p\n", getptrNextCell(avant));
            setPtrPrevCell(apres, avant);
            // printf("apres->prev = %p\n", getptrLastCell(apres));
            Tstation *troncon;
            troncon = creeTroncon(id_ligne + 1, getPtrData(avant), getPtrData(apres));
            set_ligne_in_tab_ligne(tab_ligne, id_ligne, (add_n(get_ligne_bus_from_tab_ligne(tab_ligne, id_ligne), index_supr * 2 - 1, troncon)));

            /*
            printf("liste finale = \n");
            afficheListe(get_ligne_bus_from_tab_ligne(tab_ligne, id_ligne));

                        printf("verification de toute les lignes de bus \n");
                        for (int i = 0; i < get_num_ligne(tab_ligne); i++)
                        {
                            afficheListe(get_ligne_bus_from_tab_ligne(tab_ligne, i));
                        }
                        sleep(600);
                        */
            return 0;
        }
    }
    else if (index_supr == 0)
    {
        // si on veut enlever la premiere station
        TlisteStation temp = get_ligne_bus_from_tab_ligne(tab_ligne, id_ligne);
        // printf("\n\n\nresultat previousStation : %p\n", getPreviousStation(temp));
        if (listeVide(getPreviousStation(temp)))
        {
            // si la ligne de bus n'est pas circulaire
            temp = getptrNextCell(temp);
            if (temp != NULL)
            {
                free(getptrPrevCell(temp));
                temp = free_head(temp);
                setPtrPrevCell(temp, NULL);
                set_ligne_in_tab_ligne(tab_ligne, id_ligne, temp);
                return 0;
            }
            else
            {
                printf("impossible il faut absolument une station dans une ligne de bus\n");
                return -1;
            }
        }
        else
        {
            // printf("LA LIGNE %d EST CIRCULAIRE\n", id_ligne + 1);
            //  si la ligne de bus est circulaire
            //  on elimine le dernier troncon la premiere station et le premier troncon
            //  puis circulariser la ligne

            if (getNextStation(temp) == temp)
            {
                // pour une ligne circulaire la prochaine station sera la premiere station SI il n'y a qu'une station
                printf("impossible il faut absolument une station dans une ligne de bus\n");
                return -1;
            }

            temp = get_ligne_bus_from_tab_ligne(tab_ligne, id_ligne);
            temp = free_last(temp); // elimination du dernier troncon

            temp = getptrNextCell(temp);
            free(getptrPrevCell(temp)); // elimination de la premiere station

            temp = free_head(temp);
            setPtrPrevCell(temp, NULL); // elimination du premier troncon
            // et rebranchement du deuxieme station vers NULL

            set_ligne_in_tab_ligne(tab_ligne, id_ligne, temp); // on remplace la ligne en memoire

            circuralise_ligne_de_bus(tab_ligne, id_ligne); // on rend tout circulaire

            // afficheListe(get_ligne_bus_from_tab_ligne(tab_ligne, id_ligne));
            return 0;
        }
    }
    else
    {
        return -1;
    }
}

int circuralise_ligne_de_bus(T_tab_ligne *tab_ligne, int id_ligne)
{
    if (tab_ligne == NULL || get_ligne_bus_from_tab_ligne(tab_ligne, id_ligne) == NULL)
        return -1;
    TlisteStation ligne = (get_ligne_bus_from_tab_ligne(tab_ligne, id_ligne));
    Tstation *start = getPtrData(ligne);
    Tstation *end = getPtrData(getptrLastCell(ligne));
    Tstation *troncon = creeTroncon(id_ligne + 1, start, end);
    ligne = ajoutEnFin(ligne, troncon);
    return 0;
}
