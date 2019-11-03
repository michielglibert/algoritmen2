#include <iostream>
#include <vector>
#include "graaf.h"

using namespace std;

void behandelKnoop(Graaf<GERICHT> &graaf, int knoop, vector<bool> &bezocht, vector<int> &preorder, vector<int> &lowv, vector<pair<int, int>> &bruggen, int &teller)
{
    preorder[knoop] = teller++;
    lowv[knoop] = preorder[knoop];
    bezocht[knoop] = true;

    for (auto it = graaf[knoop].begin(); it != graaf[knoop].end(); it++)
    {
        if (!bezocht[it->first])
        {
            behandelKnoop(graaf, it->first, bezocht, preorder, lowv, bruggen, teller);
            lowv[knoop] = min(lowv[knoop], lowv[it->first]);
            if (preorder[knoop] < lowv[it->first])
            {
                pair<int, int> brug(knoop, it->first);
                bruggen.push_back(brug);
            }
        }
        else
        {
            lowv[knoop] = min(lowv[knoop], preorder[it->first]);
        }
    }
}

void behandelKnoop(Graaf<GERICHT> &graaf, int knoop, vector<bool> &bezocht, vector<int> &preorder, vector<int> &lowv, vector<bool> &scharnierpunten, int &teller)
{
    preorder[knoop] = teller++;
    lowv[knoop] = preorder[knoop];
    bezocht[knoop] = true;

    for (auto it = graaf[knoop].begin(); it != graaf[knoop].end(); it++)
    {
        if (!bezocht[it->first])
        {
            behandelKnoop(graaf, it->first, bezocht, preorder, lowv, scharnierpunten, teller);
            lowv[knoop] = min(lowv[knoop], lowv[it->first]);
            if (preorder[knoop] <= lowv[it->first])
            {
                scharnierpunten[knoop] = true;
            }
        }
        else
        {
            lowv[knoop] = min(lowv[knoop], preorder[it->first]);
        }
    }
}

vector<pair<int, int>> vindBruggen(Graaf<GERICHT> &graaf)
{
    vector<bool> bezocht(graaf.aantalKnopen());
    vector<int> preorder(graaf.aantalKnopen()), lowv(graaf.aantalKnopen());
    vector<pair<int, int>> bruggen;
    int teller = 0;

    for (int i = 0; i < graaf.aantalKnopen(); i++)
    {
        if (!bezocht[i])
        {
            behandelKnoop(graaf, i, bezocht, preorder, lowv, bruggen, teller);
        }
    }

    return bruggen;
}

vector<bool> vindScharnierpunten(Graaf<GERICHT> &graaf)
{
    vector<bool> bezocht(graaf.aantalKnopen());
    vector<int> preorder(graaf.aantalKnopen()), lowv(graaf.aantalKnopen());
    vector<bool> scharnierpunten(graaf.aantalKnopen());
    int teller = 0;

    for (int i = 0; i < graaf.aantalKnopen(); i++)
    {
        if (!bezocht[i])
        {
            behandelKnoop(graaf, i, bezocht, preorder, lowv, scharnierpunten, teller);
            if (graaf[i].size() <= 1)
                scharnierpunten[i] = false;
        }
    }

    return scharnierpunten;
}

int main()
{
    Graaf<GERICHT> graaf(9);
    graaf.voegVerbindingToe(0, 1);
    graaf.voegVerbindingToe(1, 2);
    graaf.voegVerbindingToe(2, 0);
    graaf.voegVerbindingToe(2, 5);
    graaf.voegVerbindingToe(2, 3);
    graaf.voegVerbindingToe(3, 4);
    graaf.voegVerbindingToe(5, 6);
    graaf.voegVerbindingToe(6, 7);
    graaf.voegVerbindingToe(7, 8);
    graaf.voegVerbindingToe(8, 5);
    graaf.teken("graaf");

    vector<pair<int, int>> bruggen = vindBruggen(graaf);
    assert(bruggen[0].first == 3 && bruggen[0].second == 4 && bruggen[1].first == 2 && bruggen[1].second == 3 && bruggen[2].first == 2 && bruggen[2].second == 5);
    for (int i = 0; i < bruggen.size(); i++)
    {
        cout << "Er is een brug van " << bruggen[i].first << " naar " << bruggen[i].second << endl;
    }

    vector<bool> scharnierpunten = vindScharnierpunten(graaf);
    assert(scharnierpunten[2] && scharnierpunten[3] && scharnierpunten[5]);
    for (int i = 0; i < scharnierpunten.size(); i++)
    {
        if (scharnierpunten[i])
        {
            cout << i << " is een scharnierpunt." << endl;
        }
    }

    cout << "Finished" << endl;

    return 0;
}