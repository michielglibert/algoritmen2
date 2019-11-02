#ifndef __DNASEQUENTIE_H
#define __DNASEQUENTIE_H

#include <iostream>
#include <vector>
#include <string>

using namespace std;

class DNAsequentie : public string
{
public:
    DNAsequentie(const std::string &s) : string(s) {}
    int d(const string &) const;
    void print_matrix(vector<vector<int>> &, const string &) const;
};

void DNAsequentie::print_matrix(vector<vector<int>> &matrix, const string &s) const
{
    //Functie voor uitrprinten ter controle
    cout << "      ";
    for (int i = 0; i < s.size(); i++)
    {
        cout << s[i] << "  ";
    }
    cout << endl;
    for (int i = 0; i < matrix.size(); i++)
    {
        if (i == 0)
        {
            cout << "   ";
        }
        if (i >= 1)
        {
            cout << (*this)[i - 1] << "  ";
        }
        for (int j = 0; j < matrix[i].size(); j++)
        {
            cout << matrix[i][j] << "  ";
        }
        cout << endl;
    }
    cout << endl;
}

int DNAsequentie::d(const string &s) const
{
    //Mogelijke oplossing:
    //      d o o r
    //    0 0 0 0 0
    // d  0 1 1 1 1
    // o  0 1 2 2 2
    // m  0 1 2 2 2
    //Er zijn 3 verschillende operaties in de tabel
    //Dit zijn: {links}, {boven} en {diagonaal}

    //LET OP
    //In dit geval moeten we het probleem iets anders aanpakken
    //Als we 2 gelijke letters vinden is dat geen mutatie dus geven we 0 terug
    //2 Verschillende letters duid echter wel op 1 mutatie dus doen we daar wel 1 + ...
    //We zoeken ipv het maximum, het minimum van de deelsequenties
    //We doen dus eigenlijk gewoon het omgekeerde dan bij een normale LGD

    //2D vector voor tabel c initialiseren
    //Nulwaardes voor lege substring vergelijkingen
    vector<vector<int>> c(this->length() + 1, vector<int>(s.length() + 1, 0));

    //Overlopen tabel
    for (int i = 1; i < c.size(); i++)
    {
        for (int j = 1; j < c[i].size(); j++)
        {
            if ((*this)[i - 1] == s[j - 1])
            {
                //Hetzelfde? Sowieso mutatieafstand 0
                c[i][j] = c[i - 1][j - 1];
            }
            else
            {
                //Niet hetzelfde? 1 + de minimale deelsequentie
                //Hier moeten we het minimum vinden van de 3 operaties vermeldt in de opgave
                c[i][j] = min(1 + c[i - 1][j], min(1 + c[i][j - 1], 1 + c[i - 1][j - 1]));
            }
        }
    }

    //print_matrix(c, s);
    return c[this->length()][s.length()];
}

#endif