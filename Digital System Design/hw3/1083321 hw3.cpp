#include <iostream>
#include <vector>
#include <fstream>
#include <tuple>
#include <map>
#include <string>

using namespace std;
typedef tuple<string, string, string> tp;

int main()
{
	// fast io
	ios::sync_with_stdio(false);
	cin.tie(nullptr);
	cout.tie(nullptr);

	ifstream inFile("t00.kiss");
	string str, reset;
	int in, out, state, term, cnt = 0;
	map< string, vector<tp> > data;
	vector<string> V;
	map<string, int> same;

	// store data in map
	while (inFile >> str, str != ".e")
	{
		if (str == ".i") inFile >> in;
		else if (str == ".o") inFile >> out;
		else if (str == ".p") inFile >> term;
		else if (str == ".s") inFile >> state;
		else if (str == ".r") inFile >> reset;
		else if (str == "#")
		{
			getline(inFile, str);
			continue;
		}
		else
		{
			string cs, ns, o;
			inFile >> cs >> ns >> o;
			data[cs].push_back({ ns, str, o });
			if (!count(V.begin(), V.end(), cs)) V.emplace_back(cs);
			if (!count(V.begin(), V.end(), ns)) V.emplace_back(ns);
			++same[o];
		}
	}

	// reduce terms
	bool re = true;
	while (re)
	{
		re = false;

		for (int i = 0; i < V.size() && !re; ++i) for (int j = i + 1; j < V.size(); ++j)
		{
			bool out = false;
			for (auto& [u_ns, u_i, u_o] : data[V[i]]) for (auto& [v_ns, v_i, v_o] : data[V[j]])
			{
				if (u_i == v_i) out = true;
			}

			// "data[V[i]] == data[V[j]]" : next state, input, output are the same
			// "!out" : there is no conflict on inputs
			if (data[V[i]] == data[V[j]] || !out)
			{
				if (!out) for (auto& [v_ns, v_i, v_o] : data[V[j]]) data[V[i]].push_back({ v_ns, v_i, v_o });

				for (auto& [s, v] : data) for (auto& [ns, _i, _o] : v)
					if (ns == V[j]) ns = V[i];

				data.erase(V[j]);
				auto it = find(V.begin(), V.end(), V[j]);
				V.erase(it);
				re = true;

				break;
			}
		}
	}

	// reduce terms if outputs are the same
	if (same.size() == 1)
	{
		for (int i = 1; i < V.size(); ++i) data.erase(V[i]);
		for (auto& [s, v] : data) for (auto& [ns, _i, _o] : v)
			if (ns != s) ns = s;
	}

	// print map
	/*
	for (auto& [s, v] : data)
	{
		cout << s << "\n";
		for (auto& [ns, _i, _o] : v) cout << ns << " " << _i << " " << _o << "\n";
		cout << "\n";
	}
	cout << "\n";
	*/

	// print results
	cout << ".i " << in << "\n";
	cout << ".o " << out << "\n";
	cout << ".s " << data.size() << "\n";
	for (auto& [s, v] : data) for (auto& [ns, _i, _o] : v) ++cnt;
	cout << ".p " << cnt << "\n";
	cout << ".r " << reset << "\n";
	for (auto& [s, v] : data) for(auto & [ns, _i, _o] : v)
	{
		cout << _i << " " << s << " " << ns << " " << _o << "\n";
	}
	cout << ".e\n";
	cout << "\nReduce " << state - data.size() << " states.\n";

	return 0;
}