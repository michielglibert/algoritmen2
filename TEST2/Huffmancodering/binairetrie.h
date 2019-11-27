#ifndef __BINAIRETRIE_H__
#define __BINAIRETRIE_H__
#include "bincode.h"
#include <functional>
#include <memory>
using std::cerr;
/**********************************************************************

   Class: BinaireTrie, met hulpklassen van knopen
   
   beschrijving: Huffmantries, geschikt voor byte-alfabet
   letters zijn 0-255, plus afsluitingsteken 256.
   Een lege Huffmantrie is zinloos, ook als deeltrie
   Alleen bij opbouw zijn er zo'n tries mogelijk.
   Vandaar dat de parameterloze constructor protected is
   
***************************************************************************/
using std::binary_function;
using std::cout;
class Trieknoop;

typedef std::unique_ptr<Trieknoop> upKnoop;

class BinaireTrie : public upKnoop
{
    friend class TrieNietblad;

public:
    BinaireTrie(const Bincode &);
    BinaireTrie(BinaireTrie &&a)
    {
        upKnoop::operator=(move(a));
    };
    BinaireTrie &operator=(upKnoop &&a)
    {
        upKnoop::operator=(move(a));
        return *this;
    };
    BinaireTrie &operator=(BinaireTrie &&a)
    {
        upKnoop::operator=(move(a));
        return *this;
    };

protected:
    BinaireTrie() : upKnoop(){};
    BinaireTrie(upKnoop &&a) : upKnoop(move(a)){};
    //hulpfunctie voor constructor uitgaande van Bincode
    void voegToe(const Varbitpatroon &vb, int letter);
};

class Trieknoop
{
public:
    virtual bool isBlad() = 0;
};

class TrieBlad : public Trieknoop
{
public:
    TrieBlad(int letter) : letter{letter} {};
    virtual bool isBlad() { return true; }
    int letter;
};

class TrieNietblad : public Trieknoop
{
public:
    TrieNietblad(){};
    TrieNietblad(BinaireTrie &&l, BinaireTrie &&r) : links(move(l)), rechts(move(r)){};
    virtual bool isBlad() { return false; }
    BinaireTrie &geefKind(bool rechterkind)
    {
        return (rechterkind ? rechts : links);
    };
    BinaireTrie links, rechts;
};

BinaireTrie::BinaireTrie(const Bincode &bincode)
{
    *this = std::make_unique<TrieNietblad>();
    for (int i = 0; i < 257; i++)
    {
        this->voegToe(bincode[i], i);
    }
}

void BinaireTrie::voegToe(const Varbitpatroon &vb, int letter)
{
    BinaireTrie *boom = this;

    for (int i = 0; i < vb.geeflengte(); i++)
    {
        if (*boom == nullptr)
        {
            *boom = std::make_unique<TrieNietblad>();
        }

        TrieNietblad *knoop = static_cast<TrieNietblad *>((*boom).get());
        boom = &(knoop->geefKind(vb.geefbit(i)));
    }

    *boom = std::make_unique<TrieBlad>(letter);
}

#endif
