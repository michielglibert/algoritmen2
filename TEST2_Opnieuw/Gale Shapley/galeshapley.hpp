#include <vector>
#include <queue>
#include <iostream>

using namespace std;

vector<int> galeshapley(vector<vector<int>> &voorkeurMannen, vector<vector<int>> &voorkeurVrouwen)
{
    //Ranglijst opstellen
    //Vrouw i en man j op positie k
    vector<vector<int>> ranglijst(voorkeurVrouwen.size());
    for (int i = 0; i < voorkeurVrouwen.size(); i++)
    {
        ranglijst[i].resize(voorkeurMannen.size());
        for (int j = 0; j < voorkeurVrouwen[i].size(); j++)
        {
            ranglijst[i][voorkeurVrouwen[i][j]] = j;
        }
    }

    //-1 = niet gekoppeld
    vector<int> koppelingMannen(voorkeurMannen.size(), -1);
    vector<int> koppelingVrouwen(voorkeurVrouwen.size(), -1);
    queue<int> mannenWachtrij;

    for (int i = 0; i < voorkeurMannen.size(); i++)
    {
        mannenWachtrij.push(i);
    }
    //Algo uitvoeren
    while (!mannenWachtrij.empty())
    {
        int man = mannenWachtrij.front();
        mannenWachtrij.pop();
        for (int vrouw : voorkeurMannen[man])
        {
            if (koppelingVrouwen[vrouw] == -1 || ranglijst[vrouw][man] < ranglijst[vrouw][koppelingVrouwen[vrouw]])
            {
                if (koppelingVrouwen[vrouw] != -1)
                {
                    cout << "Vrouw " << vrouw << " ontbindt de relatie met man " << koppelingVrouwen[vrouw] << "." << endl;
                    mannenWachtrij.push(koppelingVrouwen[vrouw]);
                }
                cout << "Man " << man << " trouwt met vrouw " << vrouw << "." << endl;
                koppelingVrouwen[vrouw] = man;
                koppelingMannen[man] = vrouw;
                break;
            }
        }
    }

    return koppelingMannen;
}