#ifndef __KNUTHMORRIS_H
#define __KNUTHMORRIS_H

#include <queue>
#include <iostream>

using namespace std;

class KnuthMorris
{
public:
    KnuthMorris(const string &patroon) : patroon(patroon), prefixTabel(patroon.size())
    {
        int i = 0;
        for (int j = 1; j < patroon.size(); j++)
        {
            if (patroon[i] == patroon[j])
            {
                prefixTabel[j] = i++;
            }
            else
            {
                i = 0;
            }
        }
    }
    queue<int> zoek(const string &tekst, long int &teller);
    queue<int> zoekNaief(const string &tekst, long int &teller);

private:
    //Houdt voor het karakter op die bepaalde positie in string patroon de prefixwaarde bij
    vector<int> prefixTabel;
    const string &patroon;
};

queue<int> KnuthMorris::zoek(const string &tekst, long int &teller)
{
    queue<int> posities;
    //Wijzer naar de tekst
    int i = 0;
    //Wijzer naar het patroon
    int j = 0;
    while (i <= tekst.size())
    {
        teller++;
        if (patroon[j] == tekst[i])
        {
            if (j == patroon.size() - 1)
                posities.push(i - patroon.size() + 1);
            i++;
            j++;
        }
        else if (j - 1 >= 0)
        {
            j = prefixTabel[j - 1];
        }
        else
        {
            i++;
        }
    }
    return posities;
}

queue<int> KnuthMorris::zoekNaief(const string &tekst, long int &teller)
{
    queue<int> posities;
    int offset = 0;
    //Wijzer naar de tekst
    int i = 0;
    //Wijzer naar het patroon
    int j = 0;
    while (offset + patroon.size() <= tekst.size())
    {
        teller++;
        if (patroon[j] == tekst[offset + i])
        {
            if (j == patroon.size() - 1)
                posities.push(i - patroon.size() + 1);
            i++;
            j++;
        }
        else
        {
            offset++;
            i = 0;
            j = 0;
        }
    }
    return posities;
}

#endif