#include <iostream>
#include <vector>
#include "johnson.hpp"
#include "graaf.h"

using namespace std;

int main()
{
    GraafMetTakdata<GERICHT, int> graaf(6);
    graaf.voegVerbindingToe(0, 1, 10);
    graaf.voegVerbindingToe(0, 5, 8);
    graaf.voegVerbindingToe(1, 3, 2);
    graaf.voegVerbindingToe(2, 1, 1);
    graaf.voegVerbindingToe(3, 2, -2);
    graaf.voegVerbindingToe(4, 1, -4);
    graaf.voegVerbindingToe(4, 3, -1);
    graaf.voegVerbindingToe(5, 4, 1);
    graaf.teken("graaf");

    vector<vector<int>> kortstePaden = johnson(graaf);
    cout << "Dit zijn de kortste paden met johnson voor alle knopen:" << endl;
    for (int i = 0; i < kortstePaden.size(); i++)
    {
        cout << "------ Voor knoop ------ " << i << endl;
        for (int j = 0; j < kortstePaden[i].size(); j++)
        {
            cout << "Van " << i << " naar " << j << " is de waarde " << kortstePaden[i][j] << endl;
        }
        cout << "BELLMAN CONTROLE:" << i << endl;
        vector<int> kortstePaden2 = bellman_deque(graaf, i);
        for (int j = 0; j < kortstePaden2.size(); j++)
        {
            cout << "Van " << i << " naar " << j << " is de waarde " << kortstePaden2[j] << endl;
        }
    }
}