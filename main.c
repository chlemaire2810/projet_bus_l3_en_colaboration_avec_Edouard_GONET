// NOM prenom etudiant 1 : Lemaire Chloe
// NOM prenom etudiant 2 : Gonet Edouard

// UTILISER SI SOUS CODEBLOCKS
// #include "SDL.h"

// UTILISER SI SOUS UBUNTU AVEC SDL2 INSTALLE
#include <SDL2/SDL.h>

// UTILISER SI SOUS MAC AVEC SDL2 INSTALLE
// #include <SDL.h>

// #include "types.h"
// #include "lectureFichier.h"

#include "maSDL.h"
#include "SDL_VSYNC.h"
#include "donnees_bus.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <stdio.h>
#include <stdbool.h>

/*--------- Main ---------------------*/
int main(void)
{
    SDL_Window *gWindow = NULL;

    // la texture (charg�e dans la ram de la carte graphique) target representant tout le jeu
    SDL_Renderer *gRenderer = NULL;

    // Les textures, le type Ltexture est d�fini dans SDL_VSYNC.h
    LTexture gSpriteArretBus;

    // Start up SDL and create window
    printf("Lancement du projet\n");
    if (!init(&gWindow, &gRenderer)) // initialise la fenetre SDL et charge UNE texture
    {
        printf("Erreur d'initialisation\n");
        return 0;
    }
    else
    {
        // Load media
        if (!loadSprite(&gSpriteArretBus, gRenderer, "./data/arret_bus_50x27.png"))
        {
            fprintf(stdout, "échec de chargement du sprite (%s)\n", SDL_GetError());
        }
        else
        {
            message("Bienvenue dans notre Projet",
                    "");
            message("Pour quitter le projet",
                    "ECHAP -> quitter \n Lire le ReadMe pour utiliser les touches");

            /* ******************************************************************** */
            /*                                                                    */
            /*              DEFINISSEZ/INITIALISER ICI VOS VARIABLES              */
            /*                                                                    */
            /**********************************************************************/

            T_data_tab *data;
            TbusDataTab *bus_tab;
            T_tab_ligne *tab_ligne;
            bus_tab = init_bus_data_tab();
            printf("on commance la lecture des sauvegardes\n");
            data = lecture_sauvegarde("./sauvegarde/lignesDeBus.data", "./sauvegarde/stations.data");
            tab_ligne = get_tab_ligne_from_data(data);

            printf("lecture de sauvegarde a fini, on passe au bus\n");

            // initialisation des bus
            for (int i = 0; i < get_num_ligne(tab_ligne); i++)
            {
                printf("on essaye d'initialiser un bus avec un id=%d\n", i + 1);
                TbusData *bus = init_bus_data(i + 1, get_ligne_bus_from_tab_ligne(tab_ligne, i), gRenderer);
                printf("on a bien initialise le bus \n");
                printf("on a fait un bus avec l'id : %d\n", getIdBus(get_bus(bus)));
                if (bus != NULL)
                {
                    add_bus_to_tab(bus_tab, *bus);
                }
            }
            printf("initialisation des bus fini\n");

            int frame = 0;
            // boucle principale du jeu
            int cont = 1;

            while (cont != 0)
            {
                SDL_PumpEvents(); // do events
                const Uint8 *pKeyStates = SDL_GetKeyboardState(NULL);

                //  deplaceBus g�re e d�placement du bus sur sa ligne, selon son sens du parcours de la ligne;
                //  met � jour les variations en X et Y pour d�placer le sprite du Bus (cf ligne 151)
                //  deplaceBus(bus1, getSensParcours(bus1), &incXDeplSpriteBus1, &incYDeplSpriteBus1);
                // printf("DOING BUS LAPS\n");
                for (int i = 0; i < get_num_bus_from_tab(bus_tab); i++)
                {
                    TbusData *bus_data = get_bus_data_from_tab(bus_tab, i);
                    Tbus bus = get_bus(bus_data);
                    deplaceBus(
                        bus, getSensParcours(bus), get_x_inc_sprite_bus(bus_data), get_y_inc_sprite_bus(bus_data));
                    // afficheListe(getActualStation(bus));
                    if (getIdStation(getPtrData(getActualStation(bus))) == get_id_start_bus(bus_data) && !get_tours_compte_bus(bus_data))
                    {
                        set_num_tours_bus(bus_data, get_num_tours_bus(bus_data) + 1);
                        printf("\n\n\nTOUR %d\n", get_num_tours_bus(bus_data));
                        set_tours_compte_bus(bus_data, true);
                    }
                    if (getIdStation(getPtrData(getActualStation(bus))) != get_id_start_bus(bus_data))
                    {
                        set_tours_compte_bus(bus_data, false);
                    }
                }

                // les touches sont lues en Qwerty
                // printf("DOING SCAN CODES\n");
                if (pKeyStates[SDL_SCANCODE_W])
                {
                    // Ajouter vos appels de fonctions ci-dessous qd le joueur appuye sur W
                    printf("\nTouche W, sens déplacement depart vers terminus\n");
                    for (int i = 0; i < get_num_bus_from_tab(bus_tab); i++)
                    {
                        TbusData *bus_data = get_bus_data_from_tab(bus_tab, i);
                        Tbus bus = get_bus(bus_data);
                        setSensParcours(bus, depart_vers_arrivee);
                    }
                }
                if (pKeyStates[SDL_SCANCODE_T])
                {
                    int index_ligne;
                    int type_tri;
                    int filtre;

                    printf("\nNombre total de lignes disponibles : %d\n", get_num_ligne(tab_ligne));
                    printf("\nChoisissez l'indice de la ligne à trier (entre 1 et %d) : ", get_num_ligne(tab_ligne));
                    scanf("%d", &index_ligne);
                    index_ligne--; // car l'utilisateur entre à partir de 1

                    if (index_ligne < 0 || index_ligne >= get_num_ligne(tab_ligne))
                    {
                        printf("Indice de ligne invalide.\n");
                    }
                    else
                    {
                        printf("Par quoi trier ? (1 = coût maintenance, 2 = date dernière maintenance) : ");
                        scanf("%d", &type_tri);

                        printf("Avec quel filtre ? (0 = aucun, 1 = seulement stations, 2 = seulement tronçons) : ");
                        scanf("%d", &filtre);

                        T_liste ligne = get_ligne_bus_from_tab_ligne(tab_ligne, index_ligne);

                        printf("\n--- TRI ---\n");

                        if (type_tri == 1) // coût
                        {
                            if (filtre == 0)
                            {
                                printf("Tri global par coût maintenance (stations + tronçons)\n");
                                afficheListe(tri_selection(ligne, plus_petit_cout_maintenance));
                            }
                            else if (filtre == 1)
                            {
                                printf("Tri par coût maintenance (stations uniquement)\n");
                                afficheListe(tri_selection(ligne, plus_petit_cout_maintenance_station));
                            }
                            else if (filtre == 2)
                            {
                                printf("Tri par coût maintenance (tronçons uniquement)\n");
                                afficheListe(tri_selection(ligne, plus_petit_cout_maintenance_troncon));
                            }
                            else
                            {
                                printf("Filtre invalide\n");
                            }
                        }
                        else if (type_tri == 2) // date
                        {
                            if (filtre == 0)
                            {
                                printf("Tri global par date dernière maintenance (stations + tronçons)\n");
                                afficheListe(tri_selection(ligne, date_derniere_maintenance_recent));
                            }
                            else if (filtre == 1)
                            {
                                printf("Tri par date dernière maintenance (stations uniquement)\n");
                                afficheListe(tri_selection(ligne, date_derniere_maintenance_recent_station));
                            }
                            else if (filtre == 2)
                            {
                                printf("Tri par date dernière maintenance (tronçons uniquement)\n");
                                afficheListe(tri_selection(ligne, date_derniere_maintenance_recent_troncon));
                            }
                            else
                            {
                                printf("Filtre invalide\n");
                            }
                        }
                        else
                        {
                            printf("Type de tri invalide\n");
                        }

                        printf("--- Fin du tri ---\n");
                    }
                }
                if (pKeyStates[SDL_SCANCODE_F])
                {
                    int l1, l2;
                    printf("\nFusion de deux lignes de bus\n");
                    printf("Nombre total de lignes disponibles : %d\n", get_num_ligne(tab_ligne));

                    printf("Entrez l'indice de la première ligne à fusionner (entre 1 et %d) : ", get_num_ligne(tab_ligne));
                    scanf("%d", &l1);
                    l1--;

                    printf("Entrez l'indice de la deuxième ligne à fusionner (entre 1 et %d) : ", get_num_ligne(tab_ligne));
                    scanf("%d", &l2);
                    l2--;

                    if (l1 >= 0 && l1 < get_num_ligne(tab_ligne) && l2 >= 0 && l2 < get_num_ligne(tab_ligne))
                    {
                        fusion_ligne_de_bus(tab_ligne, l1, l2);
                        printf("Fusion des lignes %d et %d terminée.\n", l1 + 1, l2 + 1);

                        // Attention : récupère la nouvelle ligne
                        int nouvelle_ligne_index = get_num_ligne(tab_ligne) - 1;

                        // Nouvelle ligne -> start station
                        TlisteStation start_station = get_ligne_bus_from_tab_ligne(tab_ligne, nouvelle_ligne_index);

                        if (start_station != NULL)
                        {
                            int nouvel_id_bus = get_num_bus_from_tab(bus_tab); // le nouveau identifiant de bus est la nombre de bus dans le tableau
                            // si on effacait des bus il faudrait faire un compteur, mais puisque la on n'en enleve pas
                            // on a qu'a faire ca.

                            printf("Création du nouveau bus ID %d sur ligne %d.\n", nouvel_id_bus, nouvelle_ligne_index + 1);

                            // CREATION DU TBUSDATA, PAS juste un TBUS
                            TbusData *nouveau_bus_data = init_bus_data(nouvel_id_bus, start_station, gRenderer);

                            if (nouveau_bus_data != NULL)
                            {
                                add_bus_to_tab(bus_tab, *nouveau_bus_data);
                                printf("Nouveau bus ajouté dans bus_tab. Tout est OK.\n");
                            }
                            else
                            {
                                printf("Erreur de création du TbusData.\n");
                            }
                        }
                        else
                        {
                            printf("Erreur : Impossible de trouver la station de départ.\n");
                        }
                    }
                    else
                    {
                        printf("Indices de lignes invalides. Fusion annulée.\n");
                    }
                }

                if (pKeyStates[SDL_SCANCODE_C])
                {
                    int id_ligne;

                    printf("\nCircularisation d'une ligne\n");
                    printf("Nombre total de lignes disponibles : %d\n", get_num_ligne(tab_ligne));
                    printf("Entrez l'indice de la ligne à circulariser (entre 1 et %d) : ", get_num_ligne(tab_ligne));
                    scanf("%d", &id_ligne);
                    id_ligne--; // Car l'utilisateur entre à partir de 1

                    if (id_ligne >= 0 && id_ligne < get_num_ligne(tab_ligne))
                    {
                        int result = circuralise_ligne_de_bus(tab_ligne, id_ligne);
                        if (result == 0)
                        {
                            printf("La ligne %d a été circularisée avec succès.\n", id_ligne + 1);
                            TbusData *bus_data = get_bus_data_from_tab(bus_tab, id_ligne);
                            Tbus bus = get_bus(bus_data);
                            busSurStation(bus,
                                          get_ligne_bus_from_tab_ligne(tab_ligne, id_ligne),
                                          depart_vers_arrivee);
                            Affiche_Sprite(get_bus_texture(bus_data), gRenderer, getPosXBus(bus), getPosYBus(bus), getIdFrame(frame));
                        }
                        else
                        {
                            printf("Une erreur est survenue lors de la circularisation de la ligne %d.\n", id_ligne + 1);
                        }
                    }
                    else
                    {
                        printf("Indice de ligne invalide. Opération annulée.\n");
                    }
                }
                if (pKeyStates[SDL_SCANCODE_X])
                {
                    int id_ligne, index_supr;

                    printf("\nSuppression d'une station\n");
                    printf("Nombre total de lignes disponibles : %d\n", get_num_ligne(tab_ligne));

                    printf("Entrez l'indice de la ligne dans laquelle supprimer une station (entre 1 et %d) : ", get_num_ligne(tab_ligne));
                    scanf("%d", &id_ligne);
                    id_ligne--;

                    if (id_ligne < 0 || id_ligne >= get_num_ligne(tab_ligne))
                    {
                        printf("Indice de ligne invalide. Opération annulée.\n");
                    }
                    else
                    {
                        printf("Entrez l'indice de la station à supprimer (en ne comptant que les stations, pas les tronçons) : ");
                        scanf("%d", &index_supr);
                        index_supr--;

                        int result = supr_station_de_ligne(tab_ligne, id_ligne, index_supr);
                        if (result == 0)
                        {
                            printf("Station d’indice %d supprimée de la ligne %d.\n", index_supr + 1, id_ligne + 1);
                            TbusData *bus_data = get_bus_data_from_tab(bus_tab, id_ligne);
                            Tbus bus = get_bus(bus_data);
                            busSurStation(bus,
                                          get_ligne_bus_from_tab_ligne(tab_ligne, id_ligne),
                                          depart_vers_arrivee);
                            Affiche_Sprite(get_bus_texture(bus_data), gRenderer, getPosXBus(bus), getPosYBus(bus), getIdFrame(frame));
                        }
                        else
                        {
                            printf("Échec de la suppression. Vérifiez que l’indice de station est valide.\n");
                        }
                    }
                }

                if (pKeyStates[SDL_SCANCODE_S])
                {
                    printf("\nTouche S, sens deplacement terminus vers depart\n");
                    for (int i = 0; i < get_num_bus_from_tab(bus_tab); i++)
                    {
                        TbusData *bus_data = get_bus_data_from_tab(bus_tab, i);
                        Tbus bus = get_bus(bus_data);
                        setSensParcours(bus, arrivee_vers_depart);
                    }
                }

                if (pKeyStates[SDL_SCANCODE_R])
                {
                    printf("on recomance\n");
                    for (int i = 0; i < get_num_bus_from_tab(bus_tab); i++)
                    {
                        TbusData *bus_data = get_bus_data_from_tab(bus_tab, i);
                        Tbus bus = get_bus(bus_data);
                        busSurStation(bus,
                                      get_ligne_bus_from_tab_ligne(tab_ligne, i),
                                      depart_vers_arrivee);
                        Affiche_Sprite(get_bus_texture(bus_data), gRenderer, getPosXBus(bus), getPosYBus(bus), getIdFrame(frame));
                    }
                }

                if (pKeyStates[SDL_SCANCODE_ESCAPE])
                {
                    printf("\nTouche ECHAP\n");
                    cont = 0; // sortie de la boucle
                }

                // printf("RENDERING\n");
                //  affichage du jeu � chaque tour
                //  on efface toute la fen�tre
                efface_fenetre_texture(gRenderer);

                // D�placement de sprite du bus sur la texture
                for (int i = 0; i < get_num_bus_from_tab(bus_tab); i++)
                {
                    TbusData *bus_data = get_bus_data_from_tab(bus_tab, i);
                    Deplace_Sprite(get_bus_texture(bus_data),
                                   gRenderer,
                                   *get_x_inc_sprite_bus(bus_data),
                                   *get_y_inc_sprite_bus(bus_data),
                                   getIdFrame(frame));
                }

                // r�affichage � chaque tour de toutes les stations
                // TODO : encapsuler tab data etc etc
                for (int index_ligne = 0; index_ligne < get_num_ligne(tab_ligne); index_ligne++)
                {
                    if (get_ligne_bus_from_tab_ligne(tab_ligne, index_ligne) != NULL)
                    {
                        DessineUneLigneBus(
                            get_ligne_bus_from_tab_ligne(tab_ligne, index_ligne), gSpriteArretBus, gRenderer);
                    }
                }

                // affichage de la texture ainsi mis � jour
                maj_fenetre_texture(gRenderer);
                SDL_Delay(10); // valeur du d�lai modifiable en fonction de votre CPU
                ++frame;
            }
            // fin boucle du jeu

            free_bus_data_tab(bus_tab);

            free_data(data);
        }
    }
    // Free resources and close SDL
    free_Ltexture(&gSpriteArretBus);
    close(gWindow, gRenderer);

    return 0;
}
