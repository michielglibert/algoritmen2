#ifndef __SPLAYBOOM_H
#define __SPLAYBOOM_H

#include <iostream>
#include "zoekboom17.h"

using namespace std;

template <class Sleutel, class Data>
class Splayboom : public Zoekboom<Sleutel, Data>
{
public:
    void splay(zoekKnoop<Sleutel, Data> *);
    zoekKnoop<Sleutel, Data> &zoek(const Sleutel &sleutel);
    void voegtoe(const Sleutel &, const Data &, bool = false);
    void verwijder(const Sleutel &);
};

template <class Sleutel, class Data>
void Splayboom<Sleutel, Data>::splay(zoekKnoop<Sleutel, Data> *knoop)
{
}

template <class Sleutel, class Data>
zoekKnoop<Sleutel, Data> &Splayboom<Sleutel, Data>::zoek(const Sleutel &sleutel)
{
    zoekKnoop<Sleutel, Data> *knoop = (*this)->get();
    zoekKnoop<Sleutel, Data> *Lboom;
    zoekKnoop<Sleutel, Data> *Rboom;

    while (sleutel != knoop->sleutel)
    {
        if (sleutel < knoop->sleutel)
        {
            //Daal naar links af
            if ((sleutel < knoop->links->sleutel && knoop->links->links == nullptr) || (sleutel > knoop->links->sleutel && knoop->links->rechts == nullptr))
            {
                //Knoop is niet aanwezig
                Rboom->links;
            }
            else if (sleutel < knoop->links->sleutel)
            {
                //We moeten langs links afdalen
            }
            else
            {
                //We moeten langs rechts afdalen
            }
        }
        else
        {
            //Daal naar rechts af
        }
    }
}

template <class Sleutel, class Data>
void Splayboom<Sleutel, Data>::voegtoe(const Sleutel &sleutel, const Data &data, bool dubbelstoestaan)
{
}

template <class Sleutel, class Data>
void Splayboom<Sleutel, Data>::verwijder(const Sleutel &sleutel)
{
}

#endif