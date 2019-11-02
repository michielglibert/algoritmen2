#include <iostream>
#include <fstream>
#include <vector>
#include "DNAsequentie.h"

using namespace std;

int main()
{
    //Vectors aanmaken
    vector<DNAsequentie> human_dna;
    vector<DNAsequentie> chimp_dna;

    //Bestanden inlezen
    string streng;

    ifstream human_if{"human.dna"};
    while (human_if >> streng)
    {
        human_dna.push_back(streng);
    }

    ifstream chimp_if{"chimp.dna"};
    while (chimp_if >> streng)
    {
        chimp_dna.push_back(streng);
    }

    DNAsequentie dna(human_dna[0]);
    cout << dna.d(chimp_dna[69]) << endl;

    for (int i = 0; i < human_dna.size(); i++)
    {
        int min_afstand = human_dna[i].d(chimp_dna[0]);
        int min_index = 0;
        for (int j = 1; j < chimp_dna.size(); j++)
        {
            int afstand = human_dna[i].d(chimp_dna[j]);
            if (afstand < min_afstand)
            {
                min_afstand = afstand;
                min_index = j;
            }
        }
        cout << "Menselijk " << i << " komt overeen met chimpansees " << min_index << " met afstand " << min_afstand << endl;
    }

    return 0;
}