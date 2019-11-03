#ifndef __BINOMIALHEAP_H
#define __BINOMIALHEAP_H

#include <iostream>
#include <vector>
#include <memory>
#include <fstream>
#include <sstream>
#include <cassert>

using namespace std;

template <class Data>
class Knoop;

template <class Data>
class BinomialTree : public unique_ptr<Knoop<Data>>
{
public:
    BinomialTree() = default;
    BinomialTree(unique_ptr<Knoop<Data>> knoop)
    {
        this->swap(knoop);
    };
    void voegDeelboomToe(BinomialTree<Data> &boom)
    {
        boom->broer = move((*this)->linkerkind);
        (*this)->linkerkind = move(boom);
    }
    int geefDiepte()
    {
        int diepte = 0;
        BinomialTree<Data> *kind = &((*this)->linkerkind);
        while (*kind)
        {
            kind = &((*kind)->linkerkind);
            diepte++;
        }
        return diepte;
    }

    vector<BinomialTree<Data> *> geefDeelbomen()
    {
        vector<BinomialTree<Data> *> deelbomen;
        BinomialTree<Data> *kind = &((*this)->linkerkind);
        deelbomen.push_back(kind);
        while ((*kind)->broer)
        {
            deelbomen.push_back(&((*kind)->broer));
            kind = &((*kind)->broer);
        }
        return deelbomen;
    }

    string tekenrec(ostream &uit, int &knoopteller);
};

template <class Data>
class BinomialHeap
{

public:
    vector<BinomialTree<Data>> bomen;

    void voegToe(const int &sleutel, const Data &data)
    {
        BinomialTree<Data> nieuwe = make_unique<Knoop<Data>>(sleutel, data);
        if (bomen.empty())
        {
            bomen.push_back(move(nieuwe));
        }
        else
        {
            int i = 0;
            while (bomen[i] != 0 && i < bomen.size())
            {
                this->samenVoegen(i, nieuwe);
                i++;
            }

            if (i == bomen.size())
            {
                bomen.push_back(move(nieuwe));
            }
            else
            {
                bomen[i] = move(nieuwe);
            }
        }
    }

    void voegBoomToe(BinomialTree<Data> &boom)
    {
        int diepte = boom.geefDiepte();

        while (diepte < bomen.size() && bomen[diepte] != 0)
        {
            this->samenVoegen(diepte, boom);
            diepte++;
        }

        if (diepte == bomen.size())
        {
            bomen.push_back(move(boom));
        }
        else
        {
            if (bomen.size() < diepte)
                bomen.resize(diepte + 1);
            bomen[diepte] = move(boom);
        }
    }

    void samenVoegen(const int index, BinomialTree<Data> &boom)
    {
        if (bomen[index]->sleutel < boom->sleutel)
        {
            bomen[index].voegDeelboomToe(boom);
            boom = move(bomen[index]);
        }
        else
        {
            boom.voegDeelboomToe(bomen[index]);
        }
    }

    void heapSamenvoegen(BinomialHeap<Data> &heap)
    {
        for (int i = 0; i < heap.bomen.size(); i++)
        {
            this->voegBoomToe(heap.bomen[i]);
        }
    }

    int geefEersteIndex()
    {
        int i = 0;
        while (bomen[i] == 0)
        {
            i++;
        }
        return i;
    }

    int popMinimum()
    {
        if (bomen.size() == 0)
            return 0;

        int minimum = bomen[this->geefEersteIndex()]->sleutel;
        int minimum_index;
        for (int i = 0; i < bomen.size(); i++)
        {
            if (bomen[i])
            {
                minimum = min(minimum, bomen[i]->sleutel);
                minimum_index = i;
            }
        }
        vector<BinomialTree<Data> *> deelbomen_ptrs = bomen[minimum_index].geefDeelbomen();
        vector<BinomialTree<Data>> tijdelijk(deelbomen_ptrs.size());
        for (int i = 0; i < deelbomen_ptrs.size(); i++)
        {
            tijdelijk[i] = move(*(deelbomen_ptrs[i]));
        }
        bomen[minimum_index].release();

        BinomialHeap nieuwe_heap;
        for (int i = 0; i < tijdelijk.size(); i++)
        {
            nieuwe_heap.voegBoomToe(tijdelijk[i]);
        }
        this->heapSamenvoegen(nieuwe_heap);

        return minimum;
    }

    void verwijder(int sleutel)
    {
        //TODO
    }

    void verminderPrioriteit(int sleutel, int aantal)
    {
        //TODO
    }

    void teken(const char *bestandsnaam)
    {
        ofstream uit(bestandsnaam);
        assert(uit);
        int knoopteller = 0; //knopen moeten een eigen nummer krijgen.
        uit << "digraph {\n";
        for (int i = 0; i < bomen.size(); i++)
        {
            bomen[i].tekenrec(uit, knoopteller);
        }
        uit << "}";
    }
};

template <class Data>
class Knoop
{
    friend class BinomialTree<Data>;

public:
    Knoop(const int &sleutel, const Data &data) : sleutel(sleutel), data(data) {}
    int sleutel;
    Data data;
    BinomialTree<Data> linkerkind;
    BinomialTree<Data> broer;
};

template <class Data>
string BinomialTree<Data>::tekenrec(ostream &uit, int &knoopteller)
{
    ostringstream wortelstring;
    wortelstring << '"' << ++knoopteller << '"';
    if (!*this)
    {
        uit << wortelstring.str() << " [shape=point];\n";
    }
    else
    {
        uit << wortelstring.str() << "[label=\"" << (*this)->sleutel << "\"]";
        uit << ";\n";
        BinomialTree<Data> *kind = &((*this)->linkerkind);
        while (*kind != nullptr)
        {
            string kindstring = kind->tekenrec(uit, knoopteller);
            uit << wortelstring.str() << " -> " << kindstring << ";\n";
            kind = &((*kind)->broer);
        }
        // string linkskind = (*this)->links.tekenrec(uit, knoopteller);
        // string rechtskind = (*this)->rechts.tekenrec(uit, knoopteller);
        // uit << wortelstring.str() << " -> " << linkskind << ";\n";
        // uit << wortelstring.str() << " -> " << rechtskind << ";\n";
    };
    return wortelstring.str();
}

#endif