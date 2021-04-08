#include <iostream>
#include <string.h>
#include "RISC_V.h"

using namespace std;

int main()
{
	// fast io
	ios::sync_with_stdio(false);
	cout.tie(nullptr);
	cin.tie(nullptr);

	string input;
	risc_v oj;

	while (getline(cin, input))
	{
		oj.load(input);
		oj.convert();
		oj.print();
		oj.print_detail();
	}

	return 0;
}