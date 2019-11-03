#ifndef __PRQUADTREE_H__
#define __PRQUADTREE_H__
#include <cassert>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <memory>
#include <functional>
#include <stack>
#include <algorithm>
#include <string>
#include <cassert>
#include <sstream>

using namespace std;

/*****************************************************************************
    Bevat de headers en code voor een PRQuadtree
    en voor de abstracte klasse PRKnoop met deelklassen Blad en Nietblad
    
*****************************************************************************/

class PRKnoop;
typedef unique_ptr<PRKnoop> Knoopptr;
class PRQuadtree;

class PRQuadtree : public Knoopptr
{
public:
    using Knoopptr::unique_ptr;
    PRQuadtree(int a) : maxcoordinaat{a} {};
    PRQuadtree(Knoopptr &&a) : Knoopptr(move(a)){};
    PRQuadtree &operator=(Knoopptr &&a)
    {
        Knoopptr::operator=(move(a));
        return *this;
    };
    void preEnPostOrder(std::function<void(PRKnoop *)> &bezoekPre, std::function<void(PRKnoop *)> &bezoekPost) const;

    //te implementeren
    void voegToe(int x, int y);
    int geefDiepte();
    void teken(const char *bestandsnaam);
    string tekenrec(ostream &uit, int &knoopteller);
    //de PRquadtree kan alleen punten bevatten met
    //-maxcoordinaat <= x < maxcoordinaat
    //-maxcoordinaat <= y < maxcoordinaat
    int maxcoordinaat;
};

//Opmerking: om de functies specifiek aan een deelklasse te kunnen gebruiken moet je soms een
//static_cast doen, zoals in
//  PRKnoop* knoopptr=...;
//  if (knoopptr!=nullptr && knoopptr->isBlad() && static_cast<PRBlad*>(knoopptr)->x == 5)
class PRKnoop
{
public:
    virtual bool isBlad() = 0;
    virtual int geefDiepte() = 0;
};
class PRBlad : public PRKnoop
{
public:
    PRBlad(int x, int y) : x{x}, y{y} {};
    virtual bool isBlad() { return true; }
    int x, y; //co"ordinaten punt
    virtual int geefDiepte()
    {
        return 1;
    };
};
class PRNietblad : public PRKnoop
{
public:
    virtual bool isBlad() { return false; }
    virtual int geefDiepte();
    int geefAantalKinderen()
    {
        int aantal = 0;
        for (int i = 0; i < 4; i++)
        {
            if (kind[i])
                ++aantal;
        }
        return aantal;
    };
    //xc, yc: co"ordinaten van het centrum van het gebied
    Knoopptr *geefKind(int x, int y, int xc, int yc)
    {
        int xindex = (x <= xc ? WEST : OOST);
        int yindex = (y <= yc ? NOORD : ZUID);
        return &kind[xindex + yindex];
    };
    static const int OOST, WEST, NOORD, ZUID;
    Knoopptr kind[4]; //indexeren met windrichting (bv. kind[NOORD+WEST] bevat punten
                      //met x en y kleiner dan grenswaarde)
                      //leeg gebied: nulpointer
};

void PRQuadtree::preEnPostOrder(std::function<void(PRKnoop *)> &bezoekPre, std::function<void(PRKnoop *)> &bezoekPost) const
{
    enum staat
    {
        pre,
        post
    };
    std::stack<std::pair<PRKnoop *, staat>> DEstack; //bevat alleen niet-nulpointers
    if (*this)
        DEstack.emplace(this->get(), pre);
    while (!DEstack.empty())
    {
        auto [nuknoop, nustaat] = DEstack.top();
        if (nustaat == pre)
        {
            bezoekPre(nuknoop);
            DEstack.top().second = post;
            if (!nuknoop->isBlad())
            {
                for (int i = 0; i < 4; i++)
                {
                    Knoopptr *kind = &(static_cast<PRNietblad *>(nuknoop)->kind[i]);
                    if (*kind)
                        DEstack.emplace(kind->get(), pre);
                };
            };
        }
        else
        {
            bezoekPost(nuknoop);
            DEstack.pop();
        };
    };
};

void PRQuadtree::voegToe(int x, int y)
{
    Knoopptr *knoop = this;
    if (x < -this->maxcoordinaat || x > this->maxcoordinaat || y < -this->maxcoordinaat || y > this->maxcoordinaat)
    {
        cerr << "Coordinates out of bound for(" << -this->maxcoordinaat << " - " << this->maxcoordinaat << "): " << x << ", " << y << endl;
        return;
    }

    int xLeftBound, xRightBound, yLeftBound, yRightBound, maximum, xc, yc;
    xLeftBound = yLeftBound = -this->maxcoordinaat;
    xRightBound = yRightBound = this->maxcoordinaat;
    cerr << "new instance" << endl;
    while (*knoop && !(*knoop)->isBlad() && (xLeftBound < xRightBound || yLeftBound < yRightBound))
    {
        cerr << xc << "," << yc << endl;
        xc = xLeftBound + ((xRightBound - xLeftBound) / 2);
        yc = yLeftBound + ((yRightBound - yLeftBound) / 2);

        if (x <= xc && y <= yc)
        {
            //Linksboven
            xRightBound = xc;
            yRightBound = yc;
        }
        else if (x <= xc && y > yc)
        {
            //Linksonder
            xRightBound = xc;
            yLeftBound = yc + 1;
        }
        else if (x > xc && y <= yc)
        {
            //Rechtsboven
            xLeftBound = xc + 1;
            yRightBound = yc;
        }
        else
        {
            //Rechtsonder
            xLeftBound = xc + 1;
            yLeftBound = yc + 1;
        }

        PRNietblad *nietblad = static_cast<PRNietblad *>(knoop->get());
        knoop = nietblad->geefKind(x, y, xc, yc);
    }

    if (*knoop && (*knoop)->isBlad())
    {
        PRBlad *blad = static_cast<PRBlad *>(knoop->get());
        //Is blad = nieuwe niet-blad maken en knoop toevoegen
        int blad_x = blad->x;
        int blad_y = blad->y;

        if (blad_x == x && blad_y == y)
        {
            cerr << "Point already defined" << endl;
            return;
        }

        *knoop = PRQuadtree(make_unique<PRNietblad>());
        this->voegToe(x, y);
        this->voegToe(blad_x, blad_y);
    }
    else
    {
        cerr << "Added: " << x << ", " << y << endl;
        *knoop = make_unique<PRBlad>(x, y);
    }
}

int PRQuadtree::geefDiepte()
{
    if (*this)
    {
        return (*this)->geefDiepte();
    }
    else
    {
        return 0;
    }
}

int PRNietblad::geefDiepte()
{
    return 0;
}

void PRQuadtree::teken(const char *bestandsnaam)
{
    ofstream uit(bestandsnaam);
    assert(uit);
    int knoopteller = 0; //knopen moeten een eigen nummer krijgen.
    uit << "digraph {\nnode[label=\"\"]\n";
    this->tekenrec(uit, knoopteller);
    uit << "}";
}

string PRQuadtree::tekenrec(ostream &uit, int &knoopteller)
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
            PRBlad *blad = static_cast<PRBlad *>(this->get());
            uit << wortelstring.str() << "[label=\"(" << blad->x << "," << blad->y << ")\"]";
            uit << ";\n";
        }
        else
        {
            PRNietblad *blad = static_cast<PRNietblad *>(this->get());
            for (int i = 0; i < 4; i++)
            {
                if (blad->kind[i])
                {
                    PRQuadtree *tree = static_cast<PRQuadtree *>(&(blad->kind[i]));
                    string kind = tree->tekenrec(uit, knoopteller);
                    uit << wortelstring.str() << " -> " << kind << ";\n";
                }
            }
        }
    };
    return wortelstring.str();
}

const int PRNietblad::OOST = 0;
const int PRNietblad::WEST = 1;
const int PRNietblad::NOORD = 0;
const int PRNietblad::ZUID = 2;
#endif
