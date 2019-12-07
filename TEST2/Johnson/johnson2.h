#include "graaf.h"
#include "bellman.h"
#include "dijkstra.h"
#include <queue>
#include <vector>
#include <memory>
#include <iostream>
using std::make_unique;
using std::queue;
using std::unique_ptr;
using std::vector;

vector<vector<int>> johnson_twee(GraafMetTakdata<GERICHT, int> &graaf)
{
    // ----
    // 1. Nieuwe knoop s toevoegen met een uitgaande verbinding met gewicht 0 naar elke andere knoop
    // ----

    // maak een kopie van de originele graaf
    GraafMetTakdata<GERICHT, int> nieuw = graaf;
    // voeg de nieuwe knoop s toe
    int s = nieuw.voegKnoopToe();

    // voeg een uitgaande verbinding toe van s naar alle andere knopen met gewicht 0
    for (int knoop = 0; knoop < nieuw.aantalKnopen(); knoop++)
    {
        if (knoop != s)
        {
            nieuw.voegVerbindingToe(s, knoop, 0);
        }
    }

    // ----
    // 2. Pas Bellman-ford toe om vanuit de nieuwe knoop de kortste weg naar alle andere knopen te vinden
    // ----

    vector<int> d = bellman(nieuw, s);

    // ----
    // 3. Bepaal de nieuwe (positieve) gewichten Gij = gij + di - dj
    // ----
    for (int knoop = 0; knoop < graaf.aantalKnopen(); knoop++)
    {
        for (auto &[buur, tak] : graaf[knoop])
        {
            *(graaf.geefTakdata(knoop, buur)) = *(graaf.geefTakdata(knoop, buur)) + d[knoop] - d[buur];
        }
    }

    // ----
    // 4. Pas Dijkstra toe op elke knoop
    // ----
    vector<vector<int>> afstanden;

    for (int knoop = 0; knoop < nieuw.aantalKnopen(); knoop++)
    {
        if (knoop != s)
        {
            afstanden.push_back(dijkstra(nieuw, knoop));
        }
    }

    return afstanden;
}