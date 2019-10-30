#ifndef __ROODZWARTEBOOM_H
#define __ROODZWARTEBOOM_H

#include <iostream>
#include "zoekboom17.h"

using namespace std;

//We gebruiken een bool om aan te geven of hij rood of zwart is
//0 = zwart, 1 = rood
template <class Sleutel, class Data>
class Roodzwarteboom : public Zoekboom<Sleutel, pair<Data, bool>>
{
public:
    void voegtoe(Sleutel, Data);
    void herkleur(Sleutel);
    void verwijder(Sleutel, Data);
    bool geefKleur(zoekKnoop<Sleutel, pair<Data, bool>> &);
};

template <class Sleutel, class Data>
void Roodzwarteboom<Sleutel, Data>::herkleur(Sleutel sleutel)
{
    zoekKnoop<Sleutel, pair<Data, bool>> *ouder;
    Zoekboom<Sleutel, pair<Data, bool>> *knoop;
    Zoekboom<Sleutel, pair<Data, bool>>::zoek(sleutel, ouder, knoop);

    //Als ouder bestaat en rood is, dan is de RZ voorwaarde niet meer voldaan
    if (ouder != nullptr && geefKleur(*ouder) && geefKleur(**knoop))
    {
        //Als ouder rood is bestaat zeker grootouder
        //Als broer bestaat
        if (ouder->ouder->rechts != nullptr && ouder->ouder->sleutel > sleutel)
        {
            if (sleutel == -8)
            {
                cerr << "lol" << endl;
            }
            //Als broer van ouder rood is -> Kleuren omdraaien
            if (geefKleur(*(ouder->ouder->rechts)))
            {
                ouder->data.second = false;

                //Enkel indien geen wortel is, zwart maken

                if (ouder->ouder->ouder == nullptr)
                {

                    ouder->ouder->data.second = false;
                }
                else
                {
                    ouder->ouder->data.second = true;
                }

                (*(ouder->ouder->rechts)).data.second = false;
                if (ouder->ouder != nullptr)
                    this->herkleur(ouder->ouder->sleutel);
            }
            else
            {
                //Als broer zwart is en c is rechterkind
                if (sleutel > ouder->sleutel)
                {
                    Zoekboom<Sleutel, pair<Data, bool>>::roteer(sleutel);
                    this->herkleur(ouder->sleutel);
                }
                else
                {
                    //Als broer zwart is en c is linkerkind
                    Zoekboom<Sleutel, pair<Data, bool>>::roteer(ouder->sleutel);
                    ouder->data.second = false;
                    (*(ouder->rechts)).data.second = true;
                }
            }
        }
        else if (ouder->ouder != nullptr && ouder->ouder->sleutel > sleutel)
        {
            //Als broer zwart is en c is rechterkind
            if (sleutel > ouder->sleutel)
            {
                Zoekboom<Sleutel, pair<Data, bool>>::roteer(sleutel);
                this->herkleur(ouder->sleutel);
            }
            else
            {
                //Als broer zwart is en c is linkerkind
                Zoekboom<Sleutel, pair<Data, bool>>::roteer(ouder->sleutel);
                ouder->data.second = false;
                (*(ouder->rechts)).data.second = true;
            }
        }
        else if (ouder->ouder->links != nullptr && ouder->ouder->sleutel < sleutel)
        {
            //Als broer van ouder rood is -> Kleuren omdraaien
            if (geefKleur(*(ouder->ouder->links)))
            {
                ouder->data.second = false;

                //Enkel indien geen wortel is, zwart maken

                if (ouder->ouder->ouder == nullptr)
                {

                    ouder->ouder->data.second = false;
                }
                else
                {
                    ouder->ouder->data.second = true;
                }

                (*(ouder->ouder->links)).data.second = false;
                if (ouder->ouder != nullptr)
                    this->herkleur(ouder->ouder->sleutel);
            }
            else
            {
                //Als broer zwart is en c is rechterkind
                if (sleutel < ouder->sleutel)
                {
                    Zoekboom<Sleutel, pair<Data, bool>>::roteer(sleutel);
                    this->herkleur(ouder->sleutel);
                }
                else
                {
                    //Als broer zwart is en c is linkerkind
                    Zoekboom<Sleutel, pair<Data, bool>>::roteer(ouder->sleutel);
                    ouder->data.second = false;
                    (*(ouder->links)).data.second = true;
                }
            }
        }
        else
        {
            //Als broer zwart is en c is rechterkind
            if (sleutel < ouder->sleutel)
            {
                Zoekboom<Sleutel, pair<Data, bool>>::roteer(sleutel);
                this->herkleur(ouder->sleutel);
            }
            else
            {
                //Als broer zwart is en c is linkerkind
                Zoekboom<Sleutel, pair<Data, bool>>::roteer(ouder->sleutel);
                ouder->data.second = false;
                (*(ouder->links)).data.second = true;
            }
        }
    }
}

template <class Sleutel, class Data>
void Roodzwarteboom<Sleutel, Data>::voegtoe(Sleutel sleutel, Data data)
{
    pair<Data, bool> knoopdata(data, true);
    if (Zoekboom<Sleutel, pair<Data, bool>>::geefDiepte() == 0)
    {
        knoopdata.second = false;
    }
    Zoekboom<Sleutel, pair<Data, bool>>::voegtoe(sleutel, knoopdata, false);
    this->herkleur(sleutel);
}

template <class Sleutel, class Data>
bool Roodzwarteboom<Sleutel, Data>::geefKleur(zoekKnoop<Sleutel, pair<Data, bool>> &knoop)
{
    return knoop.data.second;
}

#endif