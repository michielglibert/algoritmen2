#ifndef __DECODEER_H
#define __DECODEER_H
#include <iostream>
#include <fstream>
#include <string>
#include "varbitpatroon.h"
#include "bincode.h"
#include "bitstream.h"
#include "binairetrie.h"

using namespace std;

void decodeer(const char *codedFileName, const char *decodedFileName, const char *codeFileName)
{

    Bincode bincode(codeFileName);
    BinaireTrie trie(bincode);

    BinaireTrie *positie = &trie;
    ibitstream bitstream(codedFileName);
    bool huidigebit;
    ofstream output(decodedFileName);

    huidigebit = bitstream.leesbit();
    while (!bitstream.fail())
    {
        if ((*positie).get()->isBlad())
        {
            TrieBlad *blad = static_cast<TrieBlad *>((*positie).get());
            output << char(blad->letter);
            positie = &trie;
        }
        TrieNietblad *nietblad = static_cast<TrieNietblad *>((*positie).get());
        positie = &(nietblad->geefKind(huidigebit));

        huidigebit = bitstream.leesbit();
    }

    bitstream.close();
    output.close();
}

#endif
