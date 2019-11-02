#include "graaf.h"
#include <string>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <stack>
#define BUREN 5000

// DEEL 1
void postorderDEZrec(int knoopnr, Graaf<GERICHT> &omgekeerd, std::stack<int> &postorder, std::vector<bool> &bezocht)
{
	bezocht[knoopnr] = true;

	for (auto it = omgekeerd[knoopnr].begin(); it != omgekeerd[knoopnr].end(); ++it)
	{
		if (!bezocht[it->first])
		{
			postorderDEZrec(it->first, omgekeerd, postorder, bezocht);
		}
	}
	postorder.push(knoopnr);
}

void postorderDEZ(Graaf<GERICHT> &omgekeerd, std::stack<int> &postorder)
{
	std::vector<bool> bezocht(omgekeerd.aantalKnopen());

	for (int i = 0; i < omgekeerd.aantalKnopen(); i++)
	{
		if (!bezocht[i])
		{
			postorderDEZrec(i, omgekeerd, postorder, bezocht);
		}
	}
}

void DEZmetstackrec(int knoopnr, Graaf<GERICHT> &graaf, std::vector<bool> &bezocht)
{
	bezocht[knoopnr] = true;

	for (auto it = graaf[knoopnr].begin(); it != graaf[knoopnr].end(); ++it)
	{
		if (!bezocht[it->first])
		{
			DEZmetstackrec(it->first, graaf, bezocht);
		}
	}
}

void DEZmetstack(Graaf<GERICHT> &graaf, std::stack<int> &postorder)
{
	std::cout << "----- DEEL 1 -----" << std::endl;
	std::vector<bool> bezocht(graaf.aantalKnopen());
	std::vector<int> gezinnen;
	int knoopnr, stofzuigers;

	stofzuigers = 0;
	while (!postorder.empty())
	{
		knoopnr = postorder.top();

		if (!bezocht[knoopnr])
		{
			std::cout << "Ik verkoop stofzuiger " << ++stofzuigers << " aan gezin " << knoopnr << std::endl;
			DEZmetstackrec(knoopnr, graaf, bezocht);
			gezinnen.push_back(knoopnr);
		}

		postorder.pop();
	}

	/*
	sort(gezinnen.begin(), gezinnen.end()); 
	for(int gezin : gezinnen){
		std::cout << gezin << ", ";
	}
	*/
	std::cout << "Ik kan " << stofzuigers << " stofzuigers verkopen!" << std::endl;
}

// DEEL 2
void DEZaantallossecomponenten(Graaf<GERICHT> &graaf, std::stack<int> &postorder)
{
	std::cout << "----- DEEL 2 -----" << std::endl;
	std::vector<bool> bezocht(graaf.aantalKnopen());
	int knoopnr, stofzuigers;

	stofzuigers = 0;
	while (!postorder.empty())
	{
		knoopnr = postorder.top();

		if (!bezocht[knoopnr])
		{
			stofzuigers++;
			std::cout << "Ik moet een stofzuiger verkopen aan gezin " << knoopnr << std::endl;
			DEZmetstackrec(knoopnr, graaf, bezocht);
		}

		postorder.pop();
	}

	std::cout << "Ik moet " << stofzuigers << " stofzuiger(s) verkopen!" << std::endl;
}

int main()
{
	// DEEL 1 -> sterk samenhangende componenten vinden (theorie p61)
	Graaf<GERICHT> graaf(BUREN);
	Graaf<GERICHT> omgekeerd(BUREN);
	Graaf<ONGERICHT> ongericht(BUREN);

	// buren inlezen
	std::ifstream invoer("rss.txt");
	std::string vuilbak;
	int subscriber, publisher;

	// graaf en omgekeerde graaf opstellen
	while (invoer >> vuilbak >> subscriber >> vuilbak >> vuilbak >> vuilbak >> vuilbak >> vuilbak >> publisher)
	{
		try
		{
			graaf.voegVerbindingToe(publisher, subscriber);
			omgekeerd.voegVerbindingToe(subscriber, publisher);
			ongericht.voegVerbindingToe(publisher, subscriber);
		}
		catch (GraafExceptie ex)
		{
			//std::cout << "Er is al een verbinding tussen " << publisher << " en " << subscriber << "!" << std::endl;
		}
	}

	// postorder nummering
	std::stack<int> postorder;

	postorderDEZ(omgekeerd, postorder);
	DEZmetstack(graaf, postorder);

	// DEEL 2 -> verbindingen tussen sterk samenhangende componenten vinden
	postorderDEZ(graaf, postorder);
	DEZaantallossecomponenten(graaf, postorder);

	return 0;
}