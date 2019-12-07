#ifndef __DIJKSTRA_H
#define __DIJKSTRA_H

#include <vector>
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

vector<int> dijkstra(GraafMetTakdata<GERICHT, int> &graaf, int startKnoop)
{
    vector<bool> bezocht(graaf.aantalKnopen());
    vector<int> kortstePaden(graaf.aantalKnopen(), INT_MAX);
    kortstePaden[startKnoop] = 0;
    priority_queue<pair<int, int>, vector<pair<int, int>>, CustomCompare> queue;
    pair<int, int> startknoop_pair(startKnoop, 0);
    queue.push(startknoop_pair);

    while (!queue.empty())
    {
        pair<int, int> knoop = queue.top();
        queue.pop();
        bezocht[knoop.first] = true;
        for (auto it = graaf[knoop.first].begin(); it != graaf[knoop.first].end(); it++)
        {
            if (bezocht[it->first])
                continue;
            if (*(graaf.geefTakdata(knoop.first, it->first)) + kortstePaden[knoop.first] < kortstePaden[it->first])
            {
                kortstePaden[it->first] = *(graaf.geefTakdata(knoop.first, it->first)) + kortstePaden[knoop.first];
                pair<int, int> nieuwe_knoop(it->first, kortstePaden[it->first]);
                queue.push(nieuwe_knoop);
            }
        }
    }

    return kortstePaden;
}

vector<int> dijkstra(GraafMetTakdata<GERICHT, int> &graaf, GraafMetTakdata<GERICHT, int> &gewichtengraaf, int startKnoop)
{
    vector<bool> bezocht(graaf.aantalKnopen());
    vector<int> kortstePaden(graaf.aantalKnopen(), INT_MAX);
    vector<int> kortstePaden_gewichten(graaf.aantalKnopen(), INT_MAX);
    kortstePaden[startKnoop] = 0;
    kortstePaden_gewichten[startKnoop] = 0;
    priority_queue<pair<int, int>, vector<pair<int, int>>, CustomCompare> queue;
    pair<int, int> startknoop_pair(startKnoop, 0);
    queue.push(startknoop_pair);

    while (!queue.empty())
    {
        pair<int, int> knoop = queue.top();
        queue.pop();
        bezocht[knoop.first] = true;
        for (auto it = graaf[knoop.first].begin(); it != graaf[knoop.first].end(); it++)
        {
            if (bezocht[it->first])
                continue;
            if (*(graaf.geefTakdata(knoop.first, it->first)) + kortstePaden[knoop.first] < kortstePaden[it->first])
            {
                kortstePaden[it->first] = *(graaf.geefTakdata(knoop.first, it->first)) + kortstePaden[knoop.first];
                kortstePaden_gewichten[it->first] = *(gewichtengraaf.geefTakdata(knoop.first, it->first)) + kortstePaden_gewichten[knoop.first];
                pair<int, int> nieuwe_knoop(it->first, kortstePaden[it->first]);
                queue.push(nieuwe_knoop);
            }
        }
    }

    return kortstePaden_gewichten;
}

#endif