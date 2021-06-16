#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "Tomasulo.h"

using namespace std;

static auto fast_io = []
{
	ios::sync_with_stdio(false);
	cout.tie(nullptr);
	cin.tie(nullptr);
	return 0;
}();

int main()
{
	ifstream inFile("input.txt");
	vector<string> V;
	string str;

	while (getline(inFile, str)) V.emplace_back(str);
	Tomasulo cur(V);
}