#include <iostream>
#include "zoekboom1.h"
#include <string>

int main()
{
    Zoekboom<int, string> boom;
    boom.voegtoe(4, "test", false);
    boom.voegtoe(2, "test 2", false);
    boom.voegtoe(1, "test 2", false);
    boom.voegtoe(3, "test 2", false);
    boom.voegtoe(2, "test 2", false);
    boom.voegtoe(0, "test", false);
    boom.voegtoe(10, "test 2", false);
    boom.voegtoe(8, "test 2", false);
    boom.voegtoe(5, "test 2", false);
    boom.voegtoe(11, "test 2", false);

    boom.teken("start");
    boom.roteer(2);

    boom.teken("file1");
    boom.roteer(1);
    boom.maakOnevenwichtig();
    boom.teken("file2");
    std::cout << "Hello, World!" << std::endl;

    return 0;
}