#include <vector>
#include <iostream>
#include <queue>
#include "graaf.h"

using namespace std;

struct CustomCompare
{
    bool operator()(const pair<int, int> &lhs, const pair<int, int> &rhs)
    {
        return lhs.second > rhs.second;
    }
};

vector<int> dijkstra(GraafMetTakdata<GERICHT, int> &graaf, GraafMetTakdata<GERICHT, int> &echteGraaf, int startknoop)
{
    vector<int> afstanden(graaf.aantalKnopen(), INT_MAX - 1);
    vector<bool> bezocht(graaf.aantalKnopen(), false);
    vector<int> echteGewichten(echteGraaf.aantalKnopen(), INT_MAX - 1);
    echteGewichten[startknoop] = 0;
    priority_queue<pair<int, int>, vector<pair<int, int>>, CustomCompare> knopen;
    pair<int, int> startPair(startknoop, 0);
    knopen.push(startPair);

    while (!knopen.empty())
    {
        pair<int, int> knoop = knopen.top();
        bezocht[knoop.first] = true;
        afstanden[knoop.first] = knoop.second;
        knopen.pop();

        for (auto it = graaf[knoop.first].begin(); it != graaf[knoop.first].end(); it++)
        {
            if (!bezocht[it->first] && afstanden[knoop.first] + *(graaf.geefTakdata(knoop.first, it->first)) < afstanden[it->first])
            {
                pair<int, int> nieuwe_knoop(it->first, afstanden[knoop.first] + *(graaf.geefTakdata(knoop.first, it->first)));
                if (it->first < echteGraaf.aantalKnopen())
                    echteGewichten[it->first] = echteGewichten[knoop.first] + *(echteGraaf.geefTakdata(knoop.first, it->first));
                knopen.push(nieuwe_knoop);
            }
        }
    }

    return echteGewichten;
}