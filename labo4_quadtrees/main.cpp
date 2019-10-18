#include <iostream>
#include "PRQuadtree.h"

using namespace std;

int main()
{
    PRQuadtree tree(100);
    tree.voegToe(1, 10);
    tree.voegToe(1, 9);
    tree.voegToe(51, 26);
    tree.voegToe(76, 77);
    tree.voegToe(27, 11);
    tree.voegToe(1, 70);
    tree.voegToe(2, 70);
    tree.voegToe(3, 70);
    tree.voegToe(2, 1);
    tree.voegToe(3, 1);
    tree.voegToe(3, 1);
    tree.voegToe(24, 11);
    tree.voegToe(27, 9);
    tree.voegToe(75, 75);
    tree.schrijf();
    tree.teken("test");

    cout << "Diepte: " << tree.geefDiepte() << endl;
    cout << "Finished" << endl;
    return 0;
}