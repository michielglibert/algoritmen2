#include <iostream>
#include <queue>
#include "shiftand.h"

using std::cout;
using std::endl;
using std::queue;
using std::string;

//Letop!!!!
//Met fouten bedoelt men fouten in de tekst, de hooiberg.
//Niet fouten in het patroon zelf!! Dat is altijd juist
//Maar misschien wil je een woord zoeken dat er op lijkt (then en than bvb.)
int main()
{
    string naald = "announce";
    string hooiberg = "annual_andounce";
    queue<int> plaatsen;
    ShiftAnd sa((uchar *)naald.c_str(), naald.size());
    sa.zoekMetFout(plaatsen, (uchar *)hooiberg.c_str(), hooiberg.size());

    int i = 0;
    while (!plaatsen.empty())
    {
        plaatsen.pop();
        i++;
    }

    cout << "De naald komt " << i << " keer voor." << endl;

    return 0;
}
