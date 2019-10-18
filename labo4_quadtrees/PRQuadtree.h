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
using std::cerr;
using std::endl;
using std::unique_ptr;

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
    void schrijf();
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
    virtual void schrijf(int diepte) = 0;
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
    virtual void schrijf(int diepte)
    {
        for (int i = 0; i < diepte; i++)
            std::cout << "\t";
        std::cout << "Blad met coordinaten " << this->x << " en " << this->y << std::endl;
    }
};
class PRNietblad : public PRKnoop
{
public:
    virtual bool isBlad() { return false; }
    virtual int geefDiepte();
    virtual void schrijf(int diepte)
    {
        for (int i = 0; i < diepte; i++)
            std::cout << "\t";
        std::cout << "Intermediaire knoop met " << this->geefAantalKinderen() << " kinderen" << std::endl;
        for (int i = 0; i < 4; i++)
        {
            if (kind[i])
                kind[i]->schrijf(diepte + 1);
        }
    }
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
        int xindex = (x < xc ? WEST : OOST);
        int yindex = (y < yc ? NOORD : ZUID);
        return &kind[xindex + yindex];
    };
    static const int OOST, WEST, NOORD, ZUID;
    Knoopptr kind[4]; //indexeren met windrichting (bv. kind[NOORD+WEST] bevat punten
                      //met x en y kleiner dan grenswaarde)
                      //leeg gebied: nulpointer
};

void PRQuadtree::schrijf()
{
    if (*this)
        (*this)->schrijf(0);
}

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
    // Checken of punt binnen de bounds ligt
    if (x < 0 || y < 0 || x > this->maxcoordinaat || y > this->maxcoordinaat)
    {
        cerr << "Point out of bounds" << endl;
        return;
    }

    // Als punt een blad is en bestaat
    //cerr << "Adding point with coordinates (" << x << "," << y << ")" << endl;
    Knoopptr *knoop = this;
    int xc, yc, maximum;
    xc = yc = 0;
    maximum = this->maxcoordinaat;

    while (*knoop && !(*knoop)->isBlad())
    {
        maximum / 2 == 0 ? maximum = 1 : maximum /= 2;
        xc = x <= xc ? xc - maximum : xc + maximum;
        yc = y <= yc ? yc - maximum : yc + maximum;
        knoop = static_cast<PRNietblad *>(knoop->get())->geefKind(x, y, xc, yc);
    }

    if (*knoop && (*knoop)->isBlad())
    {
        PRBlad *blad = static_cast<PRBlad *>(knoop->get());

        if (x == blad->x && y == blad->y)
        {
            cerr << "Point already defined" << endl;
            return;
        }

        int blad_x = blad->x;
        int blad_y = blad->y;

        *knoop = std::make_unique<PRNietblad>();
        this->voegToe(blad_x, blad_y);
        this->voegToe(x, y);
    }
    else
    {
        *knoop = std::make_unique<PRBlad>(PRBlad(x, y));
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
    if (this)
    {
        int diepte = 0;
        for (int i = 0; i < 4; i++)
        {
            if (this->kind[i])
            {
                diepte = std::max(diepte, this->kind[i]->geefDiepte());
            }
        }
        return 1 + diepte;
    }
    else
    {
        return 0;
    }
}

const int PRNietblad::OOST = 0;
const int PRNietblad::WEST = 1;
const int PRNietblad::NOORD = 0;
const int PRNietblad::ZUID = 2;
#endif
