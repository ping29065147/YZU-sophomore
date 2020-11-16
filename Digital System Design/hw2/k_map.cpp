#include "k_map.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#define variable 4

using namespace std;

//fast io
k_map::k_map()
{
	ios::sync_with_stdio(false);
	cin.tie(nullptr);
	cout.tie(nullptr);
}

//read k-map from file and store it in square
void k_map::readfile()
{
	inFile.open("Case1", ios::in);
	if (!inFile) cout << "File doesn't exist.", exit(1);

	inFile >> num;
}

//return the number of cases
int k_map::getnum()
{
	return num;
}

void k_map::readdata()
{
	square.emplace_back(-1);
	for (size_t i = 0; i < variable * variable; ++i) inFile >> input, square.emplace_back(input);
}

//swap the data in the right position
// m0  m1  m3  m2
// m4  m5  m7  m6
// m12 m13 m15 m16
// m8  m9  m11 m10
void k_map::dealwithkmap(int c)
{
	printkmap(c);
	
	for (size_t i = 1; i < square.size(); ++i) {		
		if (!(i & 3)) swap(square[i], square[i - 1]);
		if (!(i & 15))
			for (size_t j = 0; j < 4; ++j) swap(square[i - j], square[i - 4 - j]);
	}
}

//print the k-map
void k_map::printkmap(int c)
{
	cout << "\n#" << c << "\n\nk-map:\n";
	for (size_t i = 1; i < square.size(); ++i) {
		cout << square[i] << " ";
		if (!(i & 3)) cout << "\n";
	}
}

//store both data's binary number in m
//store ones compliment in ones
void k_map::find_ones_dontcare()
{	
	for (size_t i = 1; i < square.size(); ++i)
		if (square[i] != 0) {

			pair< vector<int>, bool > temp;
			temp.second = false;

			for (size_t cpy = i - 1, j = 0; j < 4; ++j)
				if (cpy == 0) temp.first.emplace_back(0);
				else temp.first.emplace_back(cpy & 1), cpy >>= 1;

			m.emplace_back(temp.first);
			if (square[i] == 1) ones.emplace_back(temp);
		}

	print_ones_dontcare();
}

//print k-map's ones compliment and don't care
//print binary numbers of ones and don't care
void k_map::print_ones_dontcare()
{
	cout << "\nones: ";
	for (size_t i = 1; i < square.size(); ++i)
		if (square[i] == 1) cout << i - 1 << " ";
	
	cout << "\ndon't care: ";
	for (size_t i = 1; i < square.size(); ++i)
		if (square[i] == 2) cout << i - 1 << " ";

	cout << "\n\nbinary numbers of ones and don't care:\n";
	for (size_t i = 0; i < m.size(); ++i) {
		for (int j = m[i].size() - 1; j >= 0; --j)
			cout << m[i][j] << " ";
		cout << "\n";
	}
}

//compare if the ones and don't care can combine
void k_map::compare()
{
	//store the combination of the data in m by save
	vector< vector<int> > save;

	//check if the data in m can combine
	vector<bool> checked;

	for (size_t i = 0; i < m.size(); ++i) checked.emplace_back(false);

	for (size_t i = 0; i < m.size() - 1; ++i) {
		for (size_t j = i + 1; j < m.size(); ++j) {

			vector<int> temp;

			for (size_t k = 0; k < variable; ++k) {
				if (m[i][k] == m[j][k]) temp.emplace_back(m[i][k]);
				else temp.emplace_back(-2);
			}
			for (size_t k = 0, count = 4; k < variable; ++k) {
				if (temp[k] != -2) --count;
				if (count == 1) {
					checked[i] = true, checked[j] = true;
					for (size_t l = 0; l < variable; ++l)
						if (temp[l] == -2) temp[l] = 2;
					save.emplace_back(temp);
					break;
				}
			}
		}
	}
	for (size_t i = 0; i < m.size(); ++i) {
		if (checked[i] == false)
			save.emplace_back(m[i]);
	}

	m.clear();
	m = save;
}

//print the result after compare
void k_map::printcompare()
{
	cout << "\nafter compare:\n";
	for (size_t i = 0; i < m.size(); ++i) {
		for (int j = m[i].size() - 1; j >= 0; --j)
			cout << m[i][j] << " ";
		cout << "\n";
	}
}

//use dfs to search minterm of the result after compare
void k_map::searchminterm()
{
	for (size_t i = 1; i <= m.size(); ++i)
		dfs(m, ones, 0, m.size() - 1, 0, i, string());
}

//find all the minterm
void k_map::dfs(vector< vector<int> > data, vector< pair< vector<int>, bool > > ones,
	int start, int end, int index, int r, string re)
{
	if (index == r) {
		size_t i = 0;
		for (; i < ones.size(); ++i)
			if (ones[i].second != true) break;
		if (i == ones.size()) final_result.emplace_back(re);

		return;
	}

	for (int i = start; i <= end && end - i + 1 >= r - index; i++) {

		auto temp = ones;

		for (size_t j = 0; j < ones.size(); ++j) {

			size_t k = 0;

			for (; k < ones[j].first.size(); ++k)
				if (!(data[i][k] == 2 ||
					data[i][k] == temp[j].first[k])) break;
			if (k == temp[j].first.size()) temp[j].second = true;
		}

		string tmp;
		stringstream ss;
		ss << i;
		ss >> tmp;
		dfs(data, temp, i + 1, end, index + 1, r, re + tmp);
	}
}

//print the final results
void k_map::printresults()
{
	int min = final_result[0].size();
	for (auto& i : final_result)
		if (i.size() < min) min = i.size();

	cout << "\nresults:\n";
	for (size_t i = 0; i < final_result.size(); ++i) {
		if (final_result[i].size() == min) {
			for (size_t j = 0; j < final_result[i].size(); ++j) {
				for (int k = m[final_result[i][j] - '0'].size() - 1; k >= 0; k--)
					cout << m[final_result[i][j] - '0'][k] << " ";
				cout << "\n";
			}
			cout << "\n";
		}
	}
}

void k_map::clear()
{
	square.clear();
	m.clear();
	ones.clear();
	final_result.clear();
}