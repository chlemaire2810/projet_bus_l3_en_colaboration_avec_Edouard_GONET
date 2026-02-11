#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "tri.h"
#include "listeDouble.h"
#include "types.h"

bool comp_date_sup(t_date date_a, t_date date_b)
{
    if (date_a.annee < date_b.annee)
    {
        return(false);
    }
    if (date_a.annee > date_b.annee)
    {
        return(true);
    }
    if (date_a.mois < date_b.mois)
    {
        return(false);
    }
    if (date_a.mois > date_b.mois)
    {
        return(true);
    }
    if (date_a.jour < date_b.jour)
    {
        return(false);
    }
    return true ;
}

bool comp_cout_inf(int cout_a,int cout_b)
{
    return (cout_a<cout_b);
}

bool comp_cout_maintenance_station(Tstation *stationA,Tstation *stationB)
{
    return comp_cout_inf(get_cout_maintenance(stationA),get_cout_maintenance(stationB));
}

bool comp_station_date_derniere_maintenance_sup(Tstation *stationA,Tstation *stationB)
{
    return comp_date_sup(get_date_derniere_maintenance(stationA),get_date_derniere_maintenance(stationB));
}

bool date_derniere_maintenance_recent(T_liste a,T_liste b)
{
    return comp_station_date_derniere_maintenance_sup(getPtrData(a),getPtrData(b));
}
bool plus_petit_cout_maintenance(T_liste a,T_liste b)
{
    return comp_cout_maintenance_station(getPtrData(a),getPtrData(b));
}

void tri_selection_base(T_liste list, bool(*fcomp)(T_liste,T_liste))
{
    for (T_liste en_cours = list; getNextCell(en_cours) != NULL; en_cours = getNextCell(en_cours))
    {
        T_liste plus_petit = en_cours;
        for (T_liste j = getNextCell(en_cours); j != NULL ; j = getNextCell(j))
        {
            if (fcomp(j, plus_petit))
            {
                plus_petit=j;
            }
        }
        if(fcomp(plus_petit,en_cours))
        {
            swapPtrData(plus_petit, en_cours);
        }
    }

}

bool type_est_station(T_liste a,T_liste b)
{
    return(getTypeNoeud(getPtrData(a)) == ARRET && getTypeNoeud(getPtrData(b)) == ARRET);
}

bool type_est_troncon(T_liste a,T_liste b)
{
    return(getTypeNoeud(getPtrData(a)) == TRONCON && getTypeNoeud(getPtrData(b)) == TRONCON);
}

bool date_derniere_maintenance_recent_station(T_liste a,T_liste b)
{
    return comp_station_date_derniere_maintenance_sup(getPtrData(a),getPtrData(b)) && type_est_station(a,b);
}

bool plus_petit_cout_maintenance_station(T_liste a,T_liste b)
{
    return comp_cout_maintenance_station(getPtrData(a),getPtrData(b)) && type_est_station(a,b);
}

bool plus_petit_cout_maintenance_troncon(T_liste a,T_liste b)
{
    return comp_cout_maintenance_station(getPtrData(a),getPtrData(b)) && type_est_troncon(a,b);
}

bool date_derniere_maintenance_recent_troncon(T_liste a,T_liste b)
{
    return comp_station_date_derniere_maintenance_sup(getPtrData(a),getPtrData(b)) && type_est_troncon(a,b);
}


T_liste tri_selection(T_liste liste,bool(*fcomp)(T_liste,T_liste))
{
    T_liste dup = duplicate(liste);
    tri_selection_base(dup,fcomp);
    return dup;
}
