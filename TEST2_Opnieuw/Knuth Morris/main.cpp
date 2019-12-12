#include <iostream>
#include <queue>
#include "knuthmorris.hpp"

int main()
{
    string naald = "aaaaa";
    string hooiberg = "aaaaa";
    KnuthMorrisPratt kmp2(naald);
    KnuthMorrisPratt kmp((uchar *)naald.c_str(), naald.size());
    queue<int> plaats;
    int aantalVergelijkingen = 0;
    kmp.zoek(plaats, (uchar *)hooiberg.c_str(), hooiberg.size(), aantalVergelijkingen);

    queue<int> lol = kmp2.zoekr(hooiberg);
    int i = 0;
    while (!lol.empty())
    {
        lol.pop();
        i++;
    }

    cout << "Aantal gevonden: " << i << endl;

    i = 0;
    while (!plaats.empty())
    {
        plaats.pop();
        i++;
    }

    cout << "Aantal gevonden: " << i << endl;
    cout << "Aantal KMP vergelijkingen: " << aantalVergelijkingen << endl;

    return 0;
}