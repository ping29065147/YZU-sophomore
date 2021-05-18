#ifndef PREDITOR
#define PREDITOR
#include <iostream>
#include <vector>
#include <sstream>
#include <map>
#include <iomanip>
#include <string>

using namespace std;

struct Data
{
	int history;
	string curState[4];
	char predict;
	char answer;
	int entry;
	string instruction;
};

class predictor
{

public:

	// easy input
	void execute(string ans)
	{
		init(0);
		for (auto& A : ans) next_state(A, 0);
		ret.emplace_back(tmp2);
		print_easy();
	}

	// hard input
	void execute(vector<string> V, int entry)
	{
		init(entry);
		string pc, tmp;

		for (auto& i : V)
		{
			stringstream ss(i);
			ss >> pc;

			if (pc[pc.size() - 1] == ':')
			{
				pc.pop_back();
				program.emplace_back(pc);
				Label[pc] = now;
			}
			else
			{
				getline(ss, tmp);
				tmp.erase(0, tmp.find_first_not_of(" "));
				program.emplace_back(tmp);
			}
			
			PC[now++] = en++;
			if (en == Entry) en = 0;
		}

		now = 0;
		solve();
		print_hard();
	}	

private:

	string state[4] = { "SN", "WN", "WT", "ST" };
	vector< vector<Data> > ret;
	vector<string> program;
	map<int, int> PC;
	map<string, int> Label;
	map<string, int> Register;
	int Entry;

	int en;
	bool ishard;
	int now, next;
	vector<int> his;
	vector< vector<string> > cur;
	vector<Data> tmp2;

	void init(int a)
	{
		if (a == 0)
		{
			ishard = false;
			Entry = 1;
		}
		else
		{
			ishard = true;
			Entry = a;
		}

		ret.clear();
		his.assign(Entry, 0);
		en = now = next = 0;
		Register["R0"] = 0;
		cur.assign(Entry, vector<string>(4, "SN"));
	}

	string binary(int his)
	{
		if (his == 0) return "00";
		else if (his == 1) return "01";
		else if (his == 2) return "10";
		else return "11";
	}

	string status(char A, int e)
	{
		int pt;
		for (int i = 0; i < 4; ++i) if (cur[e][his[e]] == state[i]) pt = i;

		if (A == 'T') { if (++pt == 4) --pt; }
		else if (--pt == -1) ++pt;
		
		return state[pt];
	}

	void next_state(char A, int e)
	{
		Data tmp;	

		tmp.history = his[e];
		for (int i = 0; i < 4; ++i) tmp.curState[i] = cur[e][i];
		tmp.predict = ' ';
		tmp.answer = ' ';
		if (ishard)
		{
			tmp.entry = e;
			tmp.instruction = program[now];
		}

		if (e == PC[now])
		{
			tmp.predict = cur[e][his[e]][1];
			tmp.answer = A;
			cur[e][his[e]] = status(A, e);
			his[e] = (his[e] << 1) % 4;
			if (A == 'T') ++his[e];
		}

		tmp2.emplace_back(tmp);
	}

	char compiler()
	{
		while (Label.find(program[now]) != Label.end()) ++now;

		vector<string> V;
		stringstream ss(program[now]);
		string tmp;
		char branch = 'N';

		ss >> tmp;
		V.emplace_back(tmp);
		while (getline(ss, tmp, ','))
		{
			tmp.erase(0, tmp.find_first_not_of(" "));
			V.emplace_back(tmp);
		}

		if (V[0] == "addi") Register[V[1]] = Register[V[2]] + stoi(V[3]), next = now + 1;
		else if (V[0] == "subi") Register[V[1]] = Register[V[2]] - stoi(V[3]), next = now + 1;
		else if (V[0] == "muli") Register[V[1]] = Register[V[2]] * stoi(V[3]), next = now + 1;
		else if (V[0] == "add") Register[V[1]] = Register[V[2]] + Register[V[3]], next = now + 1;
		else if (V[0] == "andi") Register[V[1]] = Register[V[2]] & stoi(V[3]), next = now + 1;
		else if (V[0] == "beq")
		{
			if (Register[V[1]] == Register[V[2]]) branch = 'T', next = Label[V[3]];
			else next = now + 1;
		}
		else if (V[0] == "bne")
		{
			if (Register[V[1]] != Register[V[2]]) branch = 'T', next = Label[V[3]];
			else next = now + 1;
		}
		
		return branch;
	}

	void solve()
	{
		while (now != program.size() - 1)
		{
			char A = compiler();
			tmp2.clear();

			for (int i = 0; i < Entry; ++i) next_state(A, i);
			ret.emplace_back(tmp2);

			now = next;
		}
	}

	void print_easy()
	{
		cout << "\nhistory   " << "2BC  " << "2BC  " << "2BC  " << "2BC  " << "   predict   " << "answer    \n\n";
		for (auto& T : ret) for (auto& [history, curState, predict, answer, _, __] : T)
		{
			cout << "   " << binary(history) << "      ";
			for (auto& i : curState) cout << i << "   ";
			cout << "      " << predict << "      ";
			cout << "   " << answer << "      \n";
		}
	}

	void print_hard()
	{
		cout << "\n         instruction     " << "entries   " << "history   " << "2BC  " << "2BC  "
			<< "2BC  " << "2BC  " << "   predict   " << "answer    \n\n";
		for (auto& T : ret)
		{
			for (auto& [history, curState, predict, answer, entry, instruction] : T)
			{
				if (predict == 'N' || predict == 'T') cout << setw(20) << instruction << "     ";
				else cout << setw(25) << " ";
				cout << "    " << entry << "    ";
				cout << "   " << binary(history) << "      ";
				for (auto& i : curState) cout << i << "   ";
				cout << "      " << predict << "      ";
				cout << "   " << answer << "      \n";
			}
			cout << "\n";
		}
	}
};


#endif
