#ifndef __PRQUADTREE_H__
#define __PRQUADTREE_H__
#include <cassert>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <memory>
#include <functional>
#include <stack>
#include <fstream>
#include <sstream>
using std::unique_ptr;
#define SCALE 750

int max(int a, int b){ return(a < b ? b : a); }

/*****************************************************************************
    Bevat de headers en code voor een PRQuadtree
    en voor de abstracte klasse PRKnoop met deelklassen Blad en Nietblad
    
*****************************************************************************/

class PRKnoop;
typedef unique_ptr<PRKnoop> Knoopptr;
class PRQuadtree;

class PRQuadtree: public Knoopptr{
public:
    using Knoopptr::unique_ptr;
    
    PRQuadtree(int a):maxcoordinaat{a}{};
    PRQuadtree(Knoopptr&& a):Knoopptr(move(a)){};
    PRQuadtree& operator=(Knoopptr&& a){
        Knoopptr::operator=(move(a));
        return *this;
    };
    
    void preEnPostOrder(std::function<void(PRKnoop*)>& bezoekPre,std::function<void(PRKnoop*)>& bezoekPost) const;
    
    //te implementeren
    void voegToe(int x, int y);
    bool isAanwezig(int x, int y);
    int geefDiepte();
    void schrijf();
    void teken(const char *bestandsnaam);
    std::string tekenrec(std::ostream &uit, int &knoopteller);
    void tekenSvg(const char *bestandsnaam);
    void tekenSVGrec(std::ostream &uit, Knoopptr *knoop, int x_linksboven, int y_linksboven, int x_rechtsonder, int y_rechtsonder);
    
	//de PRquadtree kan alleen punten bevatten met
	//-maxcoordinaat <= x < maxcoordinaat
	//-maxcoordinaat <= y < maxcoordinaat
    int maxcoordinaat;
};

//Opmerking: om de functies specifiek aan een deelklasse te kunnen gebruiken moet je soms een
//static_cast doen, zoals in
//  PRKnoop* knoopptr=...;
//  if (knoopptr!=nullptr && knoopptr->isBlad() && static_cast<PRBlad*>(knoopptr)->x == 5)
class PRKnoop{
public:
    virtual bool isBlad()=0;
    virtual int geefDiepte()=0;
    virtual void schrijf(int diepte) = 0;
};

class PRBlad:public PRKnoop{
public:
    PRBlad(int x,int y):x{x},y{y}{};
    virtual bool isBlad(){ return true;}
    int x,y;//co"ordinaten punt
    virtual int geefDiepte(){
        return 1;
    };
    virtual void schrijf(int diepte){
    	for(int i = 0; i < diepte; i++) std::cout << "\t";
		std::cout << "Blad met coordinaten " << this->x << " en " << this->y << std::endl;
	}
};

class PRNietblad:public PRKnoop{
public:
	PRNietblad(){};
    virtual bool isBlad(){ return false;}
    int geefAantalKinderen(){
         int aantal=0;
         for (int i=0; i<4; i++ ){
             if (kind[i])
                ++aantal;
         }
         return aantal;
    };
    //xc, yc: co"ordinaten van het centrum van het gebied
    Knoopptr* geefKind(int x, int y, int xc, int yc){
        int xindex=(x<=xc? WEST : OOST);
        int yindex=(y<=yc? NOORD : ZUID);
        return &kind[xindex+yindex];
    };
    int geefIndex(int x, int y, int xc, int yc){
		int xindex=(x<xc? WEST : OOST);
        int yindex=(y<yc? NOORD : ZUID);
        return xindex + yindex;
	};
    static const int OOST, WEST, NOORD, ZUID;
    Knoopptr kind[4];//indexeren met windrichting (bv. kind[NOORD+WEST] bevat punten
                       //met x en y kleiner dan grenswaarde)
                       //leeg gebied: nulpointer
    // ----                  
	virtual void schrijf(int diepte){
    	for(int i = 0; i < diepte; i++) std::cout << "\t";
		std::cout << "Intermediaire knoop met " << this->geefAantalKinderen() << " kinderen" << std::endl;
		for(int i = 0; i < 4; i++){
			if(kind[i]) kind[i]->schrijf(diepte + 1);
		}
	}
	virtual int geefDiepte(){
		int diepte = 1;
		for(int i = 0; i < 4; i++){
			if(kind[i]) diepte = max(diepte, 1 + (*kind[i]).geefDiepte()); 
		}
		return diepte;
	}
	// ----
};

// ----------
void PRQuadtree::schrijf(){
	if(*this) (*this)->schrijf(0);
}

int PRQuadtree::geefDiepte(){
	if(*this) return (*this)->geefDiepte();
	else return 0;
}

bool PRQuadtree::isAanwezig(int x, int y){
	Knoopptr *knoop = &(*this);
	int xc, yc, x_linksboven, y_linksboven, x_rechtsonder, y_rechtsonder;
	
	// in plaats van center bijhouden moet je de coords van de linkerbovenhoek en de rechteronderhoek bijhouden en telkens het center berekenen
	// als een coord <= is aan het center ga je links anders rechts	
	x_linksboven = y_linksboven = (-1) * this->maxcoordinaat;
	x_rechtsonder = y_rechtsonder = this->maxcoordinaat;
	
	// zolang knoop geen nullpointer, geen blad en opsplitsbaar is
	while(*knoop && !(*knoop)->isBlad() && (y_linksboven < y_rechtsonder || x_linksboven < x_rechtsonder)){
		// center berekenen
		yc = y_linksboven + ((y_rechtsonder - y_linksboven) / 2);
		xc = x_linksboven + ((x_rechtsonder - x_linksboven) / 2);
		
		knoop = static_cast<PRNietblad*>(&(**knoop))->geefKind(x, y, xc, yc);
		
		// bepalen naar welk kind je gaat
		if(x <= xc && y <= yc){		    // linksboven
			x_rechtsonder = xc;
			y_rechtsonder = yc;
		} else if(xc < x && y <= yc){   // rechtsboven
			x_linksboven = xc + 1;
			y_rechtsonder = yc;
		} else if(x <= xc && yc < y){   // linksonder
			x_rechtsonder = xc;
			y_linksboven = yc + 1;
		} else {					 	// rechtsonder
			x_linksboven = xc + 1;
			y_linksboven = yc + 1;
		}
	}
	
	// als het een blad is moet je de x en y coordinaten vergelijken
	return(*knoop && static_cast<PRBlad *>(&(**knoop))->x == x  && static_cast<PRBlad *>(&(**knoop))->y == y);
}

void PRQuadtree::voegToe(int x, int y){
	Knoopptr *knoop = &(*this);
	int xc, yc, x_linksboven, y_linksboven, x_rechtsonder, y_rechtsonder;
	//std::cout << "voegToe " << x << ", " << y << std::endl;

	// in plaats van center bijhouden moet je de coords van de linkerbovenhoek en de rechteronderhoek bijhouden en telkens het center berekenen
	// als een coord <= is aan het center ga je links anders rechts	
	x_linksboven = y_linksboven = (-1) * this->maxcoordinaat;
	x_rechtsonder = y_rechtsonder = this->maxcoordinaat;
	
	// zolang knoop geen nullpointer, geen blad en opsplitsbaar is
	while(*knoop && !(*knoop)->isBlad() && (y_linksboven < y_rechtsonder || x_linksboven < x_rechtsonder)){
		// center berekenen
		yc = y_linksboven + ((y_rechtsonder - y_linksboven) / 2);
		xc = x_linksboven + ((x_rechtsonder - x_linksboven) / 2);
		
		//std::cout << "center van (" << x_linksboven << ", " << y_linksboven << ")";
		//std::cout << "-> (" << x_rechtsonder << ", " << y_rechtsonder << ")";
		//std::cout << " = (" << xc << ", " << yc << ") " << std::endl;
		
		knoop = static_cast<PRNietblad*>(&(**knoop))->geefKind(x, y, xc, yc);
		
		// bepalen naar welk kind je gaat
		if(x <= xc && y <= yc){		    // linksboven
			x_rechtsonder = xc;
			y_rechtsonder = yc;
		} else if(xc < x && y <= yc){   // rechtsboven
			x_linksboven = xc + 1;
			y_rechtsonder = yc;
		} else if(x <= xc && yc < y){   // linksonder
			x_rechtsonder = xc;
			y_linksboven = yc + 1;
		} else {					 	// rechtsonder
			x_linksboven = xc + 1;
			y_linksboven = yc + 1;
		}
	}
	
	// als knoop een blad is moet je het vervangen door een nietblad en de bladeren opnieuw toevoegen
	if(*knoop && (*knoop)->isBlad()){
		// TODO: niet recursief!
		PRBlad *blad = static_cast<PRBlad*>(&(**knoop));
		
		int blad_x = blad->x;
		int blad_y = blad->y;
		
		if(blad_x != x || blad_y != y){
			/*
			*knoop = std::make_unique<PRNietblad>();
			this->voegToe(blad_x, blad_y);
			this->voegToe(x, y);
			*/
			
			Knoopptr *bladKnoop = knoop;
			while(knoop == bladKnoop){
				// splits op
				*knoop = std::make_unique<PRNietblad>();
				
				// center herberekenen
				yc = y_linksboven + ((y_rechtsonder - y_linksboven) / 2);
				xc = x_linksboven + ((x_rechtsonder - x_linksboven) / 2);
				
				// bepalen naar welk kind je gaat
				if(x <= xc && y <= yc){		    // linksboven
					x_rechtsonder = xc;
					y_rechtsonder = yc;
				} else if(xc < x && y <= yc){   // rechtsboven
					x_linksboven = xc + 1;
					y_rechtsonder = yc;
				} else if(x <= xc && yc < y){   // linksonder
					x_rechtsonder = xc;
					y_linksboven = yc + 1;
				} else {					 	// rechtsonder
					x_linksboven = xc + 1;
					y_linksboven = yc + 1;
				}
				
				// bepalen naar welk kind knoop en bladknoop gaan
				bladKnoop = static_cast<PRNietblad*>(&(**knoop))->geefKind(blad_x, blad_y, xc, yc);
				knoop = static_cast<PRNietblad*>(&(**knoop))->geefKind(x, y, xc, yc);
			}
			// eenmaal knoop en bladknoop zich in verschillende regios bevinden kan je ze daar als blad toevoegen
			*bladKnoop = std::make_unique<PRBlad>(PRBlad(blad_x, blad_y));
			*knoop = std::make_unique<PRBlad>(PRBlad(x, y));
		}
	} else {
		// als knoop een nullpointer is kan je gewoon overschrijven met een nieuw blad
		*knoop = std::make_unique<PRBlad>(PRBlad(x, y));
	}
}

void PRQuadtree::tekenSvg(const char *bestandsnaam){
	std::ofstream uit(bestandsnaam);
	assert(uit);
	
	uit << "<?xml version=\"1.0\" standalone=\"no\"?>" << std::endl;
	uit << "<svg width=\"" << SCALE << "\" height=\"" << SCALE << "\" version=\"1.1\" xmlns=\"http://www.w3.org/2000/svg\">" << std::endl;
	uit << "\t<g>" << std::endl;
	
	Knoopptr *knoop = &(*this);
	int x_linksboven, y_linksboven, x_rechtsonder, y_rechtsonder;
	
	x_linksboven = y_linksboven = (-1) * this->maxcoordinaat;
	x_rechtsonder = y_rechtsonder = this->maxcoordinaat;
	
	tekenSVGrec(uit, &(*this), x_linksboven, y_linksboven, x_rechtsonder, y_rechtsonder);
	
	uit << "\t</g>" << std::endl << "</svg>" << std::endl;
}

void PRQuadtree::tekenSVGrec(std::ostream &uit, Knoopptr *knoop, int x_linksboven, int y_linksboven, int x_rechtsonder, int y_rechtsonder){
	if(*knoop){
		double x = double(x_linksboven + this->maxcoordinaat) / (2 * this->maxcoordinaat) * SCALE;
		double y = double(y_linksboven + this->maxcoordinaat) / (2 * this->maxcoordinaat) * SCALE;
		double width = double(x_rechtsonder - x_linksboven) / (2 * this->maxcoordinaat) * SCALE;
		double height = double(y_rechtsonder - y_linksboven) / (2 * this->maxcoordinaat) * SCALE;
		uit << "\t\t<rect x=\"" << x << "\" y=\"" << y << "\" ";
		uit << "width=\"" << width << "\" height=\"" << height << "\" style=\"stroke:black;stroke-width:1;fill:white;\"/>" << std::endl;
	
		if(!(*knoop)->isBlad()){
			PRNietblad *nietBlad = static_cast<PRNietblad*>(&(**knoop));
			
			// center berekenen
			int yc = y_linksboven + ((y_rechtsonder - y_linksboven) / 2);
			int xc = x_linksboven + ((x_rechtsonder - x_linksboven) / 2);
			
			// kinderen
			if(nietBlad->kind[1]) this->tekenSVGrec(uit, &(nietBlad->kind[1]), x_linksboven, y_linksboven, xc, yc); 	 	  // linksboven
			if(nietBlad->kind[0]) this->tekenSVGrec(uit, &(nietBlad->kind[0]), xc + 1, y_linksboven, x_rechtsonder, yc); 	  // rechtsboven
			if(nietBlad->kind[3]) this->tekenSVGrec(uit, &(nietBlad->kind[3]), x_linksboven, yc + 1, xc, y_rechtsonder); 	  // linksonder 
			if(nietBlad->kind[2]) this->tekenSVGrec(uit, &(nietBlad->kind[2]), xc + 1, yc + 1, x_rechtsonder, y_rechtsonder); // rechtsonder
		}
	}
}

void PRQuadtree::teken(const char *bestandsnaam)
{
    std::ofstream uit(bestandsnaam);
    assert(uit);
    int knoopteller = 0; //knopen moeten een eigen nummer krijgen.
    uit << "digraph {\nnode[label=\"\"]\n";
    this->tekenrec(uit, knoopteller);
    uit << "}";
}

std::string PRQuadtree::tekenrec(std::ostream &uit, int &knoopteller)
{
    std::ostringstream wortelstring;
    wortelstring << '"' << ++knoopteller << '"';
    if (!*this){
        uit << wortelstring.str() << " [shape=point];\n";
    } else {
        if (*this && (*this)->isBlad()) {
            PRBlad *blad = static_cast<PRBlad *>(this->get());
            uit << wortelstring.str() << "[label=\"(" << blad->x << "," << blad->y << ")\"]";
            uit << ";\n";
        } else {
            PRNietblad *blad = static_cast<PRNietblad *>(this->get());
            for (int i = 0; i < 4; i++){
                if (blad->kind[i]){
                    PRQuadtree *tree = static_cast<PRQuadtree *>(&(blad->kind[i]));
                    std::string kind = tree->tekenrec(uit, knoopteller);
                    uit << wortelstring.str() << " -> " << kind << ";\n";
                }
            }
        }
    };
    return wortelstring.str();
}

// ----------

void PRQuadtree::preEnPostOrder(std::function<void(PRKnoop*)>& bezoekPre,std::function<void(PRKnoop*)>& bezoekPost) const{
        enum staat {pre,post};
        std::stack<std::pair<PRKnoop*,staat>> DEstack;//bevat alleen niet-nulpointers
        if (*this)
            DEstack.emplace(this->get(),pre);
        while (!DEstack.empty()){
            // --- C++17 specifiek
			//auto [nuknoop,nustaat]=DEstack.top();
            // --- C++14 equivalent
            auto dummy = DEstack.top();
            auto nuknoop = dummy.first;
            auto nustaat = dummy.second;
            // ---
            if (nustaat==pre){
                bezoekPre(nuknoop);
                DEstack.top().second=post;
                if (!nuknoop->isBlad()){
                    for (int i=0; i<4; i++ ){
                        Knoopptr* kind=&(static_cast<PRNietblad*>(nuknoop)->kind[i]);
                        if (*kind)
                            DEstack.emplace(kind->get(),pre);
                    };
                };
            } else{
                bezoekPost(nuknoop);
                DEstack.pop();
            };
        };

    };

const int PRNietblad::OOST=0;
const int PRNietblad::WEST=1;
const int PRNietblad::NOORD=0;
const int PRNietblad::ZUID=2;
#endif

