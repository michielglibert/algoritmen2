#ifndef __NA_H_
#define __NA_H_

#include <vector>
#include <iostream>
#include <queue>
#include "thompsonna.h"

using namespace std;

//Dit is een NA geen DA omdat een DA altijd 1 specifieke staat heeft
//Voor de overgang te doen moeten we eerder een gegevensstructuur bijhouden die alle overgangen aantoont
//in plaats van steeds te loopen door alle verbindingen uit de thompson NA. Een mogelijkheid hier zou een
//tabel zijn die dan voor elke staat de mogelijke overgangsstaat geeft.

//De vector van staten zorgt ervoor dat het een DA is.

//Daarnaast moeten enkel functies nog private/protected worden gezet.

//Na hoeft geen pointer te zijn

class NA
{
public:
    NA(const Regexp &re) : na(re)
    {
        staten.push_back(0);
    }
    int operator[](int idx);
    void setStaat(int, int);
    void pushStaat(int);
    void removeStaatByIndex(int);
    int overloop(const string &);
    void overgang(char, int, int);
    void reset();

private:
    vector<int> staten;
    ThompsonNA na;
};

int NA::operator[](int i)
{
    return staten[i];
}

void NA::setStaat(int i, int staat)
{
    this->staten[i] = staat;
}

void NA::pushStaat(int staat)
{
    this->staten.push_back(staat);
}

void NA::removeStaatByIndex(int index)
{
    this->staten.erase(staten.begin() + index);
}

int NA::overloop(const string &lijn)
{
    int aanwezig = 0;
    int j = 0;
    while (j < lijn.size())
    {
        queue<int> todo;
        for (int i = 0; i < staten.size(); i++)
            todo.push(staten[i]);
        staten.clear();

        while (!todo.empty())
        {
            int staat = todo.front();

            for (int i = 0; i < na->geefAantalVerbindingen(); i++)
            {
                if ((*na)[i].geefBron() == staat && (*na)[i].geefKarakter() == epsilon)
                {
                    todo.push((*na)[i].geefDoel());
                }
            }
            pushStaat(staat);

            todo.pop();
        }
        for (int statenTeller = 0; statenTeller < staten.size(); statenTeller++)
        {
            for (int i = 0; i < na->geefAantalVerbindingen(); i++)
            {
                if ((*na)[i].geefBron() == staten[statenTeller])
                {
                    overgang(lijn[j], i, statenTeller);
                    if (na->geefAantalStatenbits() - 1 == (*na)[i].geefDoel())
                    {
                        aanwezig++;
                    }
                }
            }
        }
        j++;

        if (staten.empty())
        {
            this->reset();
        }
    }
    return aanwezig;
}

void NA::overgang(char karakter, int verbindingnummer, int staatIndex)
{
    if (((*na)[verbindingnummer].geefKarakter() == karakter && (*na)[verbindingnummer].geefBron() == (*this)[staatIndex]) || (*na)[verbindingnummer].geefKarakter() == epsilon)
    {
        setStaat(staatIndex, (*na)[verbindingnummer].geefDoel());
    }
    else
    {
        removeStaatByIndex(staatIndex);
    }
}

void NA::reset()
{
    this->staten.clear();
    staten.push_back(0);
}

#endif