#include <iostream>
#include <string.h>
#include <fstream>
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
	char ch;

	cout << "1. 單行輸入\n2. 讀檔輸入\n";
	do cout << "? ", cin >> ch;
	while (ch != '1' && ch != '2');

	if (ch == '1')
	{
		while (getline(cin, input))
		{
			oj.load(input, 0);
			oj.convert();
			oj.print();
			oj.print_detail();
		}
	}
	else
	{
		ifstream inFile("test.txt", ios::in);
		vector<vector<string>> data_set;
		int cnt = 0;

		// store all the data
		while (getline(inFile, input)) data_set.push_back(oj.load(input, ++cnt));

		for (auto& i : data_set)
		{
			oj.set(i);
			oj.convert();
			oj.print();
			oj.print_detail();
		}
	}

	return 0;
}