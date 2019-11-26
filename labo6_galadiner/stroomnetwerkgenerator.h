#include <cstdlib>
#include <algorithm>
#include <cmath>
#include "graaf.h"
#include <memory>
//functie om random vlottendekommagetallen te geven.
double frand(double maxf)
{
    return (random() % 1000000000) * (maxf / 1000000000.0);
}

class Stroomnetwerkgenerator
{
public:
    virtual unique_ptr<GraafMetTakdata<GERICHT, int>> make_uniqueStroomnet(int aantalKnopen, double takkans, int maxGewicht)
    {
        unique_ptr<GraafMetTakdata<GERICHT, int>> nieuweGraaf =
            make_unique<GraafMetTakdata<GERICHT, int>>(aantalKnopen);
        for (int i = 0; i < aantalKnopen; i++)
        {
            for (int j = 0; j < aantalKnopen; j++)
            {
                if (j != i && frand(1) <= takkans)
                    nieuweGraaf->voegVerbindingToe(i, j, rand() % maxGewicht);
            }
        }
        return nieuweGraaf;
    }
};