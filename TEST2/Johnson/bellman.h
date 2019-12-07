#ifndef __BOYER_H
#define __BOYER_H

#include <vector>
#include <queue>
#include <deque>
#include "graaf.h"

using namespace std;

bool checkNegatieveCykels(GraafMetTakdata<GERICHT, int> &graaf, vector<int> &kortstePaden)
{
    for (int i = 0; i < graaf.aantalKnopen(); i++)
    {
        for (auto it = graaf[i].begin(); it != graaf[i].end(); it++)
        {
            if (kortstePaden[i] + *(graaf.geefTakdata(i, it->first)) < kortstePaden[it->first])
            {
                cerr << "Negatieve cykel gevonden, resultaat ongeldigd" << endl;
                return true;
            }
        }
    }
    return false;
}

vector<int> bellman(GraafMetTakdata<GERICHT, int> &graaf, int startKnoop)
{
    //We initaliseren de tabel met kortste afstanden
    //Initieel is de startknoop 0 en de andere zijn INT_MAX
    vector<int> kortstePaden(graaf.aantalKnopen(), INT_MAX);
    kortstePaden[startKnoop] = 0;

    //Veranderd zal aangeven of het algoritme al vroeger zou moge stoppen
    //Als er niks veranderd dan wil dat zeggen dat de kortste paden al gevonden zijn
    //We moeten lussen vermijden door ook ervoor te zorgen dat men niet meer iteraties
    //Doet dan het aantal knopen
    bool veranderd = false;
    int i = 0;
    do
    {
        for (int j = 0; j < graaf.aantalKnopen(); j++)
        {
            if (kortstePaden[j] < INT_MAX)
            {
                for (auto it = graaf[j].begin(); it != graaf[j].end(); it++)
                {
                    if (kortstePaden[j] + *(graaf.geefTakdata(j, it->first)) < kortstePaden[it->first])
                    {
                        kortstePaden[it->first] = kortstePaden[j] + *(graaf.geefTakdata(j, it->first));
                        veranderd = true;
                    }
                }
            }
        }
        i++;
    } while (veranderd && i < graaf.aantalKnopen());

    checkNegatieveCykels(graaf, kortstePaden);

    return kortstePaden;
}

vector<int> bellman_verbeterd(GraafMetTakdata<GERICHT, int> &graaf, int startKnoop)
{
    //We initaliseren de tabel met kortste afstanden
    //Initieel is de startknoop 0 en de andere zijn INT_MAX
    vector<int> kortstePaden(graaf.aantalKnopen(), INT_MAX);
    kortstePaden[startKnoop] = 0;

    //We maken 2 queues aan. 1 bevat de wijzigingen per iteratie en de
    //andere bevat de wijzingen die in de volgende iteratie moeten worden uitgevoerd
    queue<int> knoopQueue;
    queue<int> aanpassingen;
    aanpassingen.push(startKnoop);

    int i = 0;
    //Zouden er geen wijzigingen meer gebeurd zijn in een iteratie dan mag het algoritme al stoppen
    while (i < graaf.aantalKnopen() && !aanpassingen.empty())
    {
        //We overlopen alle aanpassingen uit de vorige iteratie
        //Of de startknoop indien het de eerste is
        while (!aanpassingen.empty())
        {
            int node = aanpassingen.front();
            aanpassingen.pop();

            if (kortstePaden[node] < INT_MAX)
            {
                for (auto it = graaf[node].begin(); it != graaf[node].end(); it++)
                {
                    if (kortstePaden[node] + *(graaf.geefTakdata(node, it->first)) < kortstePaden[it->first])
                    {
                        kortstePaden[it->first] = kortstePaden[node] + *(graaf.geefTakdata(node, it->first));
                        knoopQueue.push(it->first);
                    }
                }
            }
        }

        //We voegen al de wijzigngen toe aan de aanpassingen queue zodat de volgende tieratie
        //Deze kan verwerken
        while (!knoopQueue.empty())
        {
            int node = knoopQueue.front();
            knoopQueue.pop();
            aanpassingen.push(node);
        }
        i++;
    }

    checkNegatieveCykels(graaf, kortstePaden);

    return kortstePaden;
}

vector<int> bellman_deque(GraafMetTakdata<GERICHT, int> &graaf, int startKnoop)
{
    //We initaliseren de tabel met kortste afstanden
    //Initieel is de startknoop 0 en de andere zijn INT_MAX
    vector<int> kortstePaden(graaf.aantalKnopen(), INT_MAX);
    kortstePaden[startKnoop] = 0;

    //We houden de wijzingen nog steeds bij in de knoopQueue
    //Maar de aanpassingen voor de volgende iteratie worden wel
    //in een deque opgeslagen. Daarnaast houden we een vector bij
    //die zegt of het al eerder werd toegevoegd of niet.
    queue<int> knoopQueue;
    deque<int> aanpassingenDeque;
    vector<bool> alToegevoegd(graaf.aantalKnopen());
    aanpassingenDeque.push_back(startKnoop);

    int i = 0;
    //Zouden er geen wijzigingen meer gebeurd zijn in een iteratie dan mag het algoritme al stoppen
    while (i < graaf.aantalKnopen() && !aanpassingenDeque.empty())
    {
        //We overlopen alle aanpassingen uit de vorige iteratie
        //Of de startknoop indien het de eerste is
        while (!aanpassingenDeque.empty())
        {
            int node = aanpassingenDeque.front();
            aanpassingenDeque.pop_front();

            if (kortstePaden[node] < INT_MAX)
            {
                for (auto it = graaf[node].begin(); it != graaf[node].end(); it++)
                {
                    if (kortstePaden[node] + *(graaf.geefTakdata(node, it->first)) < kortstePaden[it->first])
                    {
                        kortstePaden[it->first] = kortstePaden[node] + *(graaf.geefTakdata(node, it->first));
                        knoopQueue.push(it->first);
                    }
                }
            }
        }

        //Indien we
        while (!knoopQueue.empty())
        {
            int node = knoopQueue.front();
            knoopQueue.pop();
            if (alToegevoegd[node])
            {
                aanpassingenDeque.push_front(node);
            }
            else
            {
                alToegevoegd[node] = true;
                aanpassingenDeque.push_back(node);
            }
        }
        i++;
    }

    checkNegatieveCykels(graaf, kortstePaden);

    return kortstePaden;
}

#endif