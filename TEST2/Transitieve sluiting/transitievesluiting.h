#ifndef __TRANSITIEVESLUITING_H
#define __TRANSITIEVESLUITING_H

#include <vector>
#include <queue>
#include <stack>
#include "graaf.h"

using namespace std;

//methode 1: breedte eerste zoeken (kan ook met DEZ)
Graaf<GERICHT> ts1(Graaf<GERICHT> graaf)
{
    for (int i = 0; i < graaf.aantalKnopen(); i++)
    {
        vector<bool> bezocht(graaf.aantalKnopen());
        vector<int> teMakenVerbindingen;
        queue<int> buren;
        buren.push(i);

        while (!buren.empty())
        {
            int knoop = buren.front();
            buren.pop();

            for (auto it = graaf[knoop].begin(); it != graaf[knoop].end(); it++)
            {
                if (!bezocht[it->first])
                {
                    bezocht[it->first] = true;
                    buren.push(it->first);
                    teMakenVerbindingen.emplace_back(it->first);
                }
            }
        }

        for (int j = 0; j < teMakenVerbindingen.size(); j++)
        {
            if (graaf.verbindingsnummer(i, teMakenVerbindingen[j]) == -1)
                graaf.voegVerbindingToe(i, teMakenVerbindingen[j]);
        }
    }

    return graaf;
}

void DEZrecursief(Graaf<GERICHT> &graaf, vector<bool> &bezocht, int knoop, int nummer, vector<int> &componenten)
{
    bezocht[knoop] = true;
    componenten[knoop] = nummer;
    for (auto it = graaf[knoop].begin(); it != graaf[knoop].end(); it++)
    {
        if (!bezocht[it->first])
        {
            DEZrecursief(graaf, bezocht, it->first, nummer, componenten);
        }
    }
}

void DEZrecursief(Graaf<GERICHT> &graaf, vector<bool> &bezocht, stack<int> &postorderNummering, int knoop)
{
    bezocht[knoop] = true;
    for (auto it = graaf[knoop].begin(); it != graaf[knoop].end(); it++)
    {
        if (!bezocht[it->first])
        {
            DEZrecursief(graaf, bezocht, postorderNummering, it->first);
        }
    }

    postorderNummering.push(knoop);
}

pair<vector<int>, int> geefComponenten(Graaf<GERICHT> &graaf, Graaf<GERICHT> &omgekeerdeGraaf)
{
    stack<int> postorderNummering;
    vector<bool> bezocht(omgekeerdeGraaf.aantalKnopen(), false);
    for (int i = 0; i < omgekeerdeGraaf.aantalKnopen(); i++)
    {
        if (!bezocht[i])
        {
            DEZrecursief(omgekeerdeGraaf, bezocht, postorderNummering, i);
        }
    }

    //Componenten in vector stoppen, per knoop geeft het componentnummer terug
    vector<int> componenten(graaf.aantalKnopen());
    std::fill(bezocht.begin(), bezocht.end(), false);

    int i = 0; //Componentnummer
    while (!postorderNummering.empty())
    {
        int knoop = postorderNummering.top();
        postorderNummering.pop();

        if (!bezocht[knoop])
        {
            DEZrecursief(graaf, bezocht, knoop, i++, componenten);
        }
    }

    pair<vector<int>, int> returnPair(componenten, i);

    return returnPair;
}

//methode 2: via componentengraaf
Graaf<GERICHT> ts2(Graaf<GERICHT> graaf)
{
    Graaf<GERICHT> omgekeerdeGraaf(graaf.aantalKnopen());

    //Verbindingen van de omgekeerde graaf maken
    for (int i = 0; i < graaf.aantalKnopen(); i++)
    {
        for (auto it = graaf[i].begin(); it != graaf[i].end(); it++)
        {
            omgekeerdeGraaf.voegVerbindingToe(it->first, i);
        }
    }

    //Vector met componenten maken
    pair<vector<int>, int> pair = geefComponenten(graaf, omgekeerdeGraaf);
    vector<int> componenten = move(pair.first);
    int aantalComponenten = move(pair.second);

    //Componentengraaf maken
    Graaf<GERICHT> componentenGraaf(aantalComponenten);

    for (int i = 0; i < graaf.aantalKnopen(); i++)
    {
        for (auto it = graaf[i].begin(); it != graaf[i].end(); it++)
        {
            if (componenten[i] != componenten[it->first] && componentenGraaf.verbindingsnummer(componenten[i], componenten[it->first]) == -1)
            {
                componentenGraaf.voegVerbindingToe(componenten[i], componenten[it->first]);
            }
        }
    }

    //We bepalen nu de transitieve sluiting van de componentengraaf
    //Deze is heel wat kleiner dus zullen we minder werk hebben
    auto sluiting = ts1(componentenGraaf);

    //Transitieve sluiting opstellen
    for (int i = 0; i < graaf.aantalKnopen(); i++)
    {
        for (int j = 0; j < graaf.aantalKnopen(); j++)
        {
            //Ofwel behoren ze tot dezelfde sterk samenhangende componenten, en zijn ze dus beiden voor elkaar bereikbaar
            //Ofwel bestaat er een verbinding tussen de componentengrafen
            if (i != j && graaf.verbindingsnummer(i, j) == -1 && (componenten[i] == componenten[j] || sluiting.verbindingsnummer(componenten[i], componenten[j]) != -1))
            {
                graaf.voegVerbindingToe(i, j);
            }
        }
    }

    return graaf;
}

//methode 3: floyd warshall
Graaf<GERICHT> ts3(Graaf<GERICHT> graaf)
{
    vector<vector<vector<bool>>> matrix(graaf.aantalKnopen());

    //Vector initialiseren
    for (int i = 0; i < graaf.aantalKnopen(); i++)
    {
        matrix[i].resize(graaf.aantalKnopen());
        for (int j = 0; j < graaf.aantalKnopen(); j++)
        {
            matrix[i][j].resize(graaf.aantalKnopen());
        }
    }

    //Eerste iteratie
    for (int i = 0; i < graaf.aantalKnopen(); i++)
    {
        for (int j = 0; j < graaf.aantalKnopen(); j++)
        {
            matrix[0][i][j] = (i == j || graaf.verbindingsnummer(i, j) != -1);
        }
    }

    //Volgende iteraties

    for (int k = 1; k < graaf.aantalKnopen(); k++)
    {
        for (int i = 0; i < graaf.aantalKnopen(); i++)
        {
            for (int j = 0; j < graaf.aantalKnopen(); j++)
            {
                matrix[k][i][j] = (matrix[k - 1][i][j] || (matrix[k - 1][i][k] && matrix[k - 1][k][j]));
            }
        }
    }

    for (int i = 0; i < graaf.aantalKnopen(); i++)
    {
        for (int j = 0; j < graaf.aantalKnopen(); j++)
        {
            if (i != j && matrix[graaf.aantalKnopen() - 1][i][j] && graaf.verbindingsnummer(i, j) == -1)
            {
                graaf.voegVerbindingToe(i, j);
            }
        }
    }

    return graaf;
}

#endif