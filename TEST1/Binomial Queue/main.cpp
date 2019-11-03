#include <iostream>
#include <string>
#include "binomialheap.h"

using namespace std;

int main()
{
    BinomialHeap<string> heap;
    heap.voegToe(5, "data");
    heap.voegToe(2, "data");
    heap.voegToe(2, "data");
    heap.voegToe(10, "data");
    heap.voegToe(25, "data");
    heap.voegToe(1, "data");
    heap.voegToe(3, "data");
    heap.voegToe(40, "data");
    heap.voegToe(24, "data");
    heap.voegToe(20, "data");
    cout << heap.popMinimum() << endl;
    heap.teken("test");

    cout << "Finished" << endl;
    return 0;
}