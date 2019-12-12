#include <iostream>
#include <fstream>
#include <vector>
#include <stack>
#include "graaf.h"

using std::cout;
using std::endl;
using std::string;
using std::ifstream;
using std::vector;
using std::stack;

#define AANTAL_GEZINNEN		5000

void readFile(string fileName, Graaf<GERICHT> &graaf, Graaf<GERICHT> &omgekeerd) {
	ifstream file(fileName);
	if(!file.is_open()) {
		cout << "Unable to open file '" << fileName << "'." << endl;
		return;
	}

	string s;
	int from;
	int to;
	while(file >> s >> from >> s  >> s  >> s  >> s  >> s >> to) {
		try {
			graaf.voegVerbindingToe(from, to);
			omgekeerd.voegVerbindingToe(to, from);
		} catch(GraafExceptie &e) {
			// cout << "[Warning]: " << e << "." << endl;
		}
	}
	file.close();
}

void behandelKnoop(Graaf<GERICHT> &graaf, int knoop, vector<bool> &behandeld, stack<int> &postorder) {
	// Markeer dat de knoop behandeld is
	behandeld[knoop] = true;

	// Behandel elke buur van deze knoop
	for(auto it = graaf[knoop].begin(); it != graaf[knoop].end(); it++) {
		if(!behandeld[it->first]) {
			behandelKnoop(graaf, it->first, behandeld, postorder);
		}
	}

	// Geef de knoop een postordernummer
	postorder.push(knoop);
}

void dez(Graaf<GERICHT> &graaf, stack<int> &postorder) {
	// Vector om bij te houden of een knoop al behandeld is
	int aantal = graaf.aantalKnopen();
	vector<bool> behandeld(aantal);

	// Behandel elke knoop
	for(int knoop = 0; knoop < aantal; knoop++) {
		if(!behandeld[knoop]) {
			behandelKnoop(graaf, knoop, behandeld, postorder);
		}
	}
}

void behandelKnoop2(Graaf<GERICHT> &graaf, int knoop, vector<bool> &behandeld) {
	// Markeer dat de knoop behandeld is
	behandeld[knoop] = true;

	// Behandel elke buur van deze knoop
	for(auto it = graaf[knoop].begin(); it != graaf[knoop].end(); it++) {
		if(!behandeld[it->first]) {
			behandelKnoop2(graaf, it->first, behandeld);
		}
	}
}

void dez2(Graaf<GERICHT> &graaf, stack<int> &postorder) {
	cout << "Verkoopvolgorde:" << endl;

	// Vector om bij te houden of een knoop al behandeld is
	vector<bool> behandeld(graaf.aantalKnopen());

	// Counter om het aantal componenten en dus het aantal verkochte stofzuigers bij te houden
	int aantal = 0;

	// Behandel telkens de resterende nog-niet-behandelde knoop met het hoogste postordernummer
	while(!postorder.empty()) {
		int knoop = postorder.top();
		postorder.pop();
		if(!behandeld[knoop]) {
			behandelKnoop2(graaf, knoop, behandeld);

			// Incrementeer het aantal componenten
			aantal++;

			// Print het resultaat uit (een diepte-eerst bos waarvan de bomen de gezochte sterk samenhangende componenten zijn)
			cout << knoop << endl;
		}
	}

	cout << "Aantal stofzuigers " << aantal << "." << endl;
}


int main() {
	// Stel de graaf en omgekeerde graaf op
	Graaf<GERICHT> graaf(AANTAL_GEZINNEN);
	Graaf<GERICHT> omgekeerd(AANTAL_GEZINNEN);
	readFile("rss.txt", graaf, omgekeerd);
	
	// Stack die de postordernummering van de knopen bijhoudt (grootste nummer zit bovenaan de stack)
	stack<int> postorder;

	// Pas diepte-eerst-zoeken toe op de omgekeerde graaf, waarbij de knopen in post-order genummerd worden (hoe groter het nummer, hoe later de knoop werd afgewerkt)
	dez(omgekeerd, postorder);

	// Pas diepte-eerst-zoeken toe op de oorspronkelijke graaf, waarbij als startknoop telkens de resterende nog-niet-behandelde knoop met het hoogste postordernummer wordt gebruikt
	dez2(graaf, postorder);
	
	// Graaf<GERICHT> g(6);
	// g.voegVerbindingToe(0, 1);
	// g.voegVerbindingToe(1, 2);
	// g.voegVerbindingToe(0, 2);
	// g.voegVerbindingToe(2, 0);
	// g.voegVerbindingToe(1, 3);
	// g.voegVerbindingToe(3, 4);
	// g.voegVerbindingToe(4, 5);
	// g.voegVerbindingToe(3, 5);
	// g.voegVerbindingToe(5, 3);
	// Graaf<GERICHT> o(6);
	// o.voegVerbindingToe(1, 0);
	// o.voegVerbindingToe(2, 1);
	// o.voegVerbindingToe(2, 0);
	// o.voegVerbindingToe(0, 2);
	// o.voegVerbindingToe(3, 1);
	// o.voegVerbindingToe(4, 3);
	// o.voegVerbindingToe(5, 4);
	// o.voegVerbindingToe(5, 3);
	// o.voegVerbindingToe(3, 5);
	// stack<int> postorder;
	// dez(o, postorder);
	// dez2(g, postorder);
	return 0;
}

// cd "/mnt/c/Users/Wout/Desktop/Vakken/Semester 1/Algoritmen II/Labo/Reeks 5"
// g++ -std=c++17 main.cpp -o main

// TODO extra opgave