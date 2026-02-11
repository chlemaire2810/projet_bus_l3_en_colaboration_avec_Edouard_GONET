#ifndef DONNEES_BUS_H
#define DONNEES_BUS_H

#include "ligneBus.h"
#include "maSDL.h"
#include <stdbool.h>

typedef struct t_bus_data
{
    LTexture *gSpriteBus;
    int *incXDeplSpriteBus;
    int *incYDeplSpriteBus;
    int id_start;
    int num_tours;
    int tours_compte;
    Tbus bus;
} TbusData;

typedef struct t_bus_data_array
{
    int num_values;
    int size;
    TbusData *tab_bus;
} TbusDataTab;

TbusData *init_bus_data(int idBus, TlisteStation start, SDL_Renderer *gRenderer);
int *get_x_inc_sprite_bus(TbusData *busData);
int *get_y_inc_sprite_bus(TbusData *busData);
int get_num_tours_bus(TbusData *busData);
int get_id_start_bus(TbusData *busData);
bool get_tours_compte_bus(TbusData *busData);
LTexture *get_bus_texture(TbusData *busData);
Tbus get_bus(TbusData *busData);
void set_tours_compte_bus(TbusData *busData, bool val);
void set_num_tours_bus(TbusData *busData, int value);
void free_bus_data(TbusData *busData);

TbusDataTab *init_bus_data_tab(void);
int get_num_bus_from_tab(TbusDataTab *data_tab);
int get_size_tab(TbusDataTab *data_tab);
TbusData *get_bus_array(TbusDataTab *data_tab);
TbusData *get_bus_data_from_tab(TbusDataTab *data_tab, int index);
void add_size_to_tab(TbusDataTab *data_tab);
void add_bus_to_tab(TbusDataTab *data_tab, TbusData bus_data);
void free_bus_data_tab(TbusDataTab *data_tab);
#endif
