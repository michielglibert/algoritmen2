#include <iostream>
#include <vector>
#include <string>
#include <stack>
#include "graaf.h"

using namespace std;

void behandel_knoop_omgekeerd(Graaf<GERICHT> &graaf, int knoop, vector<bool> &bezocht, stack<int> &postordernummer)
{
    bezocht[knoop] = true;
    for (auto it = graaf[knoop].begin(); it != graaf[knoop].end(); it++)
    {
        if (!bezocht[it->first])
            behandel_knoop_omgekeerd(graaf, it->first, bezocht, postordernummer);
    }

    postordernummer.push(knoop);
}

void behandel_knoop(Graaf<GERICHT> &graaf, int knoop, vector<bool> &bezocht)
{
    bezocht[knoop] = true;
    for (auto it = graaf[knoop].begin(); it != graaf[knoop].end(); it++)
    {
        if (!bezocht[it->first])
            behandel_knoop(graaf, it->first, bezocht);
    }
}

void diepte_eerst(Graaf<GERICHT> &graaf, stack<int> &postordernummer)
{
    int aantal = 0;
    vector<bool> bezocht(graaf.aantalKnopen());

    while (!postordernummer.empty())
    {
        int knoop = postordernummer.top();
        postordernummer.pop();

        if (!bezocht[knoop])
        {
            behandel_knoop(graaf, knoop, bezocht);
            cout << "Ik heb stofzuiger " << ++aantal << " verkocht aan klant " << knoop << endl;
        }
    }
    cout << "Ik heb in totaal " << aantal << " stofzuigers verkocht" << endl;
}

void diepte_eerst_omgekeerd(Graaf<GERICHT> &graaf, stack<int> &postordernummer)
{
    vector<bool> bezocht(graaf.aantalKnopen());

    for (int i = 0; i < graaf.aantalKnopen(); i++)
    {
        if (!bezocht[i])
        {
            behandel_knoop_omgekeerd(graaf, i, bezocht, postordernummer);
        }
    }
}

void verkoop_stofzuigers(Graaf<GERICHT> &graaf, Graaf<GERICHT> &omgekeerd)
{
    stack<int> postordernummer;

    cout << "--- Slechte stofzuigers verkopen aan meeste mensen ---" << endl;
    diepte_eerst_omgekeerd(omgekeerd, postordernummer);
    diepte_eerst(graaf, postordernummer);

    cout << "--- Goede stofzuigers verkopen aan meeste mensen ---" << endl;
    diepte_eerst_omgekeerd(graaf, postordernummer);
    diepte_eerst(graaf, postordernummer);
}

int main()
{
    Graaf<GERICHT> graaf(5000);
    Graaf<GERICHT> omgekeerd(5000);

    ifstream input{"rss.txt"};
    assert(input);
    string s;
    int sub, prod;
    while (input >> s >> sub >> s >> s >> s >> s >> s >> prod)
    {
        try
        {
            graaf.voegVerbindingToe(prod, sub);
            omgekeerd.voegVerbindingToe(sub, prod);
        }
        catch (GraafExceptie ex)
        {
            //std::cout << "Er is al een verbinding tussen " << publisher << " en " << subscriber << "!" << std::endl;
        }
    }

    verkoop_stofzuigers(graaf, omgekeerd);

    return 0;
}