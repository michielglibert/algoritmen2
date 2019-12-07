#include <iostream>
#include <vector>
#include "johnson.h"
#include "johnson2.h"
#include "graaf.h"

using namespace std;

int main()
{
    int startknoop = 0;
    GraafMetTakdata<GERICHT, int> graaf(6);
    graaf.voegVerbindingToe(0, 1, 10);
    graaf.voegVerbindingToe(0, 5, 8);
    graaf.voegVerbindingToe(1, 3, 2);
    graaf.voegVerbindingToe(2, 1, 1);
    graaf.voegVerbindingToe(3, 2, -2);
    graaf.voegVerbindingToe(4, 1, -4);
    graaf.voegVerbindingToe(4, 3, -1);
    graaf.voegVerbindingToe(5, 4, 1);
    graaf.teken("originele_graaf");

    vector<vector<int>> kortstePaden = johnson(graaf);
    cout << "Dit zijn de kortste paden met johnson voor alle knopen:" << endl;
    for (int i = 0; i < kortstePaden.size(); i++)
    {
        cout << "------ Voor knoop " << i << endl;
        for (int j = 0; j < kortstePaden[i].size(); j++)
        {
            cout << "Van " << i << " naar " << j << " is de waarde " << kortstePaden[i][j] << endl;
        }
    }

    // vector<vector<int>> kortstePaden2 = johnson_twee(graaf);
    // cout << "Dit zijn de kortste paden met johnson2 voor alle knopen:" << endl;
    // for (int i = 0; i < kortstePaden2.size(); i++)
    // {
    //     cout << "------ Voor knoop " << i << endl;
    //     for (int j = 0; j < kortstePaden2[i].size(); j++)
    //     {
    //         cout << "Van " << i << " naar " << j << " is de waarde " << kortstePaden[i][j] << endl;
    //     }
    // }

    return 0;
}