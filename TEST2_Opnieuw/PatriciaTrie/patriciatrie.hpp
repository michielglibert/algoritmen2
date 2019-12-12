#ifndef __PATRICIATRIE_H
#define __PATRICIATRIE_H

#include <memory>
#include <vector>
#include <fstream>
#include <cassert>
#include <string>
#include <sstream>
#include <iostream>

using namespace std;

class Knoop;
typedef unique_ptr<Knoop> Knoopptr;

class PatriciaTrie : public Knoopptr
{
public:
    using Knoopptr::unique_ptr;
    PatriciaTrie() = default;
    PatriciaTrie(Knoopptr &&a) : Knoopptr(move(a)){};
    PatriciaTrie &operator=(Knoopptr &&a)
    {
        Knoopptr::operator=(move(a));
        return *this;
    }
    bool zoek(const string &);
    void voegToe(const string &);
    void teken(const char *bestandsnaam);
    string tekenrec(ostream &uit, int &knoopteller);
};

class Knoop
{
public:
    PatriciaTrie links, rechts;
    int index;
    unsigned char data;
    string sleutel;
};

void PatriciaTrie::teken(const char *bestandsnaam)
{
    ofstream uit(bestandsnaam);
    assert(uit);
    int knoopteller = 0; //knopen moeten een eigen nummer krijgen.
    uit << "digraph {\nnode[label=\"\"]\n";
    this->tekenrec(uit, knoopteller);
    uit << "}";
}

string PatriciaTrie::tekenrec(ostream &uit, int &knoopteller)
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

#endif