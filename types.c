#include <stdio.h>
#include <stdlib.h>

#include "types.h"

void afficheStation(Tstation *station)
{
    if (station == NULL)
    {
        printf("\nerreur station non allouée!\n");
    }
    else
    {

        if (station->arret_ou_troncon == ARRET)
        {
            printf("id = %d\ncout maintenance = %d\ndate maintenance = %d-%d-%d \n x = %d \n y = %d\n\n", station->idStation, station->coutMaintenance, station->dateDerniereMaintenance.jour, station->dateDerniereMaintenance.mois, station->dateDerniereMaintenance.annee, station->posX, station->posY);

            // printf("\nSTATION idStation: %d, Station : %s, posX = %d, posY = %d, Cout maintenance = %d, derniere maintenance fait le : %d-%d-%d", station->idStation, station->nomStation, station->posX, station->posY,station->coutMaintenance,station->dateDerniereMaintenance.jour,station->dateDerniereMaintenance.mois,station->dateDerniereMaintenance.annee);
        }
        else
        {
            printf("id ligne = %d\ncout maintenance = %d\ndate maintenance = %d-%d-%d\n id_dep = %d\nid_fin = %d\n\n\n\n", station->idLigneBus, station->coutMaintenance, station->dateDerniereMaintenance.jour, station->dateDerniereMaintenance.mois, station->dateDerniereMaintenance.annee, station->depart->idStation, station->arrivee->idStation);

            // alors le noeud est un tron�on
            /*printf("\n\nTRONCON idLigneBus: %d, temps: %d sec, distance à parcourir: %d mètres", station->idLigneBus, station->coutTemps, station->coutDistance);
            afficheStation(station->depart);
            afficheStation(station->arrivee);
            printf("\nfin tronçon\n");*/
        }
    }
}

// Getteurs

int getIdStation(Tstation *myStation)
{
    return myStation->idStation;
}

char *getNomStation(Tstation *myStation)
{
    return myStation->nomStation;
}

int getPosXStation(Tstation *myStation)
{
    return myStation->posX;
}
int getPosYStation(Tstation *myStation)
{
    return myStation->posY;
}

int getIdLigneTroncon(Tstation *myStation)
{
    // une station n'est pas li�e � une ligne, seulement le troncon
    if (getTypeNoeud(myStation) == TRONCON)
        return myStation->idLigneBus;
    else
    {
        printf("\n(getIdLigneTroncon) Erreur algo, vous n etes pas sur un troncon");
        return -1;
    }
}

TypeNoeud getTypeNoeud(Tstation *myStation)
{
    return myStation->arret_ou_troncon;
}

int getPosXBus(Tbus myBus)
{
    return myBus->posXBus;
}
int getPosYBus(Tbus myBus)
{
    return myBus->posYBus;
}
int getIdBus(Tbus myBus)
{
    return myBus->idBus;
}
int getIdLigneActuelleDuBus(Tbus myBus)
{
    return myBus->idLigneBusActuelle;
}

int getIdLigneBus(Tbus myBus)
{
    return myBus->idLigneBusActuelle;
}

TsensParcours getSensParcours(Tbus myBus)
{
    return myBus->sensParcours;
}

inline t_date get_date_derniere_maintenance(Tstation *myStation)
{
    return myStation->dateDerniereMaintenance;
}

inline int get_cout_maintenance(Tstation *myStation)
{
    return myStation->coutMaintenance;
}

// Setteurs

void setActualStation(Tbus myBus, TlisteStation arrivalStation)
{
    myBus->positionSurLaLigneDeBus = arrivalStation;
}

TlisteStation getActualStation(Tbus myBus)
{
    return myBus->positionSurLaLigneDeBus;
}

void setPosXBus(Tbus myBus, int newX)
{
    myBus->posXBus = newX;
}

void setPosYBus(Tbus myBus, int newY)
{
    myBus->posYBus = newY;
}

void setIdLigneBus(Tbus myBus, int idLigne)
{
    myBus->idLigneBusActuelle = idLigne;
}

void setSensParcours(Tbus myBus, TsensParcours sens)
{
    myBus->sensParcours = sens;
}

void setPositionSurLaLigneDeBus(Tbus myBus, TlisteStation myStation)
{
    myBus->positionSurLaLigneDeBus = myStation;
}

inline void set_date_derniere_maintenance(Tstation *myStation, t_date date)
{
    myStation->dateDerniereMaintenance = date;
}

inline void set_cout_maintenance(Tstation *myStation, int cout)
{
    myStation->coutMaintenance = cout;
}
