#ifndef __CODEER_H
#define __CODEER_H
#include <iostream>
#include <fstream>
#include <string>
#include "varbitpatroon.h"
#include "bincode.h"
#include "bitstream.h"
#include "binairetrie.h"

void codeer(const char *codeFile, const char *newCode)
{
    Bincode bincode(codeFile);
    obitstream bitstream(newCode);
    for (int i = 0; i < 257; i++)
    {
        for (int j = 0; j < bincode[i].geeflengte(); j++)
        {
            bool bit = bincode[i].geefbit(j) == 0 ? false : true;
            bitstream.schrijfbit(bit);
        }
    }
    bitstream.close();
}
#endif