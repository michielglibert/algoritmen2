#include "prquadtree.h"

int main()
{
	PRQuadtree tree(50);
	tree.voegToe(26, 10);
	tree.voegToe(51, 26);
	tree.voegToe(76, 77);
	tree.schrijf();

	return 0;
}
