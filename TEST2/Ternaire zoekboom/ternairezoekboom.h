#ifndef __TERNAIRZOEKBOOM_H
#define __TERNAIRZOEKBOOM_H

#include <memory>
#include <iostream>
#include <vector>
#include <cassert>
#include <iostream>
#include <sstream>
#include <fstream>

using namespace std;

class Knoop;
typedef unique_ptr<Knoop> Knoopptr;

class TernaireZoekboom
{
public:
    vector<Knoopptr> knopen;
    int geefIndexChar(char);
    bool zoek(const string &);
    void voegToe(const string &);
    void teken(const char *bestandsnaam);
};

class Knoop
{
public:
    Knoop(char data) : data(data) {}
    string tekenrec(ostream &uit, int &knoopteller);
    char data;
    bool isLast = false;
    Knoopptr linkse;
    Knoopptr middelste;
    Knoopptr rechtse;
};

int TernaireZoekboom::geefIndexChar(char a)
{
    for (int i = 0; i < knopen.size(); i++)
    {
        if (knopen[i]->data == a)
        {
            return i;
        }
    }
    return -1;
}

bool TernaireZoekboom::zoek(const string &woord)
{
    int index = geefIndexChar(woord[0]);
    if (index == -1)
    {
        return false;
    }
    else
    {
        Knoopptr *ptr = &(knopen[index]);
        int i = 0;
        while (i < woord.size())
        {

            if ((*ptr)->data == woord[i])
            {
                //Middelste
                ptr = &((*ptr)->middelste);
                i++;
            }
            else if ((*ptr)->data < woord[i])
            {
                //Linkse
                ptr = &((*ptr)->linkse);
            }
            else
            {
                //Rechtse
                ptr = &((*ptr)->rechtse);
            }

            if (*ptr && (*ptr)->isLast && (*ptr)->data == woord[i] && i == woord.size() - 1)
            {
                return true;
            }
            else if (!*ptr)
            {
                return false;
            }
        }
    }
}

void TernaireZoekboom::voegToe(const string &woord)
{
    int index = geefIndexChar(woord[0]);
    if (index == -1)
    {
        //Eerste karakter niet aanwezig
        knopen.push_back(make_unique<Knoop>(woord[0]));
        index = geefIndexChar(woord[0]);
        Knoopptr *ptr = &(knopen[index]);

        for (int i = 1; i < woord.size(); i++)
        {
            (*ptr)->middelste = make_unique<Knoop>(woord[i]);
            ptr = &((*ptr)->middelste);
        }

        (*ptr)->isLast = true;
    }
    else
    {
        //Eerste karakter wel aanwezig
        Knoopptr *ptr = &(knopen[index]);
        int i = 0;
        while (i < woord.size())
        {
            if ((*ptr)->data == woord[i])
            {
                //Middelste
                ptr = &((*ptr)->middelste);
                i++;
            }
            else if ((*ptr)->data < woord[i])
            {
                //Linkse
                ptr = &((*ptr)->linkse);
            }
            else
            {
                //Rechtse
                ptr = &((*ptr)->rechtse);
            }

            if (!(*ptr))
            {
                //Undefined
                cout << woord[i] << endl;
                break;
            }
        }

        if (!(*ptr))
        {
            for (int j = i; j < woord.size(); j++)
            {
                (*ptr) = make_unique<Knoop>(woord[j]);
                if (j != woord.size() - 1)
                    ptr = &((*ptr)->middelste);
            }
            (*ptr)->isLast = true;
        }
    }
}

void TernaireZoekboom::teken(const char *bestandsnaam)
{
    ofstream uit(bestandsnaam);
    assert(uit);
    int knoopteller = 0;
    uit << "digraph {\nnode[label=\"\"]\n";
    for (int i = 0; i < knopen.size(); i++)
    {
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

        string kind = linkse->tekenrec(uit, knoopteller);
        uit << wortelstring.str() << " -> " << kind << ";\n";

        kind = middelste->tekenrec(uit, knoopteller);
        uit << wortelstring.str() << " -> " << kind << ";\n";

        kind = rechtse->tekenrec(uit, knoopteller);
        uit << wortelstring.str() << " -> " << kind << ";\n";
    };
    return wortelstring.str();
}

#endif