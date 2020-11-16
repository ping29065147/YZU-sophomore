#include "k_map.h"
#define variable 4

using namespace std;

int main()
{
	k_map mymap;

	mymap.readfile();
	
	for (int c = 1; c <= mymap.getnum(); ++c) {
				
		mymap.readdata();

		mymap.dealwithkmap(c);

		mymap.find_ones_dontcare();

		int times = variable;
		while (times--) mymap.compare();
		mymap.printcompare();		

		mymap.searchminterm();

		mymap.printresults();

		mymap.clear();
	}

	return 0;
}