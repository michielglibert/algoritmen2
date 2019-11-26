#include <iostream>
#include "ternairezoekboom.h"

int main()
{
    TernaireZoekboom boom;
    boom.voegToe("boom");
    boom.voegToe("boot");
    boom.voegToe("bootter");

    boom.voegToe("homo");
    boom.voegToe("homosapien");
    boom.voegToe("homoerectus");
    boom.voegToe("hoogmoed");
    boom.voegToe("humo");
    cout << boom.zoek("bootter") << endl;
    boom.teken("test");
    return 0;
}