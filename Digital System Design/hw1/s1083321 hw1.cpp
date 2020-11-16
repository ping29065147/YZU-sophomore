#include <iostream>
#include <string>
#include <unordered_map>
#include <sstream>
#include <vector>

using namespace std;

void build_21(unordered_map<char, int>& dic);
void build_10(unordered_map<char, int>& dic);

void convert_to_21(string val, unordered_map<char, int> dic);
void convert_to_10(string val, unordered_map<char, int> dic);

int main()
{
	unordered_map<char, int> dict21;
	unordered_map<char, int> dict10;
	build_21(dict21);
	build_10(dict10);

	char mode;
	string value;

	while (1) {
		cout << "\nr = 21\n";
		cout << "Please choose a mode:\n";
		cout << "0 -> Mode 0\n1 -> Mode 1\n";
		do {
			cout << "? : ";
			cin >> mode;
		} while (mode != '0' && mode != '1');
		cout << "Input a value: ";
		cin >> value;

		if (mode == '0')
			convert_to_21(value, dict21);
		else
			convert_to_10(value, dict10);
	}

	return 0;
}

void build_21(unordered_map<char, int>& dic)
{
	for (int i = 0; i < 10; i++)
		dic[i] = i + '0';
	for (int i = 0; i < 11; i++)
		dic[i + 10] = i + 'A';
}

void build_10(unordered_map<char, int>& dic)
{
	for (int i = 0; i < 10; i++)
		dic['0' + i] = i;
	for (int i = 0; i < 11; i++)
		dic['A' + i] = i + 10;
}

void convert_to_21(string val, unordered_map<char, int> dic)
{
	stringstream ss;
	int value;
	vector<char> result;

	ss << val;
	ss >> value;
	while (value != 0) {
		result.push_back(dic[value % 21]);
		value /= 21;
	}

	cout << "Result : ";
	for (int i = result.size() - 1; i >= 0; i--)
		cout << result[i];
	cout << endl;
}

void convert_to_10(string val, unordered_map<char, int> dic)
{
	int result = 0;
	
	for (int i = 0; i < val.size(); i++)
		result = result * 21 + dic[val[i]];

	cout << "Result : " << result << endl;
}