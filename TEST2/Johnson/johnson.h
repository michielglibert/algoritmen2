#ifndef __JOHNSON_H
#define __JOHNSON_H

#include <vector>
#include "graaf.h"
#include "dijkstra.h"
#include "bellman.h"

using namespace std;

vector<vector<int>> johnson(GraafMetTakdata<GERICHT, int> &graaf)
{
    vector<vector<int>> alle_afstanden(graaf.aantalKnopen());

    GraafMetTakdata<GERICHT, int> graafkopie = graaf;

    //Stap 1. Voeg een knoop toe aan de graaf en verbindt deze met elke knoop
    int s = graafkopie.voegKnoopToe();

    for (int i = 0; i < graafkopie.aantalKnopen(); i++)
    {
        if (i != s)
            graafkopie.voegVerbindingToe(s, i, 0);
    }

    //Stap 2. Voer Bellman Ford uit op de graaf
    vector<int> afstanden = bellman_deque(graafkopie, s);

    //Stap 3. Pas de gewichten aan
    for (int i = 0; i < graafkopie.aantalKnopen(); i++)
    {
        for (auto it = graafkopie[i].begin(); it != graafkopie[i].end(); it++)
        {
            int *data = graafkopie.geefTakdata(i, it->first);
            *data = *data + afstanden[i] - afstanden[it->first];
        }
    }

    //Stap 4. Voer dijkstra uit op elke knoop
    for (int i = 0; i < graafkopie.aantalKnopen(); i++)
    {
        if (i != s)
        {
            vector<int> afstanden_voorknoop = dijkstra(graafkopie, graaf, i);
            alle_afstanden[i] = move(afstanden_voorknoop);
        }
    }

    graafkopie.teken("aangepaste_graaf");

    return alle_afstanden;
}

#endif