#include <iostream>
#include "trie.h"

int main()
{
    Trie trie;
    trie.voegToe("woord");
    trie.voegToe("worst");

    trie.voegToe("saus");
    trie.voegToe("sardine");
    trie.voegToe("sardinie");
    trie.voegToe("zeeboot");
    trie.voegToe("zee");
    cout << trie.zoek("zeeboot");
    trie.verwijder("zeeboot");
    trie.teken("test");
    return 0;
}