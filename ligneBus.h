#ifndef LIGNEBUS_H_INCLUDED
#define LIGNEBUS_H_INCLUDED

#include "lectureFichier.h"
#include "tri.h"
#include "types.h"
#include <stdbool.h>

t_date generateur_dateDerniereMaintenance(void);
int    generateur_int(void);

Tbus      creeBus(int idBus, TlisteStation start);
Tstation* creeArret(int posX, int posY, char* nomStation, int idStation);
Tstation* creeTroncon(int idLigneBus, Tstation* depart, Tstation* arrivee);

void afficheConsoleLigneBus(TlisteStation l);

TlisteStation getNextStation(TlisteStation l);
TlisteStation getNextTroncon(TlisteStation l);
TlisteStation getPreviousStation(TlisteStation l);
bool          ligneBusVide(TlisteStation l);

int getPosXListeStation(TlisteStation myStationInListe);
int getPosYListeStation(TlisteStation myStationInListe);

//renseigne incX et incY pour le d�placement du sprite dans le main, ET modifie la position du bus qd il atteint les coord X Y de la station d'arriv�e
void deplaceBus(Tbus myBus, TsensParcours sens_deplacement, int* incX, int* incY);

void          busSurStation(Tbus myBus, TlisteStation myStation, TsensParcours sens);
void          busSurLigneX(Tbus myBus, int idLigneX);
TlisteStation stationSuivantePourLeBus(Tbus myBus);

void afficheCoordonneesBus(Tbus myBus);

// ****   Lister ci-dessous vos fonctions  ****
void tri_par_cout_maintenance(TlisteStation l);
void tri_par_date_derniere_maintenance(TlisteStation l);
int  temps_trajet(int vitesse, int distance);
int  distance_entre_2_stations(Tstation* position1, Tstation* position2);

void fusion_ligne_de_bus(T_tab_ligne* ligne, int l1, int l2);
int  supr_station_de_ligne(T_tab_ligne* tab_ligne, int id_ligne, int index_supr);
int  circuralise_ligne_de_bus(T_tab_ligne* tab_ligne, int id_ligne);
#endif // LIGNEBUS_H_INCLUDED
