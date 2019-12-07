#include <iostream>
#include <vector>
#include "graaf.h"
#include "transitievesluiting.h"

using namespace std;

int main()
{
    Graaf<GERICHT> graaf(5);
    graaf.voegVerbindingToe(0, 1);
    graaf.voegVerbindingToe(1, 2);
    graaf.voegVerbindingToe(2, 3);
    graaf.voegVerbindingToe(2, 4);
    graaf.teken("graaf");

    Graaf<GERICHT> ts1_graaf = ts1(graaf);
    ts1_graaf.teken("graaf1");

    Graaf<GERICHT> ts2_graaf = ts2(graaf);
    ts2_graaf.teken("graaf2");

    Graaf<GERICHT> ts3_graaf = ts3(graaf);
    ts3_graaf.teken("graaf3");

    cout << "Finished!" << endl;

    return 0;
}