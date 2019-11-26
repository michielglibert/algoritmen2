#ifndef __BOYERMOORE_H
#define __BOYERMOORE_H

#include <queue>
#include <iostream>
#include <vector>
#include <string>
#include "woordstroom.h"

#define NO_OF_CHARS 256

typedef unsigned int uint;
typedef unsigned char uchar;

using std::cerr;
using std::cout;
using std::endl;
using std::queue;
using std::string;
using std::vector;

class BoyerMoore
{
public:
    BoyerMoore(const string &naald);
    //de zoekfunctie geeft in teller het aantal karaktervergelijkingen terug
    std::queue<int> zoek(const string &hooiberg, long int &teller);

private:
    const string &naald;
    vector<int> posities;
};

BoyerMoore::BoyerMoore(const string &naald) : naald(naald), posities(NO_OF_CHARS)
{
    for (int i = 0; i < NO_OF_CHARS; i++)
        posities[i] = -1;

    // Fill the actual value of last occurrence
    // of a character
    for (int i = 0; i < naald.size; i++)
        posities[(int)naald.at(i)] = i;
}

queue<int> BoyerMoore::zoek(const string &hooiberg, long int &teller)
{
    queue<int> plaatsen;
    int offset = 0;
    int j = naald.size() - 1;
    cerr << j << endl;

    //eerste heuristiek wrong char
    while (j >= 0 && offset + j <= hooiberg.size())
    {
        cout << "-------------Iteratie:" << endl;
        if (hooiberg.at(offset + j) == naald.at(j))
        {
            cout << "J verschuiven:" << j << endl;
            //J verschuiven
            j--;
        }
        else
        {
            if (posities[hooiberg.at(offset + j)] != -1)
            {
                //Opnieuw zoeken maar offset deels aanpassen
                offset += j - posities[hooiberg.at(offset + j)];
                cout << "Aanwezig, offset aanpassen: " << offset << endl;
                j = naald.size() - 1;
            }
            else
            {
                //Opnieuw zoeken maar offset volledig aanpassen
                offset += j + 1;
                cout << "Niet aanwezig offset aanpassen: " << offset << endl;
                j = naald.size() - 1;
            }
        }
        cout << endl;
        teller++;
    }
    if (j == -1)
        plaatsen.push(offset);

    return plaatsen;
}

#endif
