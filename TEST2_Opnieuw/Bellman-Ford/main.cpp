#include <iostream>
#include <vector>
#include "bellmanford.hpp"
#include "graaf.h"

using namespace std;

int main()
{
    int startknoop = 5;
    GraafMetTakdata<GERICHT, int> graaf(6);

    // graaf.voegVerbindingToe(0, 1, 10);
    // graaf.voegVerbindingToe(0, 5, 8);
    // graaf.voegVerbindingToe(1, 3, 2);
    // graaf.voegVerbindingToe(2, 1, 1);
    // graaf.voegVerbindingToe(3, 2, 2);
    // graaf.voegVerbindingToe(4, 1, 4);
    // graaf.voegVerbindingToe(3, 4, 1);
    // graaf.voegVerbindingToe(5, 4, 1);
    graaf.voegVerbindingToe(0, 1, 10);
    graaf.voegVerbindingToe(0, 5, 8);
    graaf.voegVerbindingToe(1, 3, 2);
    graaf.voegVerbindingToe(2, 1, 1);
    graaf.voegVerbindingToe(3, 2, -2);
    graaf.voegVerbindingToe(4, 1, -4);
    graaf.voegVerbindingToe(4, 3, -1);
    graaf.voegVerbindingToe(5, 4, 1);
    graaf.teken("graaf");

    vector<int> kortstePaden = bellman_verbeterd(graaf, startknoop);
    cout << "Dit zijn de kortste paden met bellman:" << endl;
    for (int i = 0; i < kortstePaden.size(); i++)
    {
        cout << "Van " << startknoop << " naar " << i << " is de waarde " << kortstePaden[i] << endl;
    }

    // vector<int> kortstePaden2 = bellman_deque(graaf, startknoop);
    // cout << "Dit zijn de kortste paden met dijkstra:" << endl;
    // for (int i = 0; i < kortstePaden2.size(); i++)
    // {
    //     cout << "Van " << startknoop << " naar " << i << " is de waarde " << kortstePaden2[i] << endl;
    // }

    return 0;
}