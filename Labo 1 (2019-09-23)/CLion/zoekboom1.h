#ifndef __Zoekboom_H
#define __Zoekboom_H
#include <cstdlib>
#include <iostream>
#include <queue>
#include <memory>
#include <functional>
#include <fstream>
#include <sstream>
#include <cassert>

using std::ostream;
using std::ofstream;
using std::ostringstream;
using std::unique_ptr;
using std::cerr;
using std::pair;
using std::string;
using std::max;
/**********************************************************************

   Class: Zoekboom
   
   beschrijving: Binaire Zoekboom waarin duplicaatsleutels wel of niet zijn toegestaan.
   
***************************************************************************/

template <class Sleutel,class Data>
class zoekKnoop;

template <class Sleutel,class Data>
class Zoekboom : public unique_ptr<zoekKnoop<Sleutel,Data>>{
//....move en copy. Noot: als er geen copy nodig is, zet hem beste op delete.
public:
    void inorder(std::function<void(const zoekKnoop<Sleutel,Data>&)> bezoek) const;
    void schrijf(ostream& os) const;
    void teken(const char * bestandsnaam);
    string tekenrec(ostream& uit,int&knoopteller);

    Zoekboom() = default;
    //Copy constructor
    Zoekboom(unique_ptr<zoekKnoop<Sleutel,Data>> knoop) {
        this->swap(knoop);
    };
//    //Move constructor
//    Zoekboom(Zoekboom<Sleutel, Data> &&zoekboom) = delete;
//    //Copy operator
//    Zoekboom<Sleutel, Data> &operator=(const Zoekboom<Sleutel, Data> &zoekboom) = delete;
//    //Move operator
//    const Zoekboom &operator=(Zoekboom<Sleutel, Data> &&zoekboom) {
//        *this = std::move(zoekboom);
//    }


    //te implementeren
    bool repOK() const;
    int geefDiepte();
    // geefBoomBovenKnoop: gegeven een knooppointer, wele boom wijst naar de knoop
    // preconditie: knoop moet een naar een geldige knoop wijzen.
    Zoekboom<Sleutel, Data>* geefBoomBovenKnoop(zoekKnoop<Sleutel, Data>& knoopptr);
    void voegtoe(const Sleutel& sleutel,const Data& data,bool dubbelsToestaan=false);
    void roteerLinks(Sleutel sleutel);
    void roteerRechts(Sleutel sleutel);

protected:
//zoekfunctie zoekt sleutel en geeft de boom in waaronder de sleutel zit (eventueel een lege boom als de sleutel
//ontbreekt) en de pointer naar de ouder (als die bestaat, anders nulpointer).
//noot: alhoewel de functie niets verandert aan de boom is ze geen const functie.
    void zoek(const Sleutel& sleutel, zoekKnoop<Sleutel,Data>*& ouder, Zoekboom<Sleutel,Data>*& plaats);
};

template <class Sleutel,class Data>
class zoekKnoop {
    friend class Zoekboom<Sleutel,Data>;
public:
    zoekKnoop():ouder(0){}
    zoekKnoop(const Sleutel& sl, const Data& d):sleutel{sl},data(d),ouder(0){};
    zoekKnoop(Sleutel&& sl, Data&& d):sleutel{move(sl)},data(move(d)),ouder(0){};
    Zoekboom<Sleutel,Data>& geefKind(bool links);
    Sleutel sleutel;
    Data data;
    zoekKnoop<Sleutel,Data>* ouder;
    Zoekboom<Sleutel,Data> links,rechts;
};


/*****************************************************************************

    Implementatie
    
*****************************************************************************/


template <class Sleutel,class Data>
void Zoekboom<Sleutel,Data>::inorder(std::function<void(const zoekKnoop<Sleutel,Data>&)> bezoek) const{
    if (*this){
        (*this)->links.inorder(bezoek);
        bezoek(**this);
        (*this)->rechts.inorder(bezoek);
    };
}

template <class Sleutel,class Data>
void Zoekboom<Sleutel,Data>::schrijf(ostream& os) const{
    inorder([&os](const zoekKnoop<Sleutel,Data>& knoop){
        os<<"("<<knoop.sleutel<<" -> "<<knoop.data<<")";
        os<<"\n  Linkerkind: ";
        if (knoop.links)
            os<<knoop.links->sleutel;
        else
            os<<"-----";
        os<<"\n  Rechterkind: ";
        if (knoop.rechts)
            os<<knoop.rechts->sleutel;
        else
            os<<"-----";
        os<<"\n";
    });
}

template <class Sleutel,class Data>
void Zoekboom<Sleutel,Data>::teken(const char * bestandsnaam){
    ofstream uit(bestandsnaam);
    assert(uit);
    int knoopteller=0;//knopen moeten een eigen nummer krijgen.
    uit<<"digraph {\n";
    this->tekenrec(uit,knoopteller);
    uit<<"}";
}

template <class Sleutel,class Data>
string Zoekboom<Sleutel,Data>::tekenrec(ostream& uit,int&knoopteller ){
    ostringstream wortelstring;
    wortelstring<<'"'<<++knoopteller<<'"';
    if (!*this){
        uit<<wortelstring.str()<<" [shape=point];\n";
    }
    else{
        uit<<wortelstring.str()<<"[label=\""<<(*this)->sleutel<<":"<<(*this)->data<<"\"]";
        uit<<";\n";
        string linkskind=(*this)->links.tekenrec(uit,knoopteller);
        string rechtskind=(*this)->rechts.tekenrec(uit,knoopteller);
        uit<<wortelstring.str()<<" -> "<<linkskind<<";\n";
        uit<<wortelstring.str()<<" -> "<<rechtskind<<";\n";
    };
    return wortelstring.str();
}

template <class Sleutel,class Data>
Zoekboom<Sleutel, Data>* Zoekboom<Sleutel,Data>::geefBoomBovenKnoop(zoekKnoop<Sleutel ,Data>& knoop){
    if (knoop.ouder==0)
        return this;
    else
    if (knoop.ouder->links.get()==&knoop)
        return &(knoop.ouder->links);
    else
        return &(knoop.ouder->rechts);
}

template <class Sleutel,class Data>
void Zoekboom<Sleutel,Data>::voegtoe(const Sleutel& sleutel,const Data& data,bool dubbelsToestaan){
    zoekKnoop<Sleutel, Data>* ouder;
    Zoekboom<Sleutel, Data>* plaats;
    Zoekboom<Sleutel, Data>::zoek(sleutel,ouder,plaats);
    if (dubbelsToestaan)
        while (*plaats)
            (*plaats)->geefKind(rand()%2).zoek(sleutel,ouder,plaats);
    if (!*plaats){
        Zoekboom<Sleutel, Data> nieuw=
                std::make_unique<zoekKnoop<Sleutel, Data> >(sleutel,data);
        nieuw->ouder=ouder;
        *plaats=move(nieuw);
    }
}


template <class Sleutel,class Data>
void Zoekboom<Sleutel,Data>::zoek(const Sleutel& sleutel, zoekKnoop<Sleutel,Data>*& ouder, Zoekboom<Sleutel,Data>*& plaats){
    plaats=this;
    ouder=0;
    while (*plaats && (*plaats)->sleutel !=sleutel){
        ouder=plaats->get();
        if ((*plaats)->sleutel < sleutel)
            plaats=&(*plaats)->rechts;
        else
            plaats=&(*plaats)->links;
    };
}

template<class Sleutel, class Data>
int Zoekboom<Sleutel, Data>::geefDiepte() {
    int hoogte=0;
    // Diepte Eerst
    if(this) {
        int linkseDiepte = (*this)->links?(*this)->links.geefDiepte():0;
        int rechtsteDiepte = (*this)->rechts?(*this)->rechts.geefDiepte():0;

        return 1+max(linkseDiepte,rechtsteDiepte);
    }

    return 0;
}

template<class Sleutel, class Data>
void Zoekboom<Sleutel, Data>::roteerLinks(Sleutel sleutel) {
    zoekKnoop<Sleutel, Data>* ouder;
    Zoekboom<Sleutel, Data>* ouderBoom  = Zoekboom<Sleutel, Data>::geefBoomBovenKnoop(*ouder);
    Zoekboom<Sleutel, Data>* plaats;
    Zoekboom<Sleutel, Data>::zoek(sleutel,ouder,plaats);

    ouderBoom->teken("start");
    std::swap(ouderBoom,plaats);
    ouderBoom->teken("startna");
    std::cout << "-------------OUDER-------------" << std::endl;
    ouderBoom->schrijf(std::cout);


    unique_ptr<zoekKnoop<Sleutel,Data>> ptr = move((*ouderBoom)->rechts);
    (*ouderBoom)->rechts = move(plaats);
    ouderBoom->teken("resultaatvoor");
    (*ouderBoom)->rechts->links = move(ptr);
    ouderBoom->teken("resultaat");

//
//    ouderBoom->teken("ouder1");
//    std::cout << "-------------PLAATS-------------" << std::endl;
//    plaats->schrijf(std::cout);
//    plaats->teken("plaats1");
//    std::cout << "-------------RESULTAAT ROOT-------------" << std::endl;
//    (*plaats)->links = move((*ouderBoom)->rechts);
//    //std::swap(*plaats, ouder->rechts);
//    plaats->schrijf(std::cout);
//    ouderBoom->teken("ouder2");
//    std::cout << "-------------RESULTAAT OUDER-------------" << std::endl;
//    ouderBoom->schrijf(std::cout);
//    plaats->teken("plaats2");
//    std::cout << "--------------------------" << std::endl;
}

template<class Sleutel, class Data>
void Zoekboom<Sleutel, Data>::roteerRechts(Sleutel sleutel) {

};

template <class Sleutel,class Data>
Zoekboom<Sleutel,Data>& zoekKnoop<Sleutel,Data>:: geefKind(bool linkerkind){
    if (linkerkind)
        return links;
    else
        return rechts;
};


#endif