//Eerste lijn bevat het aantal mannen en dan het aantal vrouwen
//Vervolgens wordt elke lijn uitgelezen en toegevoegd aan de corrsponderende vector

#include <vector>
#include <iostream>
#include <fstream>
#include "galeshapley.hpp"

using namespace std;

void leesBestand(char *bestandsNaam, vector<vector<int>> &voorkeurMannen, vector<vector<int>> &voorkeurVrouwen, vector<string> &mannenNamen, vector<string> &vrouwenNamen)
{
    int aantalMannen, aantalVrouwen;
    ifstream input{bestandsNaam};

    input >> aantalMannen >> aantalVrouwen;
    voorkeurMannen.resize(aantalMannen);
    voorkeurVrouwen.resize(aantalVrouwen);

    //Mannen instellen
    for (int i = 0; i < aantalMannen; i++)
    {
        voorkeurMannen[i].resize(aantalVrouwen);
        for (int j = 0; j < aantalVrouwen; j++)
        {
            input >> voorkeurMannen[i][j];
            voorkeurMannen[i][j]--;
        }
        string naam;
        input >> naam;
        mannenNamen.push_back(naam);
    }

    //Vrouwen instellen
    for (int i = 0; i < aantalVrouwen; i++)
    {
        voorkeurVrouwen[i].resize(aantalMannen);
        for (int j = 0; j < aantalMannen; j++)
        {
            input >> voorkeurVrouwen[i][j];
            voorkeurVrouwen[i][j]--;
        }
        string naam;
        input >> naam;
        vrouwenNamen.push_back(naam);
    }

    input.close();
}

int main()
{
    vector<vector<int>> voorkeurMannen, voorkeurVrouwen;
    vector<string> mannenNamen, vrouwenNamen;
    leesBestand("input1.txt", voorkeurMannen, voorkeurVrouwen, mannenNamen, vrouwenNamen);
    vector<int> voorkeuren = galeshapley(voorkeurMannen, voorkeurVrouwen);

    for (int i = 0; i < voorkeuren.size(); i++)
    {
        cout << mannenNamen[i] << " is gekoppeld met " << vrouwenNamen[voorkeuren[i]] << "." << endl;
    }
    return 0;
}