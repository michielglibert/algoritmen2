#include <memory>
#include <fstream>
#include <cassert>
#include <sstream>
#include "stringmetbits.h"

using namespace std;

class TrieKnoop;
class TrieNietBlad;
typedef unique_ptr<TrieKnoop> Knoopptr;

//BINAIRE TRIE

//We veronderstellen dat er geen prefixen mogen worden toegevoegd
//Waardoor alles in de bladeren wordt opgeslaan
class BinaireTrie : public Knoopptr
{
public:
    using Knoopptr::unique_ptr;
    BinaireTrie &operator=(Knoopptr &&a)
    {
        Knoopptr::operator=(move(a));
        return *this;
    }
    void voegToe(const string &sleutel);
    bool zoek(const string &sleutel);
    void teken(const char *bestandsnaam);
    string tekenrec(ostream &uit, int &knoopteller);
};

class TrieKnoop
{
public:
    virtual bool isBlad() = 0;
};

class TrieNietBlad : public TrieKnoop
{
public:
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
    bool isBlad()
    {
        return true;
    }
    string sleutel;
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
        if ((*this)->isBlad())
        {
            TrieBlad *blad = static_cast<TrieBlad *>(&**this);
            StringMetBits bitSleutel(blad->sleutel);
            uit << wortelstring.str() << "[label=\"" << blad->sleutel << " (";

            for (int i = 0; i < bitSleutel.geefAantalBits(); i++)
            {
                uit << bitSleutel.bit(i);
            }

            uit << ") \"];";
        }
        else
        {
            uit << wortelstring.str() << "[label=\"\"];";
            TrieNietBlad *nietBlad = static_cast<TrieNietBlad *>(&**this);

            string links = nietBlad->links.tekenrec(uit, knoopteller);
            string rechts = nietBlad->rechts.tekenrec(uit, knoopteller);
            uit << wortelstring.str() << " -> " << links << "[label=\"0\"];\n";
            uit << wortelstring.str() << " -> " << rechts << "[label=\"1\"];\n";
        }
    };
    return wortelstring.str();
}