#include <iostream>
#include "ternairezoekboom.hpp"

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
    cout << boom.zoek("homo") << endl;
    boom.teken("tekening");
    cout << "Finished!" << endl;
    return 0;
}