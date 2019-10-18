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
using std::unique_ptr;

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
    int geefDiepte();
    void schrijf();
    
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
        int xindex=(x<xc? WEST : OOST);
        int yindex=(y<yc? NOORD : ZUID);
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
		return 1;
	}
	// ----
};

// ----------
void PRQuadtree::schrijf(){
	if(*this) (*this)->schrijf(0);
}

void PRQuadtree::voegToe(int x, int y){
	Knoopptr *knoop = &(*this);
	int range, xc, yc;
	
	range = this->maxcoordinaat; xc = yc = 0;
	
	// zolang knoop geen nullpointer of blad is
	while(*knoop && !(*knoop)->isBlad()){
		knoop = static_cast<PRNietblad*>(&(**knoop))->geefKind(x, y, xc, yc);
		
		range /= 2;
       	xc = x < xc ? xc - range : xc + range;
        yc = y < yc ? yc - range : yc + range;
	}
	
	// als knoop een blad is moet je het vervangen door een nietblad en de bladeren opnieuw toevoegen
	if(*knoop && (*knoop)->isBlad()){
		PRBlad *blad = static_cast<PRBlad*>(&(**knoop));
		
		int blad_x = blad->x;
		int blad_y = blad->y;
		
		*knoop = std::make_unique<PRNietblad>();
		this->voegToe(blad_x, blad_y);
		this->voegToe(x, y);
	} else {
		// als je een nullpointer berijkt hebt kan je die gewoon overschrijven met een nieuw blad
		*knoop = std::make_unique<PRBlad>(PRBlad(x, y));
	}
}

/*
void PRQuadtree::voegToe(int x, int y){
	PRKnoop *knoop = &(**this);
	int range, xc, yc;
	
	range = this->maxcoordinaat; xc = yc = 0;
	
	// zolang knoop geen nullpointer of blad is
	while(knoop && !knoop->isBlad()){
		knoop = &(**(static_cast<PRNietblad*>(knoop)->geefKind(x, y, xc, yc)));
		
		range /= 2;
       	xc = x < xc ? xc - range : xc + range;
        yc = y < yc ? yc - range : yc + range;
	}
	
	// als knoop een blad is moet je het vervangen door een nietblad en de bladeren opnieuw toevoegen
	if(knoop && knoop->isBlad()){
		PRBlad *blad = static_cast<PRBlad*>(knoop);
		
		int blad_x = blad->x;
		int blad_y = blad->y;
		
		std::swap(knoop, &(std::make_unique<PRNietblad>()));
		this->voegToe(blad_x, blad_y);
		this->voegToe(x, y);
	} else {
		// als je een nullpointer berijkt hebt kan je die gewoon overschrijven met een nieuw blad
		std::swap(knoop, std::make_unique<PRBlad>(PRBlad(x, y)));
	}
}
*/

/*
void PRQuadtree::voegToe(int x, int y){
	if((*this) == nullptr){
		std::cout << "root is leeg" << std::endl;
		
		// als de root leeg is vul je het in met het nieuwe punt
		Knoopptr ptr = std::make_unique<PRBlad>(PRBlad(x, y));
		this->swap(ptr);
	} else if((*this)->isBlad()){
		std::cout << "root is een blad" << std::endl;
		
		// als de root een blad is moet je het vervangen door een nietblad en de bladeren opnieuw toevoegen
		PRBlad *blad = static_cast<PRBlad*>(&(**this));
		int blad_x = blad->x;
		int blad_y = blad->y;
		
		Knoopptr ptr = std::make_unique<PRNietblad>();
		this->swap(ptr);
		
		// TODO: originele blad opnieuw toevoegen alsook het nieuwe blad
		this->voegToe(blad_x, blad_y);
		this->voegToe(x, y);
	} else{
		std::cout << "root is een intermediaire knoop" << std::endl;
		
		// als de root een intermediare knoop is moet je blijven zoeken totdat een blad of nullpointer gevonden word
		int xc, yc, maximum;
		
		maximum = this->maxcoordinaat;
		xc = yc = 0;
		
		Knoopptr *knoop = static_cast<PRNietblad*>(&(**this))->geefKind(x, y, xc, yc);
		
		while(*knoop && !(*knoop)->isBlad()){
			std::cout << "In de while" << std::endl;
			knoop = static_cast<PRNietblad*>(&(**knoop))->geefKind(x, y, xc, yc);
			
	        maximum /= 2;
	        xc = x < xc ? xc - maximum : xc + maximum;
	        yc = y < yc ? yc - maximum : yc + maximum;
		}
		
		// als de gevonden knoop een blad is moet je blijven opsplitsen tot de nieuwe knoop en de gevonden knoop zich in verschillende regios bevinden
		if(*knoop && (*knoop)->isBlad()){
			PRBlad *blad = static_cast<PRBlad*>(&(**knoop));
			int blad_x = blad->x;
			int blad_y = blad->y;
			
			*knoop = std::make_unique<PRNietblad>();
			this->voegToe(blad_x, blad_y);
			this->voegToe(x, y);
		} 
		
		// knoop is een nullpointer dus die kan je overschrijven
		*knoop = std::make_unique<PRBlad>(PRBlad(x, y));
	}
}
*/
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

