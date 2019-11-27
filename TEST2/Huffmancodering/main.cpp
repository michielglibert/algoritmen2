#include <iostream>
#include "huffmantrie.h"

int main()
{
    HuffmanTrie trie("test.txt");
    cout << trie->gewicht << endl;
    trie.teken("test");
    trie.schrijfNaarBestand("code");
    return 0;
}