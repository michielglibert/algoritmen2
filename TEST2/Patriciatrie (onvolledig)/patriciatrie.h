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
    PatriciaTrie()
    {
        *this = make_unique<Knoop>();
    }
    PatriciaTrie(Knoopptr &&a) : Knoopptr(move(a)){};
    PatriciaTrie &operator=(Knoopptr &&a)
    {
        Knoopptr::operator=(move(a));
        return *this;
    }
    bool zoek(const string &);
    void voegToe(const string &);
    void verwijder(const string &);
    void teken(const char *bestandsnaam);
    string tekenrec(ostream &uit, int &knoopteller);

private:
    bool verwijder_rec(const string &, int);
};

class Knoop
{
public:
    Knoop() {}
    Knoop(string a) : data(a) {}
    bool isLastNode();
    void setLastNode();
    bool bevatKind(char);
    int geefKind(char);

    string data = " ";
    vector<PatriciaTrie> kinderen;

private:
    bool isLast = false;
};

bool PatriciaTrie::zoek(const string &woord)
{
    Knoopptr *ptr = this;
    int i = 0;
    for (i = 0; i < woord.size(); i++)
    {
        if ((*ptr)->bevatKind(woord[i]))
        {
            int kindIndex = (*ptr)->geefKind(woord[i]);
            ptr = &((*ptr)->kinderen[kindIndex]);
        }
        else
        {
            return false;
        }
    }

    if (i == woord.size() && (*ptr)->isLastNode())
        return true;

    return false;
}

void PatriciaTrie::voegToe(const string &woord)
{
    Knoopptr *ptr = this;

    for (int i = 0; i < woord.size(); i++)
    {
        if ((*ptr)->bevatKind(woord[i]))
        {
            //karakter zit erin
            int kindIndex = (*ptr)->geefKind(woord[i]);
            ptr = &((*ptr)->kinderen[kindIndex]);
        }
        else
        {

            //karakter zit er niet in
            Knoopptr nieuw = make_unique<Knoop>(woord[i]);
            (*ptr)->kinderen.push_back(move(nieuw));
            int kindIndex = (*ptr)->geefKind(woord[i]);
            ptr = &((*ptr)->kinderen[kindIndex]);
        }
    }

    (*ptr)->setLastNode();
}

void PatriciaTrie::verwijder(const string &woord)
{
    verwijder_rec(woord, 0);
}

bool PatriciaTrie::verwijder_rec(const string &woord, int i)
{
    if (woord.size() - 1 == i && (*this)->kinderen.size() == 1)
    {
        this->release();
        return false;
    }
    else
    {
        int kindIndex = (*this)->geefKind(woord[i]);
        bool value = true;

        if (kindIndex != -1)
        {
            value = (*this)->kinderen[kindIndex].verwijder_rec(woord, i + 1);

            if ((*this)->kinderen.size() != 1)
                value = true;

            if ((*this)->isLastNode())
            {
                value = true;
                (*this)->kinderen.erase((*this)->kinderen.begin() + kindIndex);
            }
        }

        if (value)
        {
            return true;
        }
        else
        {
            this->release();
            return false;
        }
    }
}

bool Knoop::isLastNode()
{
    return isLast;
}

void Knoop::setLastNode()
{
    isLast = true;
}

int Knoop::geefKind(char a)
{
    int kindIndex = -1;
    for (int i = 0; i < kinderen.size(); i++)
    {
        if (kinderen[i]->data == a)
        {
            kindIndex = i;
            break;
        }
    }
    return kindIndex;
}

bool Knoop::bevatKind(char a)
{
    int kindIndex = geefKind(a);
    return kindIndex == -1 ? false : a == kinderen[kindIndex]->data;
}

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
        if ((*this)->isLastNode())
        {
            uit << wortelstring.str() << "[label=\"(" << (*this)->data << "-0"
                << ")\"]";
        }
        else
        {
            uit << wortelstring.str() << "[label=\"(" << (*this)->data << ")\"]";
        }
        uit << ";\n";

        for (int i = 0; i < (*this)->kinderen.size(); i++)
        {
            string kind = (*this)->kinderen[i].tekenrec(uit, knoopteller);
            uit << wortelstring.str() << " -> " << kind << ";\n";
        }
    };
    return wortelstring.str();
}

#endif