#include <iostream>
#include <queue>
#include "karprabin.hpp"

using std::cout;
using std::endl;
using std::queue;

int main()
{
    KarpRabin kr("pxpypxpypxpx", UCHAR_MAX);
    queue<int> plaatsen = kr.zoek("pxpypxpypxpxpypxpypxpx");

    int i = 0;
    while (!plaatsen.empty())
    {
        plaatsen.pop();
        i++;
    }

    cout << "De string werd " << i << " keer gevonden." << endl;
    return 0;
}