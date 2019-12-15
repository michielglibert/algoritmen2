#include "stroomnet.h"
using std::cout;
using std::endl;
int main()
{
    //Cap 14 en 20
    GraafMetTakdata<GERICHT, int> gr(8);
    gr.voegVerbindingToe(1, 2, 7);
    gr.voegVerbindingToe(7, 1, 2);
    gr.voegVerbindingToe(7, 3, 6);
    gr.voegVerbindingToe(3, 4, 7);
    gr.voegVerbindingToe(4, 5, 1);
    gr.voegVerbindingToe(2, 5, 4);
    gr.voegVerbindingToe(1, 0, 10);
    gr.voegVerbindingToe(0, 2, 3);
    gr.voegVerbindingToe(0, 7, 5);
    gr.voegVerbindingToe(0, 6, 6);
    gr.voegVerbindingToe(5, 6, 6);
    gr.voegVerbindingToe(6, 3, 2);
    gr.voegVerbindingToe(6, 4, 8);
    // GraafMetTakdata<GERICHT, int> gr(6);
    // gr.voegVerbindingToe(0, 1, 10);
    // gr.voegVerbindingToe(0, 2, 10);
    // gr.voegVerbindingToe(1, 3, 25);
    // gr.voegVerbindingToe(2, 4, 15);
    // gr.voegVerbindingToe(3, 5, 10);
    // gr.voegVerbindingToe(4, 1, 6);
    // gr.voegVerbindingToe(4, 5, 10);
    gr.teken("gr.dot");

    Pad<int> pad;
    Stroomnetwerk<int> sn(gr, 0, 5);

    cout << sn << endl;
    sn.teken("gr.dot");
    assert(sn.geefStroom().geefCapaciteit() == 14);
    cout << "DONE" << endl;
}
