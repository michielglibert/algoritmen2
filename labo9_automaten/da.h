#ifndef __DA_H_
#define __DA_H_

#include <vector>
#include <iostream>
#include "thompsonna.h"

using namespace std;

class DA
{
public:
    DA(const Regexp &&re) : na(ThompsonNA(re)) {}
    int geefStaat();
    bool overloop(string);
    void overgang(char, int);
    void reset();

private:
    int staat;
    ThompsonNA &na;
};

int DA::geefStaat()
{
    return this->staat;
}

bool DA::overloop(string lijn)
{
    int aanwezig = 0;

    for (int j = 0; j < lijn.size(); j++)
    {
        for (int i = 0; i < na.geefAantalVerbindingen(); i++)
        {
            overgang(lijn[j], i);
            if (na.geefAantalVerbindingen() == na[i].geefDoel && na[i].geefKarakter() == epsilon)
            {
                aanwezig++;
            }
        }
    }
}

void DA::overgang(char karakter, int verbindingnummer)
{
    if (na[verbindingnummer].geefKarakter() == epsilon)
    {
        this->staat++;
    }
    else if (na[verbindingnummer].geefKarakter() == karakter && na[verbindingnummer].geefBron() == this->staat)
    {
        this->staat = na[verbindingnummer].geefDoel();
    }
    else if (na[verbindingnummer].geefDoel() == na[verbindingnummer].geefBron() + 1)
    {
        this->reset();
    }
}

void DA::reset()
{
    this->staat = 0;
}

#endif