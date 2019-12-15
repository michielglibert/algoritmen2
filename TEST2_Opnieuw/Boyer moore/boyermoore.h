#ifndef __BOYERMOORE_H
#define __BOYERMOORE_H
#include <queue>
#include <iostream>
#include <cmath>

using namespace std;

typedef unsigned int uint;
typedef unsigned char uchar;

class BoyerMoore
{
public:
    BoyerMoore(const string &naald);
    //de zoekfunctie geeft in teller het aantal karaktervergelijkingen terug
    queue<int> zoek(const string &hooiberg);
    queue<int> zoekHorspool(const string &hooiberg);
    queue<int> zoekSunday(const string &hooiberg);

private:
    vector<int> badchar;
    vector<int> badchar_horspool;
    const string &naald;
};

//Variant Horspool -> Initialisatie op -1
BoyerMoore::BoyerMoore(const string &naald) : naald(naald), badchar(UCHAR_MAX), badchar_horspool(UCHAR_MAX, -1)
{
    for (int i = 0; i < naald.size(); i++)
    {
        badchar[naald[i]] = i;
    }

    for (int i = 0; i < naald.size(); i++)
    {
        badchar_horspool[naald[i]] = i;
    }
}

queue<int> BoyerMoore::zoek(const string &hooiberg)
{
    queue<int> matches;
    int aantalVergelijkingen = 0;
    int i = 0;
    while (i <= hooiberg.size() - naald.size())
    {
        int shift = 0;
        int j = naald.size() - 1;

        while (j >= 0 && naald[j] == hooiberg[i + j])
        {
            aantalVergelijkingen++;
            j--;
        }

        if (j < 0)
        {
            matches.push(i);
            shift = naald.size() - badchar[hooiberg[i + naald.size()]];
        }
        else
        {
            shift = i - badchar[hooiberg[i + j]];
        }

        i += max(1, shift);
    }
    cout << "Aantal vergelijkingen: " << aantalVergelijkingen << endl;
    return matches;
}

queue<int> BoyerMoore::zoekHorspool(const string &hooiberg)
{
    queue<int> matches;
    int aantalVergelijkingen = 0;
    int i = 0;
    while (i <= hooiberg.size() - naald.size())
    {
        int shift = 0;
        int j = naald.size() - 1;

        while (j >= 0 && naald[j] == hooiberg[i + j])
        {
            aantalVergelijkingen++;
            j--;
        }

        if (j < 0)
        {
            matches.push(i);
            shift = naald.size() - badchar_horspool[hooiberg[i + naald.size()]];
        }
        else
        {
            shift = naald.size() - badchar_horspool[hooiberg[i + naald.size()]];
        }
        i += shift;
    }
    cout << "Aantal vergelijkingen: " << aantalVergelijkingen << endl;
    return matches;
}

queue<int> BoyerMoore::zoekSunday(const string &hooiberg)
{
    queue<int> matches;
    int aantalVergelijkingen = 0;
    int i = 0;
    while (i <= hooiberg.size() - naald.size())
    {
        int shift = 0;
        int j = naald.size() - 1;

        while (j >= 0 && naald[j] == hooiberg[i + j])
        {
            aantalVergelijkingen++;
            j--;
        }

        if (j < 0)
        {
            matches.push(i);
            shift = naald.size() - badchar_horspool[hooiberg[i + naald.size()]];
        }
        else
        {
            shift = naald.size() - badchar_horspool[hooiberg[i + naald.size() + 1]];
        }

        i += shift;
    }
    cout << "Aantal vergelijkingen: " << aantalVergelijkingen << endl;
    return matches;
}

#endif
