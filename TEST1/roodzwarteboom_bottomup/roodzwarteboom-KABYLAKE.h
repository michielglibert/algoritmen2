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
    void verwijder(Sleutel);
    void verwijder_rec(Sleutel sleutel);
    bool geefKleur(zoekKnoop<Sleutel, pair<Data, bool>> *);
    bool geefKleur(Zoekboom<Sleutel, pair<Data, bool>> *);
};

template <class Sleutel, class Data>
void Roodzwarteboom<Sleutel, Data>::herkleur(Sleutel sleutel)
{
    zoekKnoop<Sleutel, pair<Data, bool>> *ouder;
    Zoekboom<Sleutel, pair<Data, bool>> *knoop;
    Zoekboom<Sleutel, pair<Data, bool>>::zoek(sleutel, ouder, knoop);

    //Als ouder bestaat en rood is, dan is de RZ voorwaarde niet meer voldaan
    if (ouder != nullptr && geefKleur(ouder) && geefKleur(knoop))
    {
        //Als ouder rood is bestaat zeker grootouder
        //Als broer bestaat
        if (ouder->ouder->rechts != nullptr && ouder->ouder->sleutel > sleutel)
        {
            //Als broer van ouder rood is -> Kleuren omdraaien
            if (geefKleur(&(ouder->ouder->rechts)))
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
            if (geefKleur(&(ouder->ouder->links)))
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
void Roodzwarteboom<Sleutel, Data>::verwijder(Sleutel sleutel)
{
    zoekKnoop<Sleutel, pair<Data, bool>> *ouder;
    Zoekboom<Sleutel, pair<Data, bool>> *knoop;
    Zoekboom<Sleutel, pair<Data, bool>>::zoek(sleutel, ouder, knoop);

    if (!geefKleur(knoop) && !geefKleur(&((*knoop)->links)) && !geefKleur(&((*knoop)->rechts)))
    {
        if (sleutel == 4)
        {
            cerr << "kom" << endl;
        }
        //2 kinderen
        cerr << "Doing recursion" << endl;
        // if (ouder != nullptr)
        // {
        //     verwijder_rec(ouder->sleutel);
        // }
        // else
        // {
        verwijder_rec(sleutel);
        // }
    }
    else if (!geefKleur(knoop) && (*knoop)->links != nullptr)
    {
        //1 linkerkind
        (*knoop)->links->data.second = false;
    }
    else if (!geefKleur(knoop) && (*knoop)->rechts != nullptr)
    {
        //1 rehcterkind
        (*knoop)->rechts->data.second = false;
    }
    //Rode knoop of geen kinderen
    if (ouder->ouder == nullptr && (*knoop)->rechts)
    {
        Zoekboom<Sleutel, pair<Data, bool>> *nieuwe = &((*knoop)->rechts);
        while ((*nieuwe)->links != nullptr)
        {
            nieuwe = &((*nieuwe)->links);
        }
        (*nieuwe)->data.second = false;
    }
    Zoekboom<Sleutel, pair<Data, bool>>::verwijder(sleutel);
}

template <class Sleutel, class Data>

void Roodzwarteboom<Sleutel, Data>::verwijder_rec(Sleutel sleutel)
{ //Indien niet wortel
    zoekKnoop<Sleutel, pair<Data, bool>> *ouder;
    Zoekboom<Sleutel, pair<Data, bool>> *knoop;
    Zoekboom<Sleutel, pair<Data, bool>>::zoek(sleutel, ouder, knoop);
    if (ouder && ouder != nullptr)
    {
        if (sleutel == 4)
            cerr << "In recursion" << endl;
        if (ouder->sleutel > sleutel && ouder->rechts && geefKleur(&(ouder->rechts)))
        {
            cerr << "Rode broer" << endl;
            //Rode broer
            Zoekboom<Sleutel, pair<Data, bool>>::roteer(ouder->rechts->sleutel);
            verwijder_rec(sleutel);
        }
        else if (ouder->sleutel < sleutel && ouder->links && geefKleur(&(ouder->links)))
        {
            cerr << "Rode broer spiegelbeeld" << endl;
            //Rode broer spiegelbeeld
            Zoekboom<Sleutel, pair<Data, bool>>::roteer(ouder->links->sleutel);
            verwijder_rec(sleutel);
        }
        else if ((ouder->sleutel > sleutel && ouder->rechts) && !geefKleur(&(ouder->rechts->links)) && !geefKleur(&(ouder->rechts->rechts)))
        {
            cerr << "2 zwarte kinderen" << endl;
            //2 zwarte kinderen
            ouder->data.second = false;
            ouder->rechts->data.second = true;
            verwijder_rec(ouder->sleutel);
        }
        else if ((ouder->sleutel < sleutel && ouder->links) && !geefKleur(&(ouder->links->links)) && !geefKleur(&(ouder->links->rechts)))
        {
            cerr << "2 zwarte kinderen spiegelbeeld" << endl;
            //2 zwarte kinderen spiegelbeeld
            ouder->data.second = false;
            ouder->links->data.second = true;
            verwijder_rec(ouder->sleutel);
        }
        else if (ouder->sleutel > sleutel && geefKleur(&(ouder->rechts->rechts)))
        {
            cerr << "1 rood rechterkind" << endl;
            //1 rood rechterkind
            Zoekboom<Sleutel, pair<Data, bool>>::roteer(ouder->rechts->sleutel);
            ouder->rechts->data.second = ouder->data.second;
            ouder->ouder->rechts->data.second = false;
            ouder->data.second = false;
        }
        else if (ouder->sleutel < sleutel && geefKleur(&(ouder->rechts->links)))
        {
            cerr << "1 rood rechterkind spiegelbeeld" << endl;
            //1 rood rechterkind spiegelbeeld
            Zoekboom<Sleutel, pair<Data, bool>>::roteer(ouder->links->sleutel);
            ouder->links->data.second = ouder->data.second;
            ouder->links->links->data.second = false;
            ouder->data.second = false;
        }
        else if (ouder->sleutel > sleutel && ouder->rechts)
        {
            cerr << "Zwart rechterkind en rood linkerkind" << endl;
            //Zwart rechterkind en rood linkerkind
            Zoekboom<Sleutel, pair<Data, bool>>::roteer(ouder->rechts->links->sleutel);
            verwijder_rec(sleutel);
        }
        else
        {
            cerr << "Zwart rechterkind en rood linkerkind spiegelbeeld" << endl;
            //Zwart recherkind en rood linkerkind spiegelbeeld
            Zoekboom<Sleutel, pair<Data, bool>>::roteer(ouder->links->rechts->sleutel);
            verwijder_rec(sleutel);
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
bool Roodzwarteboom<Sleutel, Data>::geefKleur(zoekKnoop<Sleutel, pair<Data, bool>> *knoop)
{
    if (knoop == nullptr)
    {
        return false;
    }
    return knoop->data.second;
}

template <class Sleutel, class Data>
bool Roodzwarteboom<Sleutel, Data>::geefKleur(Zoekboom<Sleutel, pair<Data, bool>> *knoop)
{
    if (*knoop == nullptr)
    {
        return false;
    }
    return (*knoop)->data.second;
}

#endif