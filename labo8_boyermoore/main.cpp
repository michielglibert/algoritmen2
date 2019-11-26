#include "boyermoore.h"
#include <iostream>
#include <string>
#include <fstream>

using std::cout;
using std::endl;
using std::ifstream;

int main()
{
    BoyerMoore bm("cbad");
    ifstream hooiberg{"test.txt"};
    string lijn;
    while (getline(hooiberg, lijn))
    {
        long int x = 0;
        queue<int> offsets = bm.zoek(lijn, x);
        if (offsets.front())
        {
            cout << offsets.front() << endl;
        }
        else
        {
            cout << "Done" << endl;
        }
    }

    cout << "Finished!" << endl;
    return 0;
}