#include <memory>
#include <iostream>
#include <vector>
#include <cassert>
#include <sstream>
#include <fstream>

using namespace std;

class Knoop;
typedef unique_ptr<Knoop> Knoopptr;

class TernaireZoekboom
{
public:
    TernaireZoekboom() : knopen(UCHAR_MAX) {}
    void voegToe(const string &woord);
    bool zoek(const string &woord);
    void teken(const char *bestandsnaam);
    vector<Knoopptr> knopen;
};

class Knoop
{
public:
    Knoop(const unsigned char data) : data(data) {}
    string tekenrec(ostream &uit, int &knoopteller);
    Knoopptr links, midden, rechts;
    unsigned char data;
    bool isLast = false;
};

void TernaireZoekboom::voegToe(const string &woord)
{
    Knoopptr *ptr = &knopen[woord[0]];
    int i = 0;
    while (i < woord.size())
    {
        if (*ptr && (*ptr)->data < woord[i])
        {
            //Indien gedefinieerd en LINKS
            ptr = &((*ptr)->links);
        }
        else if (*ptr && (*ptr)->data > woord[i])
        {
            //Indien gedefinieerd en RECHTS
            ptr = &((*ptr)->rechts);
        }
        else if (*ptr)
        {
            //Indien gedefinieerd en MIDDEN;
            ptr = &((*ptr)->midden);
            i++;
        }
        else
        {
            *ptr = make_unique<Knoop>(woord[i++]);
            if (i != woord.size())
                ptr = &((*ptr)->midden);
        }
    }
    (*ptr)->isLast = true;
}

bool TernaireZoekboom::zoek(const string &woord)
{
    Knoopptr *ptr = &knopen[woord[0]];
    int i = 0;
    while (i < woord.size())
    {
        if (*ptr && (*ptr)->data < woord[i])
        {
            //Indien gedefinieerd en LINKS
            ptr = &((*ptr)->links);
        }
        else if (*ptr && (*ptr)->data > woord[i])
        {
            //Indien gedefinieerd en RECHTS
            ptr = &((*ptr)->rechts);
        }
        else if (*ptr)
        {
            //Indien gedefinieerd en MIDDEN;
            if (i != woord.size() - 1)
                ptr = &((*ptr)->midden);
            i++;
        }
        else
        {
            if (i != woord.size() - 1)
                return false;
        }
    }
    if ((*ptr)->isLast)
        return true;
    return false;
}

void TernaireZoekboom::teken(const char *bestandsnaam)
{
    ofstream uit(bestandsnaam);
    assert(uit);
    int knoopteller = 0;
    uit << "digraph {\nnode[label=\"\"]\n";
    for (int i = 0; i < knopen.size(); i++)
    {
        if (knopen[i])
            knopen[i]->tekenrec(uit, knoopteller);
    }
    uit << "}";
}

string Knoop::tekenrec(ostream &uit, int &knoopteller)
{
    ostringstream wortelstring;
    wortelstring << '"' << ++knoopteller << '"';
    if (!this)
    {
        uit << wortelstring.str() << " [shape=point];\n";
    }
    else
    {
        if (this->isLast)
        {
            uit << wortelstring.str() << "[label=\"(" << this->data << " LAST"
                << ")\"]";
        }
        else
        {
            uit << wortelstring.str() << "[label=\"(" << this->data << ")\"]";
        }
        uit << ";\n";

        string kind = links->tekenrec(uit, knoopteller);
        uit << wortelstring.str() << " -> " << kind << ";\n";

        kind = midden->tekenrec(uit, knoopteller);
        uit << wortelstring.str() << " -> " << kind << ";\n";

        kind = rechts->tekenrec(uit, knoopteller);
        uit << wortelstring.str() << " -> " << kind << ";\n";
    };
    return wortelstring.str();
}