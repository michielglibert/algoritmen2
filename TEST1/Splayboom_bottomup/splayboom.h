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
    if (knoop->ouder)
    {
        if (knoop->ouder->ouder == nullptr)
        {
            //Zig
            Zoekboom<Sleutel, Data>::roteer(knoop->sleutel);
        }
        else if (knoop->ouder->rechts && knoop->ouder->rechts.get() == knoop)
        {
            //Zig zag
            Zoekboom<Sleutel, Data>::roteer(knoop->sleutel);
            Zoekboom<Sleutel, Data>::roteer(knoop->sleutel);
            if (knoop->ouder)
                this->splay(knoop);
        }
        else
        {
            //Zig zig
            Zoekboom<Sleutel, Data>::roteer(knoop->ouder->sleutel);
            Zoekboom<Sleutel, Data>::roteer(knoop->sleutel);
            if (knoop->ouder)
                this->splay(knoop);
        }
    }
}

template <class Sleutel, class Data>
zoekKnoop<Sleutel, Data> &Splayboom<Sleutel, Data>::zoek(const Sleutel &sleutel)
{
    zoekKnoop<Sleutel, Data> *ouder;
    Zoekboom<Sleutel, Data> *plaats;
    Zoekboom<Sleutel, Data>::zoek(sleutel, ouder, plaats);
    if (plaats)
    {
        this->splay(plaats->get());
    }
    return **plaats;
}

template <class Sleutel, class Data>
void Splayboom<Sleutel, Data>::voegtoe(const Sleutel &sleutel, const Data &data, bool dubbelstoestaan)
{
    zoekKnoop<Sleutel, Data> *knoop = Zoekboom<Sleutel, Data>::voegtoe(sleutel, data, dubbelstoestaan);
    if (knoop)
    {
        this->splay(knoop);
    }
}

template <class Sleutel, class Data>
void Splayboom<Sleutel, Data>::verwijder(const Sleutel &sleutel)
{
    zoekKnoop<Sleutel, Data> *ouder;
    Zoekboom<Sleutel, Data>::verwijder(sleutel, ouder);
    if (ouder)
    {
        this->splay(ouder);
    }
}

#endif