#ifndef TRI_H_INCLUDED
#define TRI_H_INCLUDED

#include <stdbool.h>
#include "listeDouble.h"
#include "types.h"
bool comp_date_sup(t_date date_a, t_date date_b);
bool comp_cout_inf(int cout_a,int cout_b);
bool comp_cout_maintenance_station(Tstation *stationA,Tstation *stationB);
bool comp_station_date_derniere_maintenance_sup(Tstation *stationA,Tstation *stationB);
bool date_derniere_maintenance_recent(T_liste a,T_liste b);
bool plus_petit_cout_maintenance(T_liste a,T_liste b);

void tri_selection_base(T_liste list, bool(*fcomp)(T_liste,T_liste));

bool date_derniere_maintenance_recent_station(T_liste a,T_liste b);
bool plus_petit_cout_maintenance_station(T_liste a,T_liste b);
bool plus_petit_cout_maintenance_troncon(T_liste a,T_liste b);
bool date_derniere_maintenance_recent_troncon(T_liste a,T_liste b);

T_liste tri_selection(T_liste liste,bool(*fcomp)(T_liste,T_liste));

#endif // TRI_H_INCLUDED
