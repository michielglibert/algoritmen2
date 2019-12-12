#include <iostream>
#include <vector>
#include <queue>
#include <functional>
#include <stdlib.h>
#include <cmath>
#include "simulanneal11.h"
#include "pugixml.cpp"
const char *node_types[] =
    {
        "null", "document", "element", "pcdata", "cdata", "comment", "pi", "declaration"};

using std::cerr;
using std::cout;
using std::endl;
using std::greater;
using std::pair;
using std::priority_queue;
using std::sqrt;
using std::string;
using std::vector;

struct knoop
{
    int nummer;
    float xCoord;
    float yCoord;
};

void DEZBezoekpre(pugi::xml_node n, std::function<void(pugi::xml_node)> bezoekKnoop = [](pugi::xml_node) {})
{
    bezoekKnoop(n);
    for (auto &&k : n.children())
        DEZBezoekpre(k, bezoekKnoop);
};

vector<knoop> maakVector(pugi::xml_node n)
{
    vector<knoop> knopen;
    DEZBezoekpre(n, [&knopen](pugi::xml_node nn) {
        string name = nn.name();
        if (name == "knooppunt")
        {
            int nummer = atoi(nn.attribute("nummer").value());
            float xCoord = atof(nn.attribute("xcoordinaat").value());
            float yCoord = atof(nn.attribute("ycoordinaat").value());

            struct knoop knoop;
            knoop.nummer = nummer;
            knoop.xCoord = xCoord;
            knoop.yCoord = yCoord;

            knopen.push_back(knoop);
        }
    });
    cerr << knopen.size() << endl;
    return knopen;
};

int main()
{
    //XML inlezen
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_file("kerstnet.xml");
    std::cout << "Load result: " << result.description() << ", mesh name: " << doc.child("mesh").attribute("name").value() << std::endl;

    //Knopen uitlezen
    vector<knoop> knopen = maakVector(doc);

    for (int i = 0; i < knopen.size(); i++)
    {
        cout << knopen[i].nummer << " - x: " << knopen[i].xCoord << " - y: " << knopen[i].yCoord << endl;
    }

    //Afstand berekenen voor elke knoop tussen elk paar knopen
    //Afstand tussen knoop i en knoop j
    vector<vector<float>> afstanden(knopen.size());

    for (int i = 0; i < afstanden.size(); i++)
    {
        afstanden[i].resize(afstanden.size());
        for (int j = 0; j < afstanden[i].size(); j++)
        {
            int xKwad = (knopen[i].xCoord - knopen[j].xCoord) * (knopen[i].xCoord - knopen[j].xCoord);
            int yKwad = (knopen[i].yCoord - knopen[j].yCoord) * (knopen[i].yCoord - knopen[j].yCoord);
            afstanden[i][j] = sqrt(xKwad + yKwad);
        }
    }

    //Bezocht vector aanmaken
    vector<bool> bezocht(knopen.size(), false);

    //Alle routes doorlopen en kleinste nemen
    float totaleAfstand = 0;
    int knoop = rand() % knopen.size();
    bezocht[knoop] = true;

    int i = 0;
    while (i < afstanden.size())
    {
        int minKnoop;
        for (int j = 0; j < bezocht.size(); j++)
        {
            if (!bezocht[j])
                minKnoop = j;
        }

        //Simulated annealing
        SimulatedAnnealingAfstand sad;
        int minKnoop = sad.oplossing();
        //
        // for (int j = 0; j < afstanden[knoop].size(); j++)
        // {
        //     if (i != j && !bezocht[j] && afstanden[knoop][minKnoop] > afstanden[knoop][j])
        //         minKnoop = j;
        // }
        cerr << afstanden[knoop][minKnoop] << "- " << minKnoop << endl;
        totaleAfstand += afstanden[knoop][minKnoop];
        bezocht[minKnoop] = true;
        knoop = minKnoop;
        i++;
    }

    cout << "Kortste afstand is " << totaleAfstand << endl;

    return 0;
}