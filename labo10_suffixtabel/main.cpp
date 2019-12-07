#include "saisclass.h"
#include <iostream>
#include <string.h>
#include <cstring>
#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include <queue>
#include <memory.h>
#include <cstring>
using std::make_pair;
using std::pair;
using std::queue;

unsigned char *readfile(const char *filename, int *length)
{
    unsigned char *buffer = 0;
    FILE *f = fopen(filename, "rb");

    if (f)
    {
        fseek(f, 0, SEEK_END);
        *length = ftell(f);
        fseek(f, 0, SEEK_SET);
        buffer = (unsigned char *)malloc(*length);
        if (buffer)
        {
            fread(buffer, 1, *length, f);
        }
        fclose(f);
    }
    buffer[*length] = 0;
    return buffer;
}

void test(const char *bron)
{
    int bestandslengte, positie, verschuiving, lengte;
    unsigned char *t = readfile(bron, &bestandslengte);
    SAIS sais(t, bestandslengte);

    positie = 0;
    while (positie < bestandslengte)
    {
        sais.besteVerwijzing(verschuiving, lengte, positie);

        if (lengte == 0)
        {
            printf("%c", t[positie]);
            positie++;
        }
        else
        {
            printf("(%d, %d)", lengte, verschuiving);
            positie += lengte;
        }
    }

    free(t);
}

void comprimeer(const char *bron)
{
    int bestandslengte;
    unsigned char *t = readfile(bron, &bestandslengte);
    SAIS sais(t, bestandslengte);

    // bestand comprimeren
    int positie, verschuiving, lengte, verwijzingen, lengtesom;

    positie = verwijzingen = lengtesom = 0;
    while (positie < bestandslengte)
    {
        sais.besteVerwijzing(verschuiving, lengte, positie);

        if (lengte == 0)
        {
            positie++;
        }
        else
        {
            verwijzingen++;
            lengtesom += lengte;
            positie += lengte;
        }
    }

    printf("Oorspronkelijke tekst:\n");
    printf("\t%d karakters\n", bestandslengte);
    printf("\t%d verwijzingen\n", verwijzingen);
    printf("\t%.2f gemmidelde lengte verwijzingen\n", (double)lengtesom / (double)verwijzingen);
    printf("\t%d totale lengte verwijzingen\n", lengtesom);

    printf("Gecomprimeerd bestand:\n");
    printf("\t%d karakters (3 per verwijzing?)\n", (bestandslengte - lengtesom) + 3 * verwijzingen);

    free(t);
}

void valideer(const char *bron)
{
    // leest een bestand in, comprimeert het en decomprimeert het om te valideren of er geen data verloren gaat
    int bestandslengte, positie, verschuiving, lengte;
    unsigned char *t = readfile(bron, &bestandslengte);
    SAIS sais(t, bestandslengte);

    // voor comprimeren
    printf("---- voor comprimeren ----\n");
    std::cout << t << std::endl;

    // na comprimeren
    printf("---- na comprimeren ----\n");

    // het gecomprimeerde bestand word opgeslagen in queues
    queue<pair<int, int>> verwijzingen;
    queue<unsigned char> karakters;
    queue<bool> operaties;

    positie = 0;
    while (positie < bestandslengte)
    {
        sais.besteVerwijzing(verschuiving, lengte, positie);

        if (lengte == 0)
        {
            karakters.push(t[positie]);
            printf("%c", t[positie]);
            operaties.push(false);
            positie++;
        }
        else
        {
            verwijzingen.push(make_pair(lengte, verschuiving));
            printf("(%d, %d)", lengte, verschuiving);
            operaties.push(true);
            positie += lengte;
        }
    }

    // na decomprimeren
    printf("\n---- na decomprimeren ----\n");

    std::string tekst;
    while (!operaties.empty())
    {
        if (operaties.front())
        {
            auto p = verwijzingen.front();

            for (int j = 0; j < p.first; j++)
            {
                tekst += tekst.at(tekst.size() - p.second);
            }
            verwijzingen.pop();
        }
        else
        {
            tekst += karakters.front();
            karakters.pop();
        }
        operaties.pop();
    }

    std::cout << tekst << std::endl;

    // controleer of de teksten overeen komen
    printf("\n---- validatie ----\n");
    bool valid = true;
    for (int i = 0; i < bestandslengte; i++)
    {
        if (t[i] != tekst[i])
        {
            printf("Karakter %d komt niet overeen! (%c != %c)\n", i, t[i], tekst[i]);
        }
        valid = valid && (t[i] == tekst[i]);
    }

    if (valid)
        std::cout << "Er ging geen data verloren tijdens het comprimeren" << std::endl;
}

int main()
{
    //comprimeer("donquixote.txt");
    valideer("valideer.txt");

    return 0;
}