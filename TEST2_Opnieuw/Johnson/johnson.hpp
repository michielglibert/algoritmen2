#include <iostream>
#include <vector>
#include "dijkstra.hpp"
#include "graaf.h"
#include "bellmanford.hpp"

using namespace std;

vector<vector<int>> johnson(GraafMetTakdata<GERICHT, int> &graaf)
{
    //Nieuwe knoop en verbindingen toevoegen aan tijdelijke graaf
    GraafMetTakdata<GERICHT, int> graafKopie = graaf;
    int s = graafKopie.voegKnoopToe();

    //Knoop komt sowieso op de laatste plaats terecht
    for (int i = 0; i < graaf.aantalKnopen(); i++)
    {
        graafKopie.voegVerbindingToe(s, i, 0);
    }

    //Bellman ford uitvoeren op knoop s
    vector<int> afstanden = bellman_deque(graafKopie, s);

    //Tijdelijke graaf aanpassen met nieuwe gewichten
    for (int i = 0; i < afstanden.size() - 1; i++)
    {
        for (auto it = graafKopie[i].begin(); it != graafKopie[i].end(); it++)
        {
            int afstand = *(graaf.geefTakdata(i, it->first)) + afstanden[i] - afstanden[it->first];
            *(graafKopie.geefTakdata(i, it->first)) = afstand;
        }
    }

    graafKopie.teken("shiz");

    vector<vector<int>> afstandenKnoopParen(graaf.aantalKnopen());
    //Op elke knoop dijkstra uitvoeren
    for (int i = 0; i < graaf.aantalKnopen(); i++)
    {
        afstandenKnoopParen[i] = dijkstra(graafKopie, graaf, i);
    }

    return afstandenKnoopParen;
}