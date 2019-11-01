#include <iostream>
#include "DNAsequentie.h"

using namespace std;

int main()
{
    DNAsequentie dna("louis");
    int lengte = dna.d("aloysius");
    cout << "Lengte: " << lengte << endl;
    return 0;
}