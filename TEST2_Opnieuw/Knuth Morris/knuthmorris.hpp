#include <queue>
#include <iostream>
#include <vector>

using namespace std;

typedef unsigned int uint;
typedef unsigned char uchar;

class KnuthMorrisPratt
{
public:
    KnuthMorrisPratt(const std::string &naald);
    KnuthMorrisPratt(const uchar *naald, uint _naaldlengte);
    void zoek(queue<int> &, const uchar *, uint, int &);
    std::queue<int> zoekr(const std::string &hooiberg) const;

private:
    vector<int> prefix;
    const uchar *naald;
    string _naald;
    uint _naaldlengte;

    std::vector<int> prefix_tabel;
    std::vector<int> kmp_tabel;
};

//Eigen implementatie
KnuthMorrisPratt::KnuthMorrisPratt(const uchar *naald, uint _naaldlengte)
{
    this->naald = naald;
    this->_naaldlengte = _naaldlengte;
    prefix.resize(_naaldlengte, 0);
    kmp_tabel.resize(_naaldlengte, 0);

    //We bouwen de prefix tabel op
    //Komen de letters overeen dan stijgt de prefixlengte j
    //Komen deze letters niet overeen dan is j gelijk aan
    //De vorige prefix, immers zo werkt het algoritme zelf ook
    //We moeten niet j terug op 0 zetten omdat er meerdere
    //Prefixen kunnen aanwezig zijn
    //We bouwen de prefixtabel op, op de manier waarop het algoritme werkt
    for (size_t i = 1; i < prefix.size(); i++)
    {
        int j = prefix[i - 1];
        while (j > 0 && naald[i] != naald[j])
        {
            j = prefix[j - 1];
        }
        if (naald[i] == naald[j])
            j++;
        prefix[i] = j;
    }

    //Aangezien er een aantal vergelijkingen nutteloos zijn
    //Maken we het algoritme nog beter door degene te verwijderen
    //Waarbij Naald[i + 1] == Naald[prefix[i]]
    //Zijn ze wel gelijk dan gebruiken we de prefixwaarde van ervoor
    //De intuitie is, als Naald[i] al niet past met Hooiberg[i]
    //,dan heeft het ook geen zin om Naald[prefix[i]] == Hooiberg[i]
    //uit te voeren aangezien een waarde in de prefixtabel naar
    //dezelfde letter wijst.
    for (size_t i = 1; i < kmp_tabel.size(); i++)
    {
        if (naald[i + 1] != naald[prefix[i]])
        {
            //Letter niet hetzelfde, prefixwaarde overnemen want is correct
            kmp_tabel[i] = prefix[i];
        }
        else if (prefix[i] != 0)
        {
            //Is niet gelijk aan 0 EN letter hetzelfde prefixwaarde van vorige overnemen
            kmp_tabel[i] = kmp_tabel[prefix[i]];
        }
    }
    for (int i = 0; i < _naaldlengte; i++)
    {
        cout << kmp_tabel[i];
    }
    cout << endl;
}

//Eigen implementatie
void KnuthMorrisPratt::zoek(queue<int> &plaats, const uchar *hooiberg, uint hooiberglengte, int &aantalVergelijkingen)
{
    int j = 0;
    //Hooiberge volledig overlopen
    for (int i = 0; i < hooiberglengte; i++)
    {
        cout << "i: " << i << " - j: " << j << endl;
        //J moet groter zijn dan 0 (er een match gevonden zijn en niet de 1ste waarde)
        //Indien ze niet gelijk zijn aan mekaar -> prefixwaarde van waarde ervoor overnemen
        //Op deze manier wordt de wijzer naar het patroon verschoven naar de juiste waarde
        while (j > 0 && hooiberg[i] != naald[j])
        {
            aantalVergelijkingen++;
            //Waarde van ervoor overnemen
            j = kmp_tabel[j - 1];
        }

        //Indien het de eerste waarde is of de waarde gelijk was, patroon incrementeren
        j++;

        //Match gevonden, pushen naar queue
        if (j == _naaldlengte)
        {
            plaats.push(1);
        }
    }
}

//Implementatie assisent 2018
KnuthMorrisPratt::KnuthMorrisPratt(const std::string &_naald)
    : _naald{_naald}, prefix_tabel((_naald.size() + 1), -1), kmp_tabel((_naald.size() + 1), -1)
{
    int prefix_lengte = -1; // == prefix_tabel[0]
    for (size_t i = 1; i < prefix_tabel.size(); i++)
    {
        while ((prefix_lengte >= 0) && (_naald[i - 1] != _naald[prefix_lengte]))
        {
            prefix_lengte = prefix_tabel[prefix_lengte];
        }

        prefix_lengte++;
        prefix_tabel[i] = prefix_lengte;
    }

    for (size_t i = 0; i < prefix_tabel.size(); i++)
    {
        std::cout << _naald[i];
    }
    std::cout << endl;

    for (size_t i = 0; i < prefix_tabel.size(); i++)
    {
        std::cout << prefix_tabel[i];
    }
    std::cout << endl;

    // Extra heuristiek in de cursus
    // Zie CLRS 32.4-6 op p. 1012 of Gusfield op p. 51
    for (size_t i = 1; i < kmp_tabel.size(); i++)
    {
        if (prefix_tabel[i] == 0)
        {
            kmp_tabel[i] = 0;
        }
        else if (_naald[i] != _naald[prefix_tabel[i]])
        {
            //cout << _naald[i] << " en " << _naald[prefix_tabel[i]] << endl;
            kmp_tabel[i] = prefix_tabel[i];
        }
        else
        {
            // cout << _naald[i] << " en " << _naald[prefix_tabel[i]] << endl;
            kmp_tabel[i] = kmp_tabel[prefix_tabel[i]];
        }
    }
    for (size_t i = 0; i < kmp_tabel.size(); i++)
    {
        std::cout << _naald[i];
    }
    std::cout << endl;

    for (size_t i = 0; i < kmp_tabel.size(); i++)
    {
        std::cout << kmp_tabel[i];
    }
    std::cout << endl;
}

//Implementatie assisent 2018
std::queue<int> KnuthMorrisPratt::zoekr(const std::string &hooiberg) const
{
    if (hooiberg.empty())
    {
        return std::queue<int>{};
    }

    int aantal = 0; // DEBUG

    std::queue<int> gevonden;

    int prefix_lengte = 0;
    for (size_t i = 1; i <= hooiberg.size(); i++) // Let op de <= in de for-voorwaarde! bv. "aba" zoeken in "ababa"
    {
        while ((prefix_lengte >= 0) && (hooiberg[i - 1] != _naald[prefix_lengte]))
        {
            prefix_lengte = prefix_tabel[prefix_lengte];
            aantal++; // DEBUG
        }

        prefix_lengte++;

        if (prefix_lengte == static_cast<int>(_naald.size()))
        {
            gevonden.push(i - _naald.size());
            // prefix_lengte = tabel[prefix_lengte]; // Is dit nodig? Zie CLRS
        }
    }

    std::cout << "aantal:" << aantal << std::endl; // DEBUG

    return gevonden;
}
