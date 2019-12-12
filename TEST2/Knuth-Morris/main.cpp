#include <iostream>
#include <queue>
#include "knuthmorris.h"

int main()
{
    string patroon = "pxpypxpypxpx";
    KnuthMorris km(patroon);
    long int teller = 0;
    string tekst = "pxpypxpypxpxpypxpypxpx";
    queue<int> posities = km.zoek(tekst, teller);
    cout << teller << endl;
    teller = 0;
    posities = km.zoekNaief(tekst, teller);
    cout << teller << endl;
    return 0;
}