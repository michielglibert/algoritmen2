#include <iostream>
#include <cassert>
#include <string>
#include "zoekboom17.h"
#include "splayboom.h"

using namespace std;

int main()
{
    Splayboom<int, string> boom;
    for (int i = 0; i < 100; i++)
    {
        boom.voegtoe(i, "data");
    }
    boom.zoek(0);
    boom.zoek(2);
    boom.zoek(8);
    boom.zoek(50);
    boom.teken("Splayboom");
    cout << "Finished" << endl;
    return 0;
}