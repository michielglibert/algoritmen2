#include <vector>
#include <iostream>
#include <queue>
#include <deque>
#include "graaf.h"

using namespace std;

bool negatieveCykel(GraafMetTakdata<GERICHT, int> &graaf, vector<int> &kortsteAfstanden)
{
    for (int i = 0; i < graaf.aantalKnopen(); i++)
    {
        for (auto it = graaf[i].begin(); it != graaf[i].end(); it++)
        {
            if (kortsteAfstanden[i] != INT_MAX - 1 && kortsteAfstanden[i] + *(graaf.geefTakdata(i, it->first)) < kortsteAfstanden[it->first])
            {
                cerr << "Negatieve cykel gevonden, resultaat ongeldigd" << endl;
                return true;
            }
        }
    }
    return false;
}

vector<int> bellman(GraafMetTakdata<GERICHT, int> &graaf, int startknoop)
{
    //INT_MAX met -1 erbij omdat anders 2147M = -2147M
    vector<int> kortsteAfstanden(graaf.aantalKnopen(), INT_MAX - 1);
    kortsteAfstanden[startknoop] = 0;

    //n-1 iteraties
    for (int i = 0; i < graaf.aantalKnopen() - 1; i++)
    {
        //Alle verbindingen overlopen
        for (int j = 0; j < graaf.aantalKnopen(); j++)
        {
            for (auto it = graaf[j].begin(); it != graaf[j].end(); it++)
            {
                if (kortsteAfstanden[j] + *(graaf.geefTakdata(j, it->first)) < kortsteAfstanden[it->first])
                    kortsteAfstanden[it->first] = kortsteAfstanden[j] + *(graaf.geefTakdata(j, it->first));
            }
        }
    }

    negatieveCykel(graaf, kortsteAfstanden);
    return kortsteAfstanden;
}

vector<int> bellman_verbeterd(GraafMetTakdata<GERICHT, int> &graaf, int startknoop)
{
    //INT_MAX met -1 erbij omdat anders 2147M = -2147M
    vector<int> kortsteAfstanden(graaf.aantalKnopen(), INT_MAX - 1);
    kortsteAfstanden[startknoop] = 0;

    //De enigste reden dat deze gesplitst zijn is zodat het algoritme niet in een oneindige lus terechtkomt
    //in het geval dat er een negatieve cykel aanwezig is.
    queue<int> knopen, veranderingen;
    veranderingen.push(startknoop);

    //n-1 iteraties
    for (int i = 0; i < graaf.aantalKnopen() - 1; i++)
    {
        while (!veranderingen.empty())
        {
            int knoop = veranderingen.front();
            veranderingen.pop();
            for (auto it = graaf[knoop].begin(); it != graaf[knoop].end(); it++)
            {
                if (kortsteAfstanden[knoop] + *(graaf.geefTakdata(knoop, it->first)) < kortsteAfstanden[it->first])
                {
                    kortsteAfstanden[it->first] = kortsteAfstanden[knoop] + *(graaf.geefTakdata(knoop, it->first));
                    knopen.push(it->first);
                }
            }
        }

        while (!knopen.empty())
        {
            veranderingen.push(knopen.front());
            knopen.pop();
        }
    }

    negatieveCykel(graaf, kortsteAfstanden);
    return kortsteAfstanden;
}

vector<int> bellman_deque(GraafMetTakdata<GERICHT, int> &graaf, int startknoop)
{
    //INT_MAX met -1 erbij omdat anders 2147M = -2147M
    vector<int> kortsteAfstanden(graaf.aantalKnopen(), INT_MAX - 1);
    kortsteAfstanden[startknoop] = 0;

    vector<bool> alToegevoegd(graaf.aantalKnopen(), false);
    deque<int> veranderingen;
    queue<int> knopen;
    veranderingen.push_back(startknoop);

    //n-1 iteraties
    for (int i = 0; i < graaf.aantalKnopen() - 1; i++)
    {
        while (!veranderingen.empty())
        {
            int knoop = veranderingen.front();
            veranderingen.pop_front();
            for (auto it = graaf[knoop].begin(); it != graaf[knoop].end(); it++)
            {
                if (kortsteAfstanden[knoop] + *(graaf.geefTakdata(knoop, it->first)) < kortsteAfstanden[it->first])
                {
                    kortsteAfstanden[it->first] = kortsteAfstanden[knoop] + *(graaf.geefTakdata(knoop, it->first));
                    knopen.push(it->first);
                }
            }
        }

        while (!knopen.empty())
        {
            int knoop = knopen.front();
            knopen.pop();
            if (alToegevoegd[knoop])
            {
                veranderingen.push_front(knoop);
            }
            else
            {
                alToegevoegd[knoop] = true;
                veranderingen.push_back(knoop);
            }
        }
    }

    negatieveCykel(graaf, kortsteAfstanden);
    return kortsteAfstanden;
}