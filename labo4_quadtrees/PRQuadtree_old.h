#ifndef __PRQUADTREE_H
#define __PRQUADTREE_H

#include <iostream>
#include <vector>
#include <fstream>
#include <cassert>
#include <sstream>
#include <string>

using namespace std;

class Point
{
public:
    int x, y;
    string data;
    Point(int x, int y) : x(x), y(y) {}
};

class PRQuadtree
{
public:
    int x1, y1, x2, y2;
    Point *point;
    vector<PRQuadtree> kinderen;

    PRQuadtree(int x1, int y1, int x2, int y2) : x1(x1), y1(y1), x2(x2), y2(y2) {}
    void voegToe(int, int, string);
    Point zoek(int, int) const;
    void teken(const char *);
    string tekenrec(ostream &, int &);
};

Point PRQuadtree::zoek(int x, int y) const
{
}

void PRQuadtree::teken(const char *bestandsnaam)
{
    ofstream uit(bestandsnaam);
    assert(uit);
    int knoopteller = 0; //knopen moeten een eigen nummer krijgen.
    uit << "digraph {\n";
    this->tekenrec(uit, knoopteller);
    uit << "}";
}

string PRQuadtree::tekenrec(ostream &uit, int &knoopteller)
{
    ostringstream wortelstring;
    wortelstring << '"' << ++knoopteller << '"';
    if (!this)
    {
        uit << wortelstring.str() << " [shape=point];\n";
    }
    else
    {
        uit << wortelstring.str() << "[label=\"" << this->point->x << " " << this->point->y << ":" << this->point->data << "\"]";
        uit << ";\n";
        string kind1 = this->kinderen[0].tekenrec(uit, knoopteller);
        string kind2 = this->kinderen[1].tekenrec(uit, knoopteller);
        string kind3 = this->kinderen[2].tekenrec(uit, knoopteller);
        string kind4 = this->kinderen[3].tekenrec(uit, knoopteller);
        uit << wortelstring.str() << " -> " << kind1 << ";\n";
        uit << wortelstring.str() << " -> " << kind2 << ";\n";
        uit << wortelstring.str() << " -> " << kind3 << ";\n";
        uit << wortelstring.str() << " -> " << kind4 << ";\n";
    };
    return wortelstring.str();
}

#endif