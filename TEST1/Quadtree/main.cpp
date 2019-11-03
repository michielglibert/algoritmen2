#include <iostream>
#include "prquadtree.h"

using namespace std;

int main()
{
    PRQuadtree boom(100);
    boom.voegToe(5, 5);
    boom.voegToe(-50, -50);
    boom.voegToe(51, 51);
    boom.voegToe(24, 16);
    boom.voegToe(88, 99);
    boom.voegToe(102, 103);
    boom.voegToe(-47, -41);
    boom.voegToe(-47, -46);
    boom.voegToe(-99, -99);
    boom.voegToe(-70, 80);
    boom.voegToe(10, -50);
    boom.voegToe(51, 51);
    boom.voegToe(80, 89);
    boom.voegToe(0, 0);
    boom.voegToe(-100, -100);
    boom.voegToe(98, 98);
    boom.voegToe(100, 100);
    boom.voegToe(1, 1);
    boom.voegToe(2, 2);
    // boom.voegToe(7, 51);
    boom.teken("test");
    cout << boom.get() << endl;
    return 0;
}