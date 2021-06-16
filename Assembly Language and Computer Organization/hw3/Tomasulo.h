#ifndef TOMASULO
#define TOMASULO

#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
#include <sstream>
#include <map>

using namespace std;

class Tomasulo
{
public:
	Tomasulo(vector<string> V)
	{
		code = V;
		RS.assign(5, {' ', "", "", 0});
		BUFFER.assign(2, {"", 0, 0, ' ', 0});

		for (int i = 0, n = 0; i < 5; ++i, n += 2) RF[F_[i]] = n;

		for (int i = 0; i < 5; ++i) RAT[F_[i]] = "";

		symbol["ADD"] = '+';
		symbol["ADDI"] = '+';
		symbol["SUB"] = '-';
		symbol["SUBI"] = '-';
		symbol["MUL"] = '*';
		symbol["DIV"] = '/';

		cy_time['+'] = 2;
		cy_time['-'] = 2;
		cy_time['*'] = 10;
		cy_time['/'] = 40;

		opp_RS["RS1"] = 0;
		opp_RS["RS2"] = 1;
		opp_RS["RS3"] = 2;
		opp_RS["RS4"] = 3;
		opp_RS["RS5"] = 4;

		print_input();
		execute();
	}

	void print_input()
	{
		cout << " Input data:\n";
		cout << "שÝשששששששששששששששששששששששששששששששששß\n";
		for (auto& s : code) cout << "שר " << s << " שר\n";
		cout << "שדשששששששששששששששששששששששששששששששששו\n";
		cout << "______________________\n\n";
	}

	void print_cycle(int cycle)
	{
		cout << " cycle " << cycle << "\n\n";
		
		cout << " RF table:\n";
		cout << "שÝשששששששששÞשששששששששß\n";
		for (int i = 0; i < 5; ++i) cout << "שר F" << i + 1 << " שר  " << setw(2) << RF[F_[i]] << "שר\n";
		cout << "שדשששששששששהשששששששששו\n\n";

		cout << " RAT table:\n";
		cout << "שÝשששששששששÞשששששששששששß\n";
		for (int i = 0; i < 5; ++i) cout << "שר F" << i + 1 << " שר  " << setw(3) << RAT[F_[i]] << "שר\n";
		cout << "שדשששששששששהשששששששששששו\n\n";

		cout << " RS table (ADD/SUB):\n";
		cout << "שÝשששששששששששÞשששששששששÞשששששששששששÞשששששששששששß\n";
		for (int i = 0; i < 3; ++i) 
			cout << "שר RS" << i + 1 << " שר   " << RS[i].math << "שר  " << setw(3) << RS[i].a << "שר  " << setw(3) << RS[i].b << "שר\n";
		cout << "שדשששששששששששהשששששששששהשששששששששששהשששששששששששו\n";
		cout << " BUFFER: ";
		if (BUFFER[0].math == ' ') cout << "empty\n\n";
		else cout << '(' << BUFFER[0].rs << ") " << BUFFER[0].a << BUFFER[0].math << BUFFER[0].b << "\n\n";

		cout << " RS table (MUL/DIV):\n";
		cout << "שÝשששששששששששÞשששששששששÞשששששששששששÞשששששששששששß\n";
		for (int i = 3; i < 5; ++i)
			cout << "שר RS" << i + 1 << " שר   " << RS[i].math << "שר  " << setw(3) << RS[i].a << "שר  " << setw(3) << RS[i].b << "שר\n";
		cout << "שדשששששששששששהשששששששששהשששששששששששהשששששששששששו\n";
		cout << " BUFFER: ";
		if (BUFFER[1].math == ' ') cout << "empty\n\n";
		else cout << '(' << BUFFER[1].rs << ") " << BUFFER[1].a << BUFFER[1].math << BUFFER[1].b << "\n\n";

		cout << "______________________\n\n";
	}

private:
	struct rs_data
	{
		char math;
		string a;
		string b;
		int ready;
	};
	struct buffer_data
	{
		string rs;
		int a;
		int b;
		char math;
		int cnt;
	};
	vector<string> F_ = { "F1", "F2", "F3", "F4", "F5" };
	vector<string> RS_ = { "RS1", "RS2", "RS3", "RS4", "RS5" };
	map<string, int> opp_RS;
	map<char, int> cy_time;

	vector<string> code;
	map<string, int> RF;
	map<string, string> RAT;
	vector<rs_data> RS;
	vector<buffer_data> BUFFER;
	map<string, char> symbol;
	map<string, int> result;

	void execute()
	{
		vector<string> V;
		stringstream ss;
		string tmp;
		string cpy;
		int pt = 0;
		int cycle = 0;

		while (++cycle)
		{
			bool change = false;

			// load and dispart instruction
			if (pt < code.size())
			{
				V.clear();
				ss.clear();
				cpy = code[pt];

				ss << code[pt];
				ss >> tmp;
				V.emplace_back(tmp);
				while (getline(ss, tmp, ','))
				{
					tmp.erase(0, tmp.find_first_not_of(" "));
					V.emplace_back(tmp);
				}

				// ISSUE
				if (V[0] == "MUL" || V[0] == "DIV")
				{
					for (int i = 3; i < 5; ++i) if (RS[i].math == ' ')
					{
						issue(V, i);
						change = true;
						++pt;
						break;
					}
				}
				else
				{
					for (int i = 0; i < 3; ++i) if (RS[i].math == ' ')
					{
						issue(V, i);
						change = true;
						++pt;
						break;
					}
				}
			}		

			// WRITE BACK
			--BUFFER[0].cnt;
			--BUFFER[1].cnt;
			if (BUFFER[0].cnt == 0) write_back(0), change = true;
			else if (BUFFER[1].cnt == 0) write_back(1), change = true;

			// DISPATCH
			if (BUFFER[0].math == ' ') for (int i = 0; i < 3; ++i) if (RS[i].ready == 2) dispatch(i, 0), change = true;
			if (BUFFER[1].math == ' ') for (int i = 3; i < 5; ++i) if (RS[i].ready == 2) dispatch(i, 1), change = true;		

			if (change) print_cycle(cycle);
			else if (pt < code.size())
			{
				code.push_back(code[pt]);
				code.erase(code.begin() + pt);
			}

			int out = 0;
			for (int i = 0; i < 5; ++i) if (RS[i].math != ' ') ++out;
			if (BUFFER[0].math != ' ') ++out;
			if (BUFFER[1].math != ' ') ++out;
			if (!out) break;
		}

		for (int i = 0; i < 5; ++i)
		{
			if (RAT[F_[i]] != "") RF[F_[i]] = result[RAT[F_[i]]];
			RAT[F_[i]] = "";
		}

		print_cycle(cycle);
	}

	void issue(vector<string> V, int i)
	{
		RS[i].math = symbol[V[0]];

		if (find(F_.begin(), F_.end(), V[2]) == F_.end()) RS[i].a = V[2], ++RS[i].ready;
		else if (RAT[V[2]] == "") RS[i].a = to_string(RF[V[2]]), ++RS[i].ready;
		else RS[i].a = RAT[V[2]];

		if (find(F_.begin(), F_.end(), V[3]) == F_.end()) RS[i].b = V[3], ++RS[i].ready;
		else if (RAT[V[3]] == "") RS[i].b = to_string(RF[V[3]]), ++RS[i].ready;
		else RS[i].b = RAT[V[3]];

		RAT[V[1]] = RS_[i];
	}

	void dispatch(int i, int n)
	{
		++RS[i].ready;
		BUFFER[n].rs = RS_[i];
		BUFFER[n].a = stoi(RS[i].a);
		BUFFER[n].b = stoi(RS[i].b);
		BUFFER[n].math = RS[i].math;
		BUFFER[n].cnt = cy_time[RS[i].math];
	}

	void write_back(int n)
	{
		int ret = 0;
		if (BUFFER[n].math == '+') ret = BUFFER[n].a + BUFFER[n].b;
		else if (BUFFER[n].math == '-') ret = BUFFER[n].a - BUFFER[n].b;
		else if (BUFFER[n].math == '*') ret = BUFFER[n].a * BUFFER[n].b;
		else if (BUFFER[n].math == '/') ret = BUFFER[n].a / BUFFER[n].b;

		for (auto& [math, a, b, ready] : RS)
		{
			if (BUFFER[n].rs == a) a = to_string(ret), ++ready;
			if (BUFFER[n].rs == b) b = to_string(ret), ++ready;
		}

		result[BUFFER[n].rs] = ret;
		RS[opp_RS[BUFFER[n].rs]] = { ' ', "", "", 0 };
		BUFFER[n] = { "", 0, 0, ' ', 0 };
	}
};

#endif
