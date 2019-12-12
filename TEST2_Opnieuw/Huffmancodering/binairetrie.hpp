#include <memory>
#include <fstream>
#include <cassert>
#include <sstream>
#include <vector>
#include "pq.h"
#include "bincode.h"
#include "varbitpatroon.h"
#include "stringmetbits.h"

using namespace std;

class TrieKnoop;
class TrieNietBlad;
class TrieBlad;
typedef unique_ptr<TrieKnoop> Knoopptr;

//BINAIRE TRIE aangepast voor huffmantree mee op te bouwen

class BinaireTrie : public Knoopptr
{
public:
    using Knoopptr::unique_ptr;
    BinaireTrie() = default;
    //Move constructor
    BinaireTrie(Knoopptr &&a) : Knoopptr(move(a)) {}
    BinaireTrie(const char *bestandsNaam);
    BinaireTrie &operator=(Knoopptr &&a)
    {
        Knoopptr::operator=(move(a));
        return *this;
    }
    void voegToe(const string &sleutel);
    bool zoek(const string &sleutel);
    void teken(const char *bestandsnaam);
    string tekenrec(ostream &uit, int &knoopteller);
    Bincode geefCode();
    void schrijfCode(const char *naam);

private:
    void voegSamen(BinaireTrie *trie1, BinaireTrie *trie2);
};

class TrieKnoop
{
public:
    TrieKnoop() = default;
    virtual bool isBlad() = 0;
    int gewicht;

protected:
    TrieKnoop(int gewicht) : gewicht(gewicht) {}
};

class TrieNietBlad : public TrieKnoop
{
public:
    TrieNietBlad() = default;
    TrieNietBlad(int gewicht) : TrieKnoop(gewicht) {}
    bool isBlad()
    {
        return false;
    }
    BinaireTrie links, rechts;
};

class TrieBlad : public TrieKnoop
{
public:
    TrieBlad(const string &sleutel) : sleutel(sleutel) {}
    TrieBlad(const unsigned char letter, const int gewicht) : letter(letter), sleutel(sleutel), TrieKnoop(gewicht) {}
    bool isBlad()
    {
        return true;
    }
    string sleutel;
    unsigned char letter;
};

void BinaireTrie::voegToe(const string &sleutel)
{
    Knoopptr *ptr = this;
    if (!*ptr)
    {
        *ptr = make_unique<TrieNietBlad>();
    }

    if (!zoek(sleutel))
    {
        Knoopptr *ptr = this;
        StringMetBits bitSleutel(sleutel);

        for (int i = 0; i < bitSleutel.geefAantalBits() - 1; i++)
        {
            if (bitSleutel.bit(i))
            {
                //Ga naar rechts
                TrieNietBlad *nietBlad = static_cast<TrieNietBlad *>(&**ptr);
                ptr = &nietBlad->rechts;
                if (!*ptr)
                {
                    *ptr = make_unique<TrieNietBlad>();
                }
            }
            else
            {
                //Ga naar links
                TrieNietBlad *nietBlad = static_cast<TrieNietBlad *>(&**ptr);
                ptr = &nietBlad->links;
                if (!*ptr)
                {
                    *ptr = make_unique<TrieNietBlad>();
                }
            }
        }

        TrieNietBlad *nietBlad = static_cast<TrieNietBlad *>(&**ptr);
        if (bitSleutel.bit(bitSleutel.geefAantalBits() - 1))
        {
            //Ga naar rechts
            ptr = &nietBlad->rechts;
            *ptr = make_unique<TrieBlad>(sleutel);
        }
        else
        {
            //Ga naar links
            ptr = &nietBlad->links;
            *ptr = make_unique<TrieBlad>(sleutel);
        }
    }
    else
    {
        cerr << "String already in trie" << endl;
    }
}

class BinaireTrieCompare
{
public:
    bool operator()(BinaireTrie &a, BinaireTrie &b)
    {
        return a->gewicht > b->gewicht;
    }
};

void BinaireTrie::voegSamen(BinaireTrie *trie1, BinaireTrie *trie2)
{
    static_cast<TrieNietBlad *>(this->get())->links = move(*trie2);  //Bevat grootste
    static_cast<TrieNietBlad *>(this->get())->rechts = move(*trie1); //Bevat kleinste
}
BinaireTrie::BinaireTrie(const char *bestandsNaam)
{

    ifstream input{bestandsNaam};
    vector<int> freq(UCHAR_MAX);
    unsigned char c;

    while (input >> noskipws >> c)
    {
        freq[c]++;
    }

    movable_priority_queue<BinaireTrie, vector<BinaireTrie>, BinaireTrieCompare> pq;
    for (int i = 0; i < freq.size(); i++)
    {
        if (freq[i] != 0)
        {
            Knoopptr ptr = make_unique<TrieBlad>(i, freq[i]);
            BinaireTrie trie(move(ptr));
            pq.push(move(trie));
        }
    }

    while (pq.size() != 1)
    {
        BinaireTrie trie1 = move(pq.top_and_pop());
        BinaireTrie trie2 = move(pq.top_and_pop());

        int totaalGewicht = trie1->gewicht + trie2->gewicht;
        Knoopptr ptr = make_unique<TrieNietBlad>(totaalGewicht);
        BinaireTrie nieuweTrie(move(ptr));
        nieuweTrie.voegSamen(&trie1, &trie2);
        pq.push(move(nieuweTrie));
    }

    *this = pq.top_and_pop();
}

bool BinaireTrie::zoek(const string &sleutel)
{
    Knoopptr *ptr = this;
    StringMetBits bitSleutel(sleutel);

    int i = 0;
    while (*ptr && i < bitSleutel.geefAantalBits())
    {
        if (bitSleutel.bit(i))
        {
            //Ga naar rechts
            TrieNietBlad *nietBlad = static_cast<TrieNietBlad *>(&**ptr);
            ptr = &nietBlad->rechts;
        }
        else
        {
            //Ga naar links
            TrieNietBlad *nietBlad = static_cast<TrieNietBlad *>(&**ptr);
            ptr = &nietBlad->links;
        }
        i++;
    }

    if (*ptr && (*ptr)->isBlad())
    {
        TrieBlad *trieBlad = static_cast<TrieBlad *>(&**ptr);
        if (sleutel == trieBlad->sleutel)
        {
            return true;
        }
    }

    return false;
}

void BinaireTrie::teken(const char *bestandsnaam)
{
    ofstream uit(bestandsnaam);
    assert(uit);
    int knoopteller = 0; //knopen moeten een eigen nummer krijgen.
    uit << "digraph {\nnode[label=\"\"]\n";
    this->tekenrec(uit, knoopteller);
    uit << "}";
}

string BinaireTrie::tekenrec(ostream &uit, int &knoopteller)
{
    ostringstream wortelstring;
    wortelstring << '"' << ++knoopteller << '"';
    if (!*this)
    {
        uit << wortelstring.str() << " [shape=point];\n";
    }
    else
    {
        if (*this && (*this)->isBlad())
        {
            TrieBlad *blad = static_cast<TrieBlad *>(this->get());
            uit << wortelstring.str() << "[label=\"(" << blad->letter << ": " << blad->gewicht << ")\"]";
            uit << ";\n";
        }
        else
        {
            TrieNietBlad *nietblad = static_cast<TrieNietBlad *>(this->get());
            uit << wortelstring.str() << "[label=\"(" << nietblad->gewicht << ")\"]";
            uit << ";\n";

            string kind = nietblad->links.tekenrec(uit, knoopteller);
            uit << wortelstring.str() << " -> " << kind << ";\n";

            kind = nietblad->rechts.tekenrec(uit, knoopteller);
            uit << wortelstring.str() << " -> " << kind << ";\n";
        }
    };
    return wortelstring.str();
}

Bincode BinaireTrie::geefCode()
{
    Bincode bincode;
    queue<pair<Varbitpatroon, BinaireTrie *>> wachtrij;
    Varbitpatroon start;

    wachtrij.push(make_pair(start, this));

    while (!wachtrij.empty())
    {
        pair<Varbitpatroon, BinaireTrie *> pair = wachtrij.front();
        wachtrij.pop();

        if (!(*pair.second)->isBlad())
        {
            TrieNietBlad *nietBlad = static_cast<TrieNietBlad *>(pair.second->get());

            //Links
            Varbitpatroon patroon1 = pair.first;
            patroon1.voegToe(false);
            wachtrij.push(make_pair(patroon1, &nietBlad->links));

            //Rechts
            Varbitpatroon patroon2 = pair.first;
            patroon2.voegToe(true);
            wachtrij.push(make_pair(patroon2, &nietBlad->rechts));
        }
        else
        {
            //Is blad dus toevoegen aan code
            TrieBlad *blad = static_cast<TrieBlad *>(pair.second->get());
            bincode.code[blad->letter] = pair.first;
        }
    }
    return bincode;
}

void BinaireTrie::schrijfCode(const char *naam)
{
    Bincode code = geefCode();
    code.schrijf(naam);
}

// void BinaireTrie::teken(const char *bestandsnaam)
// {
//     ofstream uit(bestandsnaam);
//     assert(uit);
//     int knoopteller = 0; //knopen moeten een eigen nummer krijgen.
//     uit << "digraph {\nnode[label=\"\"]\n";
//     this->tekenrec(uit, knoopteller);
//     uit << "}";
// }

// string BinaireTrie::tekenrec(ostream &uit, int &knoopteller)
// {
//     ostringstream wortelstring;
//     wortelstring << '"' << ++knoopteller << '"';
//     if (!*this)
//     {
//         uit << wortelstring.str() << " [shape=point];\n";
//     }
//     else
//     {
//         if ((*this)->isBlad())
//         {
//             TrieBlad *blad = static_cast<TrieBlad *>(&**this);
//             StringMetBits bitSleutel(blad->sleutel);
//             uit << wortelstring.str() << "[label=\"" << blad->sleutel << " (";

//             for (int i = 0; i < bitSleutel.geefAantalBits(); i++)
//             {
//                 uit << bitSleutel.bit(i);
//             }

//             uit << ") \"];";
//         }
//         else
//         {
//             uit << wortelstring.str() << "[label=\"\"];";
//             TrieNietBlad *nietBlad = static_cast<TrieNietBlad *>(&**this);

//             string links = nietBlad->links.tekenrec(uit, knoopteller);
//             string rechts = nietBlad->rechts.tekenrec(uit, knoopteller);
//             uit << wortelstring.str() << " -> " << links << "[label=\"0\"];\n";
//             uit << wortelstring.str() << " -> " << rechts << "[label=\"1\"];\n";
//         }
//     };
//     return wortelstring.str();
// }
