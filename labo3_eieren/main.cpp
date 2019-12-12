#include <iostream>
#include <vector>
#include "zoekboom1.h"

using namespace std;

void print_matrix(vector<vector<int>> matrix)
{
    cout << "\t";
    for (int i = 0; i < matrix[0].size(); i++)
    {
        cout << i << "\t";
    }

    cout << endl;
    for (int i = 0; i < matrix.size(); i++)
    {
        cout << i << "\t";
        for (int j = 0; j < matrix[i].size(); j++)
        {
            cout << matrix[i][j] << "\t";
        }
        cout << endl;
    }
}

/**
 * Berekent op basis van aantal eieren en verdiepingen aantal pogingen
 * @param e aantal eieren
 * @param v aantal verdiepingen
 * @return aantal pogingen
 */

// Floyd-Warshall toepassen
int goedkoopste_strategie(int e, int v)
{

    // 2d vector initializer met size v
    vector<vector<int>> matrix(v);

    for (int i = 0; i < v; i++)
    {
        // 1d vector initializeren met size e
        vector<int> rij(e);
        matrix[i] = rij;
    }

    // triviale gevallen: 0 of 1 verdieping
    for (int j = 0; j < e; j++)
    {
        // 0 verdiepingen: 0 keer laten vallen
        matrix[0][j] = 0;
        // 1 verdieping: 1 keer laten vallen
        matrix[1][j] = 1;
    }

    // triviaal geval: 1 ei
    for (int i = 1; i < v; i++)
    {
        // aantal keer laten vallen: i keer
        // i = aantal verdiepingen, onderaan bij laagste verdieping beginnen
        matrix[i][0] = i;
    }

    // Andere gevallen
    // Triviale geval voor 1 verdieping hebben we hiervoor al behandeld
    for (int i = 2; i < v; i++)
    {
        // Triviale gevallen voor 0 en 1 eieren hebben we hiervoor al behandeld
        for (int j = 1; j < e; j++)
        {
            // Alles initializeren op oneindig
            matrix[i][j] = INT_MAX;
            for (int x = 1; x < i; x++)
            {
                // resultaten uit tabel halen: maximum + 1
                int pogingen = 1 + max(matrix[x - 1][j - 1], matrix[i - x][j]);
                if (pogingen < matrix[i][j])
                {
                    matrix[i][j] = pogingen;
                    if (j == 2)
                        std::cout << "Eieren " << j << " - Verdiep " << i << ": " << x << std::endl;
                }
            }
        }
    }
    print_matrix(matrix);

    return matrix[v - 1][e - 1];
}

void bepaalStrategieRec(Zoekboom<int, int> &boom, int e, int v, int totaalverdiepen)
{
    //Breekt
    if (e > 0 && v > 0)
    {
        bepaalStrategieRec(boom, e - 1, v - 1, totaalverdiepen);
        boom.voegtoe(e, v - 1, false);
    }

    //Niet breekt
    if (v <= totaalverdiepen)
    {
        bepaalStrategieRec(boom, e, v + 1, totaalverdiepen);
        boom.voegtoe(e, v, false);
    }
    std::cerr << "test";
}

void bepaalStrategie(int e, int v)
{
    Zoekboom<int, int> boom;
    while (e > 0)
    {
        bepaalStrategieRec(boom, e - 1, v / 2, v);
    }
    std::cerr << "lol";
    boom.teken("strategie");
}

int main()
{
    int e = 20;
    int v = 36;

    int aantal_pogingen = goedkoopste_strategie(e, v);
    cout << "Aantal pogingen: " << aantal_pogingen << endl;
    // bepaalStrategie(e, v);
}