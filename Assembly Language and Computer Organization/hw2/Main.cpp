#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "predictor.h"

using namespace std;

predictor Predictor;
vector<string> V;
string str;
int x, y;

static auto fast_io = []
{
	ios::sync_with_stdio(false);
	cout.tie(nullptr);
	cin.tie(nullptr);
	return 0;
}();

int main()
{
	cout << "choose an input.\n";
	cout << "1. easy input\n";
	cout << "2. hard input\n: ";
	cin >> x;

	if (x == 1) Predictor.execute("TTTTNNTNTNTNTNTNT");
	else
	{
		ifstream inFile("input.txt");
		while (getline(inFile, str)) V.emplace_back(str);

		cout << "input entries: ";
		cin >> y;
		Predictor.execute(V, y);
	}
}