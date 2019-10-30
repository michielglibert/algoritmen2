#include <iostream>
#include <fstream>
#include "graaf.h"
#include <cassert>
#include <string>
#include <stack>
#include <vector>

using namespace std;

void behandelKnoop(Graaf<GERICHT> &graaf, int knoopnummer, vector<bool> &bezocht, stack<int> &stack)
{
    // Markeer dat de knoop behandeld is
    bezocht[knoopnummer] = true;

    // Behandel elke buur van deze knoop
    for (auto it = graaf[knoopnummer].begin(); it != graaf[knoopnummer].end(); it++)
    {
        if (!bezocht[it->first])
        {
            behandelKnoop(graaf, it->first, bezocht, stack);
        }
    }

    // Geef de knoop een postordernummer
    stack.push(knoopnummer);
}

void behandelKnoop2(Graaf<GERICHT> &graaf, int knoop, vector<bool> &behandeld)
{
    // Markeer dat de knoop behandeld is
    behandeld[knoop] = true;

    // Behandel elke buur van deze knoop
    for (auto it = graaf[knoop].begin(); it != graaf[knoop].end(); it++)
    {
        if (!behandeld[it->first])
        {
            behandelKnoop2(graaf, it->first, behandeld);
        }
    }
}

void diepte_eerst(Graaf<GERICHT> &graaf, stack<int> &stack)
{
    vector<bool> bezocht(graaf.aantalKnopen());
    for (int i = 0; i < graaf.aantalKnopen(); i++)
    {
        if (!bezocht[i])
        {
            behandelKnoop(graaf, i, bezocht, stack);
        }
    }
}

void diepte_eerst_2(Graaf<GERICHT> &graaf, stack<int> &stack)
{
    int aantal = 0;
    vector<bool> bezocht(graaf.aantalKnopen());
    while (!stack.empty())
    {
        int nummer = stack.top();
        if (!bezocht[nummer])
        {
            behandelKnoop2(graaf, nummer, bezocht);
            std::cout << "Ik verkoop stofzuiger " << ++aantal << " aan gezin " << nummer << std::endl;
        }
        stack.pop();
    }
    cout << "Aantal stofzuigers " << aantal << "." << endl;
}

void verkoop_stofzuigers(Graaf<GERICHT> &graaf, Graaf<GERICHT> &omgekeerde_graaf)
{
    stack<int> stackk;

    diepte_eerst(omgekeerde_graaf, stackk);
    diepte_eerst_2(graaf, stackk);

    diepte_eerst(graaf, stackk);
    diepte_eerst_2(graaf, stackk);
}

int main()
{
    Graaf<GERICHT> graaf(5000);
    Graaf<GERICHT> omgekeerde_graaf(5000);
    ifstream in{"rss.txt"};
    assert(in);
    string s;
    int a, b;
    while (in >> s >> a >> s >> s >> s >> s >> s >> b)
    {
        try
        {

            graaf.voegVerbindingToe(b, a);
            omgekeerde_graaf.voegVerbindingToe(a, b);
        }
        catch (GraafExceptie &ex)
        {

            cerr << "[error] " << ex << endl;
        }
    }
    graaf.teken("teste");
    verkoop_stofzuigers(graaf, omgekeerde_graaf);
    cout << "Finished" << endl;
    return 0;
}