#ifndef __HUFFMANTRIE_H
#define __HUFFMANTRIE_H

#include <memory>
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <queue>
#include <string>
#include <cassert>
#include <sstream>
#include "pq.h"

using namespace std;

class Knoop;
class Blad;
class NietBlad;
typedef unique_ptr<Knoop> Knoopptr;
class HuffmanTrie;

class HuffmanTrie : public Knoopptr
{

public:
    HuffmanTrie(char *);
    //Move constructor
    HuffmanTrie(Knoopptr &&a) : Knoopptr(move(a)) {}
    //Move operator
    HuffmanTrie &operator=(Knoopptr &&a)
    {
        Knoopptr::operator=(move(a));
        return *this;
    }

    void schrijfNaarBestand(string);
    void teken(const char *bestandsnaam);

    HuffmanTrie() {}

private:
    void schrijfNaarBestand_rec(ostream &uit, string code);
    string tekenrec(ostream &uit, int &knoopteller);
    void voegSamen(HuffmanTrie *, HuffmanTrie *, HuffmanTrie *);
};

class Knoop
{
public:
    virtual bool isBlad() = 0;
    int gewicht;

protected:
    Knoop(int gewicht) : gewicht(gewicht) {}
};

class NietBlad : public Knoop
{
public:
    NietBlad(int gewicht) : Knoop(gewicht) {}
    virtual bool isBlad()
    {
        return false;
    }
    HuffmanTrie links, rechts;
};

class Blad : public Knoop
{
public:
    Blad(char letter, int gewicht) : letter(letter), Knoop(gewicht) {}
    char letter = ' ';
    virtual bool isBlad()
    {
        return true;
    }
};

class HuffmanTriePointerCompare
{
public:
    bool operator()(HuffmanTrie &a, HuffmanTrie &b)
    {
        return a->gewicht > b->gewicht;
    }
};

HuffmanTrie::HuffmanTrie(char *bestandsnaam)
{
    //Frequentietabel maken
    vector<int> freq(256);

    ifstream input{bestandsnaam};
    string lijn;
    while (getline(input, lijn))
    {
        for (int i = 0; i < lijn.size(); i++)
        {
            freq[lijn[i]]++;
        }
    }

    //Toevoegen aan queue
    //Movable_priority_queue werd gegevens door Cnops, zal dus ook op examen aanwezig zijn
    //Dit omwille van het feit dat de normale std::priority_queue niet met unique_pointers kan werken
    //Aangezizen deze een const value teruggeeft.
    movable_priority_queue<HuffmanTrie, vector<HuffmanTrie>, HuffmanTriePointerCompare> queue;
    for (int i = 0; i < freq.size(); i++)
    {
        if (freq[i])
        {
            Knoopptr ptr = make_unique<Blad>(i, freq[i]);
            HuffmanTrie trie(move(ptr));
            queue.push(move(trie));
        }
    }

    //Steeds 2 kleinste deelbomen eruithalen en samenvoegen
    while (queue.size() != 1)
    {
        //2 kleinste eruithalen
        HuffmanTrie trie1 = queue.top_and_pop();
        HuffmanTrie trie2 = queue.top_and_pop();

        //Samenvoegen (samengevoegde komt terecht in trie1)
        int totaalGewicht = trie1->gewicht + trie2->gewicht;

        Knoopptr ptr = make_unique<NietBlad>(totaalGewicht);
        HuffmanTrie nieuweTrie(move(ptr));
        voegSamen(&trie1, &trie2, &nieuweTrie);

        //Samengevoegde deelboom opnieuw in priority queue stoppen
        queue.push(move(nieuweTrie));
    }

    //Huffmantrie klaar
    *this = move(queue.top_and_pop());
}

void HuffmanTrie::voegSamen(HuffmanTrie *trie1, HuffmanTrie *trie2, HuffmanTrie *nieuweTrie)
{
    static_cast<NietBlad *>(nieuweTrie->get())->links = move(*trie2);  //Bevat grootste
    static_cast<NietBlad *>(nieuweTrie->get())->rechts = move(*trie1); //Bevat kleinste
}

void HuffmanTrie::schrijfNaarBestand(string bestandsnaam)
{
    //Deze functie gaat de letter en zijn bijhorende hoffmancode uitschrijven
    ofstream output{bestandsnaam};
    this->schrijfNaarBestand_rec(output, "");
}

void HuffmanTrie::schrijfNaarBestand_rec(ostream &uit, string code)
{
    //Deze functie gaat de letter en zijn bijhorende hoffmancode uitschrijven
    if ((*this)->isBlad())
    {
        //Knoop is blad
        Blad *blad = static_cast<Blad *>(this->get());
        uit << blad->letter << " " << code << endl;
    }
    else
    {
        //Knoop is geen blad, recursief voortgaan
        NietBlad *nietblad = static_cast<NietBlad *>(this->get());
        nietblad->links.schrijfNaarBestand_rec(uit, code + "0");
        nietblad->rechts.schrijfNaarBestand_rec(uit, code + "1");
    }
}

void HuffmanTrie::teken(const char *bestandsnaam)
{
    ofstream uit(bestandsnaam);
    assert(uit);
    int knoopteller = 0; //knopen moeten een eigen nummer krijgen.
    uit << "digraph {\nnode[label=\"\"]\n";
    this->tekenrec(uit, knoopteller);
    uit << "}";
}

string HuffmanTrie::tekenrec(ostream &uit, int &knoopteller)
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
            Blad *blad = static_cast<Blad *>(this->get());
            uit << wortelstring.str() << "[label=\"(" << blad->letter << ": " << blad->gewicht << ")\"]";
            uit << ";\n";
        }
        else
        {
            NietBlad *nietblad = static_cast<NietBlad *>(this->get());
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

#endif