#include <vector>
#define _USE_MATH_DEFINES
#include <math.h>
#include <stdlib.h>
#include "prquadtree.h"
#include "csv.h"
#define PUNTEN 131072

int main(){
	/*
	PRQuadtree tree(127);
	
	tree.voegToe(40, 45);
	tree.voegToe(70, 10);
	tree.voegToe(69, 50);
	tree.voegToe(15, 70);
	tree.voegToe(55, 80);
	tree.voegToe(80, 90);
	tree.schrijf();
	tree.teken("tree");
	
	std::cout << "Diepte: " << tree.geefDiepte() << std::endl;
	std::cout << "isAanwezig tests: " << std::endl;
	std::cout << "\t(40,45) : " << tree.isAanwezig(40, 45) << std::endl;
	std::cout << "\t(41,45) : " << tree.isAanwezig(41, 45) << std::endl;
	
	tree.tekenSvg("tree.svg");
	*/
	
	// csv
	CsvData grafiek("dieptedata",',');
	std::vector<double> dieptedata;
	
	PRQuadtree tree(10000 + 5 * PUNTEN);
	int x, y, diepte_check;
	
	diepte_check = 2;
	for(int i = 0; i <= PUNTEN; i++){
		if(i == diepte_check){
			std::cout << "Diepte na " << i << " knopen " << tree.geefDiepte() << std::endl;
			dieptedata.push_back(tree.geefDiepte());
			diepte_check *= 2;
		}
		x = (10000 - 5 * i) * cos(i * 0.25 * M_PI / 180.0);
		y = (10000 - 5 * i) * sin(i * 0.25 * M_PI / 180.0);
		tree.voegToe(x, y);	
	}
	tree.teken("tree");
	
	grafiek.voegDataToe(dieptedata);
	
	tree.tekenSvg("tree.svg");
	
	return 0;
}
