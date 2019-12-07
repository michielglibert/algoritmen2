#include <iostream>
#include <fstream>
#include <string>
#include "thompsonna.h"
#include "regexp11.h"
#include "da.h"

using std::cerr;
using std::cout;
using std::endl;
using std::ifstream;
using std::string;

int main(int argc, char **argv)
{
    // if (argc != 3)
    // {
    //     cerr << "Not enough arguments" << endl;
    //     return 1;
    // }

    //string bestandsnaam = argv[2];
    //Regexp regexp(argv[1]);

    string bestandsnaam = "test.txt";
    Regexp regexp("(t|a)*");
    ThompsonNA automaat(regexp);
    automaat.teken("tekening");

    DA da(regexp);

    ifstream input{bestandsnaam};
    string line;
    while (getline(input, line))
    {
        cout << da.overloop(line) << endl;
    }

    return 0;
}