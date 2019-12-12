#include <queue>
#include <iostream>
#include "bitpatroon.h"

using std::cerr;
using std::endl;

typedef unsigned int uint;
typedef unsigned char uchar;

class ShiftAnd
{
public:
    //naaldlengte moet kleiner dan patroonlengte zijn,
    //anders wordt een exceptie opgegooid.
    ShiftAnd(const uchar *naald, uint _naaldlengte);
    void zoek(std::queue<int> &plaats, const uchar *hooiberg, uint hooiberglengte);
    //Elke fout mag gemaakt worden bij deze methode (max 1)
    void zoekMetFout(std::queue<int> &plaats, const uchar *hooiberg, uint hooiberglengte);
    void zoekMetInlassing(std::queue<int> &plaats, const uchar *hooiberg, uint hooiberglengte);
    void zoekMetVerwijdering(std::queue<int> &plaats, const uchar *hooiberg, uint hooiberglengte);
    void zoekMetVervanging(std::queue<int> &plaats, const uchar *hooiberg, uint hooiberglengte);

private:
    Bitpatroon letterpatroon[UCHAR_MAX];
    uint naaldlengte;
};

ShiftAnd::ShiftAnd(const uchar *naald, uint _naaldlengte) : naaldlengte(_naaldlengte)
{
    //Patroonlengte uit klasse bitpatroon
    if (patroonlengte < _naaldlengte)
    {
        throw "De naald lengte is te groot";
    }

    for (int i = 0; i < _naaldlengte; i++)
    {
        letterpatroon[naald[i]] |= Bitpatroon::eenbit(i);
    }
}

void ShiftAnd::zoek(std::queue<int> &plaats, const uchar *hooiberg, uint hooiberglengte)
{
    Bitpatroon d;

    for (int i = 0; i < hooiberglengte; i++)
    {

        cerr << i << endl;
        d = d.shiftrechts(1);
        d |= Bitpatroon::eenbit(0);
        d &= letterpatroon[hooiberg[i]];

        if (d.en(Bitpatroon::eenbit(naaldlengte - 1)))
        {
            plaats.push(1);
            d &= Bitpatroon();
        }
    }
}

void ShiftAnd::zoekMetVervanging(std::queue<int> &plaats, const uchar *hooiberg, uint hooiberglengte)
{
    Bitpatroon d;
    Bitpatroon d1;

    for (int i = 0; i < hooiberglengte; i++)
    {
        //Enkel in deze functie stap per stap in commentaar
        ////////(Schuif(R1) && S[T[j]])
        //Schuif(R1)
        d1 = d1.shiftrechts(1);
        d1 |= Bitpatroon::eenbit(0);
        // && S[T[j]]
        d1 &= letterpatroon[hooiberg[i]];

        ////////R1 = Schuif(R0) | (Schuif(R1) && S[T[j]])
        //Schuif(R0)
        d = d.shiftrechts(1);
        d |= Bitpatroon::eenbit(0);
        //R1 =  Schuif(R0) | (Schuif(R1) && S[T[j]])
        d1 |= d;

        //(Schuif(R0) && S[T[j]])
        d &= letterpatroon[hooiberg[i]];

        if (d1.en(Bitpatroon::eenbit(naaldlengte - 1)))
        {
            plaats.push(1);
            d &= Bitpatroon();
        }
    }
}

void ShiftAnd::zoekMetInlassing(std::queue<int> &plaats, const uchar *hooiberg, uint hooiberglengte)
{
    Bitpatroon d;
    Bitpatroon d1;

    for (int i = 0; i < hooiberglengte; i++)
    {
        //R1 = R0 | (Shift(R1 && S[T[j]]))

        //(Shift(R1) && S[T[j]]))
        d1 = d1.shiftrechts(1);
        d1 |= Bitpatroon::eenbit(0);
        d1 &= letterpatroon[hooiberg[i]];

        //R0 | (Shift(R1 && S[T[j]]))

        d1 |= d;

        //(Schuif(R0) && S[T[j]])
        d = d.shiftrechts(1);
        d |= Bitpatroon::eenbit(0);
        d &= letterpatroon[hooiberg[i]];

        if (d1.en(Bitpatroon::eenbit(naaldlengte - 1)))
        {
            plaats.push(1);
            d &= Bitpatroon();
        }
    }
}

void ShiftAnd::zoekMetVerwijdering(std::queue<int> &plaats, const uchar *hooiberg, uint hooiberglengte)
{
    Bitpatroon d;
    Bitpatroon d1;
    Bitpatroon d_tijdelijk;

    for (int i = 0; i < hooiberglengte; i++)
    {
        //R1 = shift(R0_j+1) | (shift(R1) && S[T[j]])

        //(shift(R1) && S[T[j]])
        d1 = d1.shiftrechts(1);
        d1 |= Bitpatroon::eenbit(0);
        d1 &= letterpatroon[hooiberg[i]];

        //R0_j+1 = (Schuif(R0) && S[T[j]])
        d = d.shiftrechts(1);
        d |= Bitpatroon::eenbit(0);
        d &= letterpatroon[hooiberg[i]];

        //schuif(R0_j+1)
        d_tijdelijk = d.shiftrechts(1);
        d_tijdelijk |= Bitpatroon::eenbit(0);

        //shift(R0_j+1) | (shift(R1) && S[T[j]])
        d1 |= d_tijdelijk;

        if (d1.en(Bitpatroon::eenbit(naaldlengte - 1)))
        {
            plaats.push(1);
            d &= Bitpatroon();
        }
    }
}

void ShiftAnd::zoekMetFout(std::queue<int> &plaats, const uchar *hooiberg, uint hooiberglengte)
{
    Bitpatroon d;
    Bitpatroon d1;
    Bitpatroon d1_tijdelijk;

    for (int i = 0; i < hooiberglengte; i++)
    {
        //(shift(R1) && S[T[j]])
        d1 = d1.shiftrechts(1);
        d1 |= Bitpatroon::eenbit(0);
        d1 &= letterpatroon[hooiberg[i]];

        //R0 | ...
        d1 |= d;

        //shift(R0) | ...
        d = d.shiftrechts(1);
        d |= Bitpatroon::eenbit(0);
        d1 |= d;

        //shift(R0_j+1) | ...
        d &= letterpatroon[hooiberg[i]];
        d1_tijdelijk = d.shiftrechts(1);
        d1_tijdelijk |= Bitpatroon::eenbit(0);

        d1 |= d1_tijdelijk;

        if (d1.en(Bitpatroon::eenbit(naaldlengte - 1)))
        {
            plaats.push(1);
            d &= Bitpatroon();
        }
    }
}
