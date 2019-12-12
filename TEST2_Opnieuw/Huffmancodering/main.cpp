#include <iostream>
#include "bitstream.h"
#include "binairetrie.hpp"
#include "stringmetbits.h"

void codeerBestand(const char *bronBestandsNaam, const char *doelBestandsNaam, char *codeBestandsNaam)
{
    BinaireTrie trie(bronBestandsNaam);
    trie.teken("huffmantrie");
    Bincode code = trie.geefCode();
    trie.schrijfCode(codeBestandsNaam);

    ifstream input(bronBestandsNaam);
    obitstream output(doelBestandsNaam);
    unsigned char c;

    while (input >> std::noskipws >> c)
    {
        for (int i = 0; i < code[c].geeflengte(); i++)
        {
            output.schrijfbit(code[c].geefbit(i));
        }
    }

    input.close();
    output.close();
}

int main()
{
    codeerBestand("tekst.txt", "biblecodeert.cps", "code.dat");
    return 0;
}