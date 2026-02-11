#ifndef LECTUREFICHIER_H_INCLUDED
#define LECTUREFICHIER_H_INCLUDED

#include "types.h"

typedef struct t_tab_station
{
    int       numStation;
    int       taille_tab;
    Tstation* tabStation;
} T_tab_station;

typedef struct t_tab_ligne
{
    int            numLigne;
    int            taille_tab;
    TlisteStation* tabLigne;
} T_tab_ligne;

typedef struct t_data_tab
{
    T_tab_station* stations;
    T_tab_ligne*   lignes;
} T_data_tab;

void print_tab_station(Tstation* stations, int taille);

T_tab_station* load_station(char* file_name);
T_tab_ligne*   load_ligne_bus(char* file_name, Tstation* tabStation);
T_data_tab*    lecture_sauvegarde(char* ligne, char* station);

int            get_num_data_from_file(char* file_name);
int            get_num_station(T_tab_station* station);
int            get_num_ligne(T_tab_ligne* ligne);
int            get_size_tab_station(T_tab_station* station);
int            get_size_tab_ligne(T_tab_ligne* ligne);
T_tab_ligne*   get_tab_ligne_from_data(T_data_tab* data);
T_tab_station* get_tab_station_from_data(T_data_tab* data);
TlisteStation  get_ligne_bus_from_tab_ligne(T_tab_ligne* lignes, int index_ligne);
Tstation       get_station_from_tab_station(T_tab_station* stations, int id_station);
void           set_ligne_in_tab_ligne(T_tab_ligne* lignes, int id_ligne, TlisteStation new_line);

void ajout_taille_tab_station(T_tab_station* station_data);
void ajout_taille_tab_ligne(T_tab_ligne* ligne_data);
void ajout_ligne_tab_ligne(T_tab_ligne* ligne_data, TlisteStation ligne);
void ajout_station_tab_station(T_tab_station* station_data, Tstation* station);

void free_tab_ligne(T_tab_ligne* ligne_data);
void free_tab_station(T_tab_station* station_data);
void free_data(T_data_tab* data);

#endif // LIGNEBUS_H_INCLUDED
