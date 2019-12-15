#include <iostream>
#include <queue>
#include "boyermoore.h"

using namespace std;

int main()
{
    string naald = "pxpypxpypxpx";
    string hooiberg = "pxpypxpypxpxpypxpypxpx";
    BoyerMoore bm(naald);
    queue<int> plaatsen = bm.zoek(hooiberg);
    queue<int> x = bm.zoekHorspool(hooiberg);
    queue<int> y = bm.zoekSunday(hooiberg);

    int i = 0;
    while (!plaatsen.empty())
    {
        plaatsen.pop();
        i++;
    }

    cout << "De naald komt " << i << " keer voor." << endl;

    i = 0;
    while (!x.empty())
    {
        x.pop();
        i++;
    }

    cout << "De naald komt " << i << " keer voor." << endl;

    i = 0;
    while (!y.empty())
    {
        y.pop();
        i++;
    }

    cout << "De naald komt " << i << " keer voor." << endl;

    return 0;
}