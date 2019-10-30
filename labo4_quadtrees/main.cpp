#include <iostream>
#define _USE_MATH_DEFINES
#include "math.h"
#include "PRQuadtree.h"

using namespace std;

int main()
{
    PRQuadtree tree((10000 - 5) * 131072);
    // tree.voegToe(55, 55);
    // tree.voegToe(1, 9);
    // tree.voegToµe(51, 26);
    // tree.voegToe(76, 77);
    // tree.teken("result");
    // tree.voegToe(27, 11);
    // tree.voegToe(1, 70);
    // tree.voegToe(2, 70);
    // tree.voegToe(3, 70);
    // tree.voegToe(2, 1);
    // tree.voegToe(3, 1);
    // tree.voegToe(3, 1);
    // tree.voegToe(24, 11);
    // tree.voegToe(27, 9);
    // tree.voegToe(75, 75);

    for (int i = 0; i < 131072; i++)
    {
        tree.voegToe((10000 - 5 * i) * cos(i * 0.25 * M_PI), (10000 - 5 * i) * sin(i * 0.25 * M_PI));
    }

    // cout << tree.totaal << endl;

    //tree.schrijf();
    //tree.teken("test");

    cout << "Diepte: " << tree.geefDiepte() << endl;
    cout << "Finished" << endl;
    return 0;
}