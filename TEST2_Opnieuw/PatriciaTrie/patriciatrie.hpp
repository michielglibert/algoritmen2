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
class NietBlad;
typedef unique_ptr<Knoop> Knoopptr;

//De implementatie in de cursus is heel vaag
//Na wat verduidelijking gevraagd te hebben aan de prof
//Blijkt de cursustekst nu nog minder duidelijker.
//Hij gaf een totaal andere uitleg aan een patriciatrie:
// - Beginnen met een unique pointer (zoals altijd bij zijn bomen)
// - Tabel van kindpointers, geindexeerd volgens hun letter
// - Index, wijst naar de te controleren letter
// - Op het einde checken of de string wel klopt
//Mijn implementatie volgt dan ook zijn uitleg.
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
    virtual bool isBlad() = 0;
};

class NietBlad : public Knoop
{
public:
    bool isBlad() { return false; }
    NietBlad() : kinderen(UCHAR_MAX) {}
    NietBlad(int index) : kinderen(UCHAR_MAX), index(index) {}
    int index;
    vector<PatriciaTrie> kinderen;
};

class Blad : public Knoop
{
public:
    Blad(string sleutel) : sleutel(sleutel) {}
    bool isBlad() { return true; }
    string sleutel;
};

void PatriciaTrie::voegToe(const string &sleutel)
{
    Knoopptr *ptr = this;
    if (!*ptr)
    {
        *ptr = (Knoopptr)std::make_unique<Blad>(sleutel);
    }
    else
    {
        char laatste = '';
        //We gaan door zolang het geen blad is
        while (*ptr && !(*ptr)->isBlad())
        {
            NietBlad *nietBlad = static_cast<NietBlad *>(&**this);

            //Index is te groot, splits niet blad op
            if (static_cast<NietBlad *>(&**this)->index > sleutel.size())
            {
                //maak een nieuw nietBlad aan, de index is hier de sleutel.size()
                //dit haalt nogmaals aan waarom bij zoeken een controle vereist is op het einde
                Knoopptr ptr = make_unique<NietBlad>(sleutel.size());
                NietBlad *nieuweNietBlad = static_cast<NietBlad *>(ptr.get());

                //Zet this op het nieuwe nietBlad

                //Hier loop ik vast, je weet namelijk niet wat de letter is op positie sleutel.size()
                //Van het 'woord' dat er al zat. Je weet enkel de index. De reden dat je deze splitsing
                //maalt is net omdat het woord dat je nu wilt toevoegen te klein is. We willen dus bvb
                //Een woord van lengte 4 toevoegen terwijl we momenteel in een knoop van index 7 zitten.
                //We maken een nieuwe knoop met index 5, maar hoe weten we dan wat de letter op index 5
                //was van het woord dat hier ooit werd toegevoegd? De enige manier lijkt mij om de boom
                //willekeurig af te dalen, maar dit is toch zeer omslachtig?

                //De enigste performante oplossing IMO, dat ook duidelijk op het internet te vinden is,is door
                //geen nutteloze index bij te houden maar per knoop een sequentie van karakters.
                //Op die manier moeten gewoon de letter per knoop worden bekeken (wat sowieso gebeurt op het einde)
                //en daarnaast vereist dit praktisch evenveel geheugenruimte.
                nieuweNietBlad->kinderen[]

                //zet ptr op de juiste plaats in de vector
                //zal nullptr zijn en verder worden afgehandeld na de while loop
            }
            else
            {
                //Naar juiste kind verplaatsen
                laatste = sleutel[nietBlad->index];
                ptr = &nietBlad->kinderen[sleutel[nietBlad->index]];
                break;
            }
        }

        if (*ptr)
        {
            //Leeg blad, maak blad aan
        }
        else
        {
            //Geen leeg blad, splits blad op

            //Zoek de laatste overeenkomstige letter

            //Maak nietBlad aan

            //Maak nieuwe bladeren aan

            //Zet blad op juiste plaats in kindvector
        }
    }
}

bool PatriciaTrie::zoek(const string &sleutel)
{
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
        if ((*this)->isBlad())
        {
            Blad *blad = static_cast<Blad *>(&**this);
            uit << wortelstring.str() << "[label=\"" << blad->sleutel << "\"];";
        }
        else
        {
            uit << wortelstring.str() << "[label=\"\"];";
            NietBlad *nietBlad = static_cast<NietBlad *>(&**this);

            for (int i = 0; i < nietBlad->kinderen.size(); i++)
            {
                if (nietBlad->kinderen[i])
                {
                    string kind = nietBlad->kinderen[i].tekenrec(uit, knoopteller);
                    uit << wortelstring.str() << " -> " << kind << "[label=\"" << (char)i << "\"];\n";
                }
            }
        }
    };
    return wortelstring.str();
}

#endif