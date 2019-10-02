#include <iostream>
#include "zoekboom17.h";
#include <string>

int main()
{

    Zoekboom<int, string> boom;
    boom.voegtoe(4, "test", false);
    boom.voegtoe(2, "test 2", false);
    boom.voegtoe(1, "test 2", false);
    boom.voegtoe(3, "test 2", false);
    boom.voegtoe(5, "test 2", false);
    std::cout << boom.geefDiepte() << std::endl;
    boom.roteerLinks(2);
    std::cout << "Hello, World!" << std::endl;
    boom.schrijf(std::cout);

    return 0;
}