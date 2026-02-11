#include "donnees_bus.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

// fonctions principaux pour des donees de bus
TbusData *init_bus_data(int idBus, TlisteStation start, SDL_Renderer *gRenderer)
{
    TbusData *busData;
    busData = calloc(1, sizeof(TbusData));
    busData->gSpriteBus = calloc(1, sizeof(LTexture));
    printf("espace memoire du bus allouee\n");
    if (!loadAnimation(busData->gSpriteBus, gRenderer, "./data/4bus.png"))
    {
        fprintf(stderr, "echec de chargement du sprite (%s)\n", SDL_GetError());
        busData->id_start = -1;
    }
    else
    {
        printf("on ajoute les donnees n�c�saire au fonctionnement du bus\n");
        busData->incXDeplSpriteBus = calloc(1, sizeof(int));
        busData->incYDeplSpriteBus = calloc(1, sizeof(int));
        busData->num_tours = 0;
        busData->tours_compte = false;
        busData->id_start = getIdStation(getPtrData(start));
        busData->bus = creeBus(idBus, start);
        Affiche_Sprite(
            busData->gSpriteBus, gRenderer, getPosXBus(busData->bus), getPosYBus(busData->bus), getIdFrame(0));
    }
    return busData;
}

int *get_x_inc_sprite_bus(TbusData *busData)
{
    return busData->incXDeplSpriteBus;
}
int *get_y_inc_sprite_bus(TbusData *busData)
{
    return busData->incYDeplSpriteBus;
}

int get_num_tours_bus(TbusData *busData)
{
    return busData->num_tours;
}

int get_id_start_bus(TbusData *busData)
{
    return busData->id_start;
}

bool get_tours_compte_bus(TbusData *busData)
{
    return busData->tours_compte;
}

LTexture *get_bus_texture(TbusData *busData)
{
    return busData->gSpriteBus;
}

Tbus get_bus(TbusData *busData)
{
    return busData->bus;
}

void set_tours_compte_bus(TbusData *busData, bool val)
{
    busData->tours_compte = val;
}

void set_num_tours_bus(TbusData *busData, int value)
{
    busData->num_tours = value;
}

void free_bus_data(TbusData *busData)
{

    free_Ltexture(get_bus_texture(busData));
    free(busData->incXDeplSpriteBus);
    free(busData->incYDeplSpriteBus);
    free(busData->bus);
}

// fonctions principaux pour un tableau dynamique de donnees de bus
TbusDataTab *init_bus_data_tab(void)
{
    TbusDataTab *res;
    res = calloc(1, sizeof(TbusDataTab));
    res->size = 1;
    res->tab_bus = calloc(1, sizeof(TbusData));
    return res;
}

int get_num_bus_from_tab(TbusDataTab *data_tab)
{
    return data_tab->num_values;
}
int get_size_tab(TbusDataTab *data_tab)
{
    return data_tab->size;
}

TbusData *get_bus_array(TbusDataTab *data_tab)
{
    return data_tab->tab_bus;
}

TbusData *get_bus_data_from_tab(TbusDataTab *data_tab, int index)
{
    if (index < 0 || index > data_tab->num_values)
    {
        fprintf(stderr,
                "get_bus_data_from_tab: ERROR INDEX %d OUT OF BOUNDS(amount of values = %d)\n",
                index,
                get_num_bus_from_tab(data_tab));
        return NULL;
    }
    return data_tab->tab_bus + index;
}

void add_size_to_tab(TbusDataTab *data_tab)
{
    TbusData *bus_data = get_bus_array(data_tab);
    TbusData *new_array = calloc(data_tab->size * 2, sizeof(TbusData));

    for (int i = 0; i < data_tab->num_values; i++)
    {
        new_array[i] = bus_data[i];
    }
    data_tab->size *= 2;
    data_tab->tab_bus = new_array;
    free(bus_data);
}

void add_bus_to_tab(TbusDataTab *data_tab, TbusData bus_data)
{
    data_tab->tab_bus[data_tab->num_values++] = bus_data;
    if (data_tab->size == data_tab->num_values)
    {
        add_size_to_tab(data_tab);
    }
}

void free_bus_data_tab(TbusDataTab *data_tab)
{
    for (int i = 0; i < get_num_bus_from_tab(data_tab); i++)
    {
        free_bus_data(get_bus_data_from_tab(data_tab, i));
    }
    free(data_tab->tab_bus);
    free(data_tab);
}
