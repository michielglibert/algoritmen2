#include "graaf.h"
#include <iostream>
#include <queue>
#include <stack>
#include <vector>

using namespace std;

// Met breedte eerst zoeken
Graaf<GERICHT> ts1(Graaf<GERICHT> graaf)
{
    for (int i = 0; i < graaf.aantalKnopen(); i++)
    {
        queue<int> wachtrij;
        vector<bool> bezocht(graaf.aantalKnopen(), false);
        wachtrij.push(i);

        while (!wachtrij.empty())
        {
            int knoop = wachtrij.front();
            wachtrij.pop();

            for (auto it = graaf[knoop].begin(); it != graaf[knoop].end(); it++)
            {
                if (!bezocht[it->first])
                {
                    if (graaf.verbindingsnummer(i, it->first) == -1)
                        graaf.voegVerbindingToe(i, it->first);
                    wachtrij.push(it->first);
                }
            }
        }
    }

    return graaf;
}

void DEZ_recursief(Graaf<GERICHT> &graaf, vector<bool> &bezocht, vector<int> &componentPerKnoop, int knoop, int componentNummer)
{
    componentPerKnoop[knoop] = componentNummer;
    bezocht[knoop] = true;
    for (auto it = graaf[knoop].begin(); it != graaf[knoop].end(); it++)
        if (!bezocht[it->first])
            DEZ_recursief(graaf, bezocht, componentPerKnoop, it->first, componentNummer);
}

void DEZ_recursief(Graaf<GERICHT> &graaf, vector<bool> &bezocht, int knoop, stack<int> &postorderStack)
{
    bezocht[knoop] = true;
    for (auto it = graaf[knoop].begin(); it != graaf[knoop].end(); it++)
        if (!bezocht[it->first])
            DEZ_recursief(graaf, bezocht, it->first, postorderStack);

    postorderStack.push(knoop);
}

pair<vector<int>, int> geefComponentenPerKnoop(Graaf<GERICHT> &graaf, Graaf<GERICHT> &omgekeerdeGraaf)
{
    vector<bool> bezocht(omgekeerdeGraaf.aantalKnopen(), false);
    stack<int> postorderStack;
    for (int i = 0; i < omgekeerdeGraaf.aantalKnopen(); i++)
    {
        if (!bezocht[i])
            DEZ_recursief(omgekeerdeGraaf, bezocht, i, postorderStack);
    }

    vector<int> componentPerKnoop(graaf.aantalKnopen());
    bezocht.clear();
    bezocht.resize(graaf.aantalKnopen(), false);

    int componentTeller = 0;
    while (!postorderStack.empty())
    {
        int knoop = postorderStack.top();
        postorderStack.pop();

        if (!bezocht[knoop])
        {
            DEZ_recursief(graaf, bezocht, componentPerKnoop, knoop, componentTeller++);
        }
    }

    pair<vector<int>, int> retPair(componentPerKnoop, componentTeller);
    return retPair;
}

// Met componenten graaf
Graaf<GERICHT> ts2(Graaf<GERICHT> graaf)
{
    // Omgekeerde graaf maken
    Graaf<GERICHT> omgekeerdeGraaf(graaf.aantalKnopen());
    for (int i = 0; i < graaf.aantalKnopen(); i++)
    {
        for (auto it = graaf[i].begin(); it != graaf[i].end(); it++)
        {
            omgekeerdeGraaf.voegVerbindingToe(it->first, i);
        }
    }

    pair<vector<int>, int> returnValue = geefComponentenPerKnoop(graaf, omgekeerdeGraaf);
    vector<int> componentPerKnoop = returnValue.first;
    int componentTeller = returnValue.second;

    Graaf<GERICHT> componentenGraaf(componentTeller);

    // Componentengraaf maken
    for (int i = 0; i < graaf.aantalKnopen(); i++)
    {
        cerr << componentPerKnoop[i] << endl;
        for (auto it = graaf[i].begin(); it != graaf[i].end(); it++)
        {
            if (componentPerKnoop[i] != componentPerKnoop[it->first] && componentenGraaf.verbindingsnummer(componentPerKnoop[i], componentPerKnoop[it->first]) == -1)
            {
                componentenGraaf.voegVerbindingToe(componentPerKnoop[i], componentPerKnoop[it->first]);
            }
        }
    }

    // Transitieve sluiting uitvoeren op componentengraaf
    Graaf<GERICHT> sluiting = ts1(componentenGraaf);

    // Is component X bereikbaar vanaf component Y ?

    for (int i = 0; i < graaf.aantalKnopen(); i++)
    {
        for (int j = 0; j < graaf.aantalKnopen(); j++)
        {
            if (i != j && (componentPerKnoop[i] == componentPerKnoop[j] || sluiting.verbindingsnummer(componentPerKnoop[i], componentPerKnoop[j]) != -1) && graaf.verbindingsnummer(i, j) == -1)
                graaf.voegVerbindingToe(i, j);
        }
    }

    return graaf;
}

// Met floyd warshall
Graaf<GERICHT> ts3(Graaf<GERICHT> graaf)
{
    vector<vector<vector<int>>> matrix(graaf.aantalKnopen());
    //n x n x n matrix maken
    for (int i = 0; i < graaf.aantalKnopen(); i++)
    {
        matrix[i].resize(graaf.aantalKnopen());
        for (int j = 0; j < graaf.aantalKnopen(); j++)
        {
            matrix[i][j].resize(graaf.aantalKnopen());
        }
    }

    //eerste iteratie
    for (int i = 0; i < graaf.aantalKnopen(); i++)
    {
        for (int j = 0; j < graaf.aantalKnopen(); j++)
        {
            matrix[0][i][j] = i == j || graaf.verbindingsnummer(i, j) != -1;
        }
    }
    //volgende iteraties
    for (int k = 1; k < graaf.aantalKnopen(); k++)
    {
        for (int i = 0; i < graaf.aantalKnopen(); i++)
        {
            for (int j = 0; j < graaf.aantalKnopen(); j++)
            {
                matrix[k][i][j] = matrix[k - 1][i][j] || (matrix[k - 1][i][k] && matrix[k - 1][k][j]);
            }
        }
    }

    //sluiting maken
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