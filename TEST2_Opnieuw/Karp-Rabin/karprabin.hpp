#include <iostream>
#include <vector>
#include <queue>
#include <cmath>

using std::cout;
using std::endl;
using std::pow;
using std::queue;
using std::string;

class KarpRabin
{
public:
    KarpRabin(const string &naald, const int alfabetGrootte) : naald(naald), naaldHashWaarde(hashFunctie(naald, alfabetGrootte)), alfabetGrootte(alfabetGrootte)
    {
        cout << "Hashwaarde voor de naald is: " << naaldHashWaarde << endl;
    }
    queue<int> zoek(const string &hooiberg);

protected:
    bool isPriem(const int nummer) const;
    int grootstePriem(const int alfabetGrootte) const;
    int hashFunctie(const string &woord, const int alfabetGrootte) const;

private:
    const string naald;
    const int naaldHashWaarde;
    const int alfabetGrootte;
};

queue<int> KarpRabin::zoek(const string &hooiberg)
{
    queue<int> matches;
    int aantalVergelijkingen = 0;

    for (int i = 0; i < hooiberg.size(); i++)
    {
        aantalVergelijkingen++;
        string subStr = hooiberg.substr(i, naald.size());
        int hashWaardeSubStr = hashFunctie(subStr, alfabetGrootte);
        if (hashWaardeSubStr == naaldHashWaarde)
        {
            int j;
            for (j = 0; j < naald.size(); j++)
            {
                aantalVergelijkingen++;
                if (naald[j] != hooiberg[i + j])
                    break;
            }
            if (j == naald.size())
                matches.push(j - naald.size() + 1);
        }
    }

    cout << "Er werd " << aantalVergelijkingen << " keer vergeleken." << endl;

    return matches;
}

int KarpRabin::hashFunctie(const string &woord, const int alfabetGrootte) const
{
    int waarde = 0;
    int r = grootstePriem(alfabetGrootte);

    for (int i = 0; i < woord.size(); i++)
    {
        waarde += woord[i] * pow(alfabetGrootte, woord.size() - i - 1);
    }

    return waarde % r;
}

bool KarpRabin::isPriem(const int nummer) const
{
    if (nummer <= 1)
        return false;

    for (size_t i = 2; i <= nummer / 2; i++)
        if (nummer % i == 0)
            return false;

    return true;
}

int KarpRabin::grootstePriem(const int alfabetGrootte) const
{
    //Grootte van processorwoord
    int grootste = 1;
    int priem = 1;

    //We beperken ons hier tot 2^16 zodat de uitvoering niet te lang duurt
    int w = sizeof(size_t) * 2;

    //Alles testen tot max 2^w
    while (priem * alfabetGrootte <= pow(2, w))
    {
        grootste = priem++;
        while (!isPriem(priem))
        {
            priem++;
        }
    }

    return grootste;
}