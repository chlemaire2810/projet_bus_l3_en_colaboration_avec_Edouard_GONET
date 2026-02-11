#include "lectureFichier.h"
#include "ligneBus.h"
#include "listeDouble.h"
#include "types.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

T_tab_station *load_station(char *file_name)
{
    FILE *f = NULL;
    int posX, posY;
    f = fopen(file_name, "r");
    if (f != NULL)
    {
        char buffer[100]; // taille ligne
        fgets(buffer, 100, f);
        if (buffer[0] == 'S')
        {
            fgets(buffer, 100, f);
            int taille = atoi(buffer);
            T_tab_station *res;
            res = calloc(sizeof(T_tab_station), 1);
            res->tabStation = calloc(sizeof(Tstation), taille);
            printf("allocated station array\n");
            res->numStation = 0;
            res->taille_tab = taille;
            for (int i = 0; i < taille; i++)
            {
                fscanf(f, "%s; ", buffer);
                posX = atoi(buffer);
                fscanf(f, "%s; ", buffer);
                posY = atoi(buffer);
                fgets(buffer, 100, f);
                Tstation *arret;
                arret = creeArret(posX, posY, buffer, i + 1);
                ajout_station_tab_station(res, arret);
            }
            fclose(f);
            printf("verrifying result :\n");
            print_tab_station(res->tabStation, res->numStation);

            return res;
        }
        else
        {
            return NULL;
        }
    }
    else
    {
        return NULL;
    }
}

T_tab_ligne *load_ligne_bus(char *file_name, Tstation *tabStation)
{
    FILE *f = NULL;
    f = fopen(file_name, "r");
    if (f != NULL)
    {
        char buffer[100]; // taille ligne
        fgets(buffer, 100, f);
        if (buffer[0] == 'L')
        {
            fgets(buffer, 100, f);
            int taille = atoi(buffer);
            T_tab_ligne *res;
            res = calloc(sizeof(T_tab_ligne), 1);
            res->numLigne = 0;
            res->tabLigne = calloc(sizeof(TlisteStation), taille);
            res->taille_tab = taille;
            printf("allocated bus line array\n");
            for (int i = 0; i < taille; i++)
            {
                printf("LIGNE %d : \n", i + 1);

                fgets(buffer, 100, f);
                printf("buffer = %s\n", buffer);

                char *ptr = strtok(buffer, "; ");
                TlisteStation newligne;
                initListe(&newligne);
                int arr = atoi(ptr);
                int dep = -1;
                Tstation *troncon = NULL;
                newligne = ajoutEnFin(newligne, tabStation + arr - 1);
                afficheStation(&tabStation[arr - 1]);
                ptr = strtok(NULL, "; ");
                while (ptr != NULL)
                {

                    dep = arr;
                    arr = atoi(ptr);
                    printf("arr = %d,dep = %d", arr, dep);
                    troncon = creeTroncon(i + 1, tabStation + dep - 1, tabStation + arr - 1);
                    afficheStation(troncon);

                    afficheStation(tabStation + arr - 1);
                    newligne = ajoutEnFin(newligne, troncon);
                    newligne = ajoutEnFin(newligne, tabStation + arr - 1);
                    ptr = strtok(NULL, "; ");
                }
                ajout_ligne_tab_ligne(res, newligne);
                printf("\n\n\n");
            }
            fclose(f);
            return res;
        }
        else
        {
            return NULL;
        }
    }
    else
    {
        return NULL;
    }
}

void print_tab_station(Tstation *stations, int taille)
{
    printf("on a %d stations \n", taille);
    for (int i = 0; i < taille; i++)
    {
        afficheStation(&stations[i]);
    }
}

T_data_tab *lecture_sauvegarde(char *ligne, char *station)
{
    T_tab_ligne *tabLigne;
    T_tab_station *stations = load_station(station);
    if (stations != NULL)
    {
        printf("load station fini,tab station ce trouve dans %p\n", (void *)&stations);
        printf("affichage stations retourne : \n");
        print_tab_station(stations->tabStation, stations->numStation);
        tabLigne = load_ligne_bus(ligne, stations->tabStation);
        if (tabLigne != NULL)
        {
            printf("load ligne fini\n");
            T_data_tab *res;
            res = calloc(1, sizeof(T_data_tab));
            res->lignes = tabLigne;
            res->stations = stations;
            printf("load sauvegarde fini\n");
            return res;
        }
        else
        {
            fprintf(stderr, "erreur, il n'y a pas de fichiers de sauvegarde pour les lignes ou de dossier sauvegarde");
            exit(1);
        }
    }
    else
    {
        fprintf(stderr, "erreur, il n'y a pas de fichiers de sauvegarde pour les stations ou de dossier sauvegarde");
        exit(1);
    }
}

int get_num_data_from_file(char *file_name)
{
    FILE *file = fopen(file_name, "r");
    char line[256];
    int i = 0;
    while (fgets(line, sizeof(line), file)) // tant qu'on arrive � lire une ligne
    {
        i++;
        if (i == 2) // si la ligne est correct on ferme le fichier et renvoie le num�ro trouv�
        {
            fclose(file);
            return atoi(line);
        }
    }
    fclose(
        file); // si on est l�, ca veut dire qu'on a un fichier de longuer endessous de 2 et donc qu'on a pas de num�ro de donn�es
    return -1;
}

int get_num_station(T_tab_station *station)
{
    return station->numStation;
}

int get_num_ligne(T_tab_ligne *ligne)
{
    return ligne->numLigne;
}

int get_size_tab_station(T_tab_station *station)
{
    return station->taille_tab;
}

int get_size_tab_ligne(T_tab_ligne *ligne)
{
    return ligne->taille_tab;
}

T_tab_ligne *get_tab_ligne_from_data(T_data_tab *data)
{
    return data->lignes;
}

T_tab_station *get_tab_station_from_data(T_data_tab *data)
{
    return data->stations;
}

TlisteStation get_ligne_bus_from_tab_ligne(T_tab_ligne *lignes, int index_ligne)
{
    return lignes->tabLigne[index_ligne];
}

Tstation get_station_from_tab_station(T_tab_station *stations, int id_station)
{
    return stations->tabStation[id_station];
}

void set_ligne_in_tab_ligne(T_tab_ligne *lignes, int id_ligne, TlisteStation new_line)
{
    lignes->tabLigne[id_ligne] = new_line;
}

void ajout_taille_tab_station(T_tab_station *station_data)
{
    Tstation *temp_tab = station_data->tabStation;
    Tstation *new_tab = calloc(sizeof(Tstation), station_data->taille_tab * 2);
    for (int i = 0; i < station_data->numStation; i++)
    {
        new_tab[i] = temp_tab[i];
    }
    station_data->tabStation = new_tab;
    free(temp_tab);
    station_data->taille_tab *= 2;
}

// � ameliorer avec un memcopy peut-etre
void ajout_taille_tab_ligne(T_tab_ligne *ligne_data)
{
    TlisteStation *temp_tab = ligne_data->tabLigne;
    TlisteStation *new_tab = calloc(sizeof(TlisteStation), ligne_data->taille_tab * 2);
    for (int i = 0; i < ligne_data->numLigne; i++)
    {
        new_tab[i] = temp_tab[i];
    }
    ligne_data->tabLigne = new_tab;
    free(temp_tab);
    ligne_data->taille_tab *= 2;
}

void ajout_ligne_tab_ligne(T_tab_ligne *ligne_data, TlisteStation ligne)
{
    ligne_data->tabLigne[ligne_data->numLigne++] = ligne;
    if (ligne_data->numLigne == ligne_data->taille_tab)
    {
        ajout_taille_tab_ligne(ligne_data);
    }
}
void ajout_station_tab_station(T_tab_station *station_data, Tstation *station)
{
    station_data->tabStation[station_data->numStation++] = *station;
    if (station_data->numStation == station_data->taille_tab)
    {
        ajout_taille_tab_station(station_data);
    }
}

void free_tab_ligne(T_tab_ligne *ligne_data)
{
    for (int i = 0; i < get_num_ligne(ligne_data); i++)
    {
        TlisteStation temp = get_ligne_bus_from_tab_ligne(ligne_data, i);
        while (!listeVide(temp))
        {
            TlisteStation next = getptrNextCell(temp);
            free(temp);
            temp = next;
        }
    }
    free(ligne_data->tabLigne);
    free(ligne_data);
}

void free_tab_station(T_tab_station *station_data)
{
    free(station_data->tabStation);
    free(station_data);
}

void free_data(T_data_tab *data)
{
    free_tab_ligne(data->lignes);

    free_tab_station(data->stations);
}
