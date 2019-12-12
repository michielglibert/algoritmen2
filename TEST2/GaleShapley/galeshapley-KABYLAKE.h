#ifndef __GALESHAPLEY_H
#define __GALESHAPLEY_H

#include <vector>
#include <iostream>
#include <queue>

using namespace std;

//Een voorkeurs lijst ziet er het volgt uit
//i bevat de man/vrouw
//j bevat de positie in de ranglijst (0 = 1ste plaats, 1 = 2de plaats)
//de waarde [i][j] bevat dan de corresponderende man
pair<vector<int>, vector<int>> galeshapley(vector<vector<int>> &voorkeurMannen, vector<vector<int>> &voorkeurVrouwen)
{
    //We stellen eerst een ranglijst op. Met deze ranglijst kunnen we snel bepalen welke man er op welke rang staat
    //Voor een vrouw. Zo kunnen we snel teweten komen of een man beter is voor een vrouw.
    //i = vrouw, j = man, k = positie

    if (voorkeurMannen.size() != voorkeurVrouwen.size())
    {
        cerr << "Aantal vrouwen moet gelijk zijn aan aantal mannen" << endl;
    }

    vector<vector<int>> ranglijst(voorkeurVrouwen.size());

    for (int i = 0; i < voorkeurVrouwen.size(); i++)
    {
        ranglijst[i].resize(voorkeurVrouwen.size());
        for (int j = 0; j < voorkeurVrouwen.size(); j++)
        {
            ranglijst[i][voorkeurVrouwen[i][j]] = j;
        }
    }

    //Nu dat we de ranglijsten hebben kunnen we het algoritme zelf uitvoeren
    //Hierbij hebben we nog nood aan een lijst van vrije mannen en vrouwen
    //Hiermee maken we direct ook duidelijk aan welke man/vrouw de persoon
    //gekoppeld is.

    vector<int> koppellingMannen(voorkeurMannen.size(), -1);
    vector<int> koppelingVrouwen(voorkeurVrouwen.size(), -1);

    //We gebruiken een queue om de mannen te overlopen
    //Soms zal een man terug vrijgezel worden waardoor hij dus
    //opnieuw aan de queue moet worden toegevoegd
    queue<int> vrijeMannen;

    while (!vrijeMannen.empty())
    {
        //We zoeken een vrouw en kijken of ze vrij is
        int man = vrijeMannen.front();
        vrijeMannen.pop();

        int i = 0;

        while (koppellingMannen[man] == -1)
        {
            int vrouw = voorkeurMannen[man][i];

            if (koppelingVrouwen[vrouw] == -1 || ranglijst[vrouw][man] < ranglijst[vrouw][koppelingVrouwen[vrouw]])
            {
                if (koppelingVrouwen[vrouw] != -1)
                {
                    vrijeMannen.push(koppelingVrouwen[vrouw]);
                    cout << "Vrouw " << vrouw << " verbreekt haar verloving met man " << koppelingVrouwen[vrouw] << endl;
                }
                koppellingMannen[koppelingVrouwen[vrouw]] = -1;
                koppellingMannen[man] = vrouw;
                koppelingVrouwen[vrouw] = man;
                cout << "Man " << man << " verloofd zich met vrouw " << vrouw << endl;
            }

            i++;
        }
    }
}

#endif