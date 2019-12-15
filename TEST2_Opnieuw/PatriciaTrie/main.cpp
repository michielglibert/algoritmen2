#include <iostream>
#include "patriciatrie.hpp"

using namespace std;

int main()
{
    PatriciaTrie trie;
    trie.voegToe("lol");
    cout << trie.get() << endl;
    // trie.voegToe("woord");
    // trie.voegToe("worst");

    // trie.voegToe("saus");
    // trie.voegToe("sardine");
    // trie.voegToe("sardinie");
    // trie.voegToe("zeeboot");
    // trie.voegToe("zee");
    // cout << trie.zoek("zeeboot");
    // trie.verwijder("zeeboot");
    // trie.teken("test");
    return 0;
}