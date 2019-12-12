//Gestolen van levimigom

#include "galeshapley.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
using std::cin;
using std::cout;
using std::endl;
using std::ifstream;
using std::string;
using std::vector;

void lees(string bestand, vector<vector<int>> &voorkeur_vrouwen, vector<vector<int>> &voorkeur_mannen)
{
    ifstream input(bestand);
    int aantal_mannen, aantal_vrouwen;

    // aantal mannen en vrouwen inlezen
    input >> aantal_vrouwen >> aantal_mannen;

    // voorkeur vrouwen inlezen
    voorkeur_vrouwen.resize(aantal_vrouwen + 1);
    for (int vrouw = 1; vrouw <= aantal_vrouwen; vrouw++)
    {
        voorkeur_vrouwen[vrouw].resize(aantal_mannen);
        for (int j = 0; j < aantal_mannen; j++)
            input >> voorkeur_vrouwen[vrouw][j];
    }

    // voorkeur mannen inlezen
    voorkeur_mannen.resize(aantal_mannen + 1);
    for (int man = 1; man <= aantal_mannen; man++)
    {
        voorkeur_mannen[man].resize(aantal_vrouwen);
        for (int j = 0; j < aantal_vrouwen; j++)
            input >> voorkeur_mannen[man][j];
    }

    input.close();
}

int main()
{
    vector<vector<int>> voorkeur_mannen, voorkeur_vrouwen;
    lees("input.txt", voorkeur_vrouwen, voorkeur_mannen);

    pair<vector<int>, vector<int>> koppelingen = galeshapley(voorkeur_mannen, voorkeur_vrouwen);
    vector<int> koppeling = koppelingen.first;

    cout << "------------------" << endl;
    cout << "STABIELE KOPPELING" << endl;
    cout << "------------------" << endl;
    for (int i = 1; i < koppeling.size(); i++)
    {
        cout << "Man " << i << " trouwt met vrouw " << koppeling[i] << endl;
    }

    return 0;
}