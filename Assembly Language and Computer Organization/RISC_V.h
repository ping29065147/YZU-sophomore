#ifndef RISC_V
#define RISC_V
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <map>
#include <tuple>

using namespace std;

class risc_v
{
public:

	risc_v()
	{
		// initial of TYPE-R
		r_type["ADD"] = { "0000000", "000", "0110011" };
		r_type["SUB"] = { "0100000", "000", "0110011" };
		r_type["SLL"] = { "0000000", "001", "0110011" };
		r_type["SLT"] = { "0000000", "010", "0110011" };
		r_type["SLTU"] = { "0000000", "011", "0110011" };
		r_type["XOR"] = { "0000000", "100", "0110011" };
		r_type["SRL"] = { "0000000", "101", "0110011" };
		r_type["SRA"] = { "0100000", "101", "0110011" };
		r_type["OR"] = { "0000000", "110", "0110011" };
		r_type["AND"] = { "0000000", "111", "0110011" };
		r_type["SLLI"] = { "0000000", "001", "0010011" };
		r_type["SRLI"] = { "0000000", "101", "0010011" };
		r_type["SRAI"] = { "0100000", "101", "0010011" };

		// initial of TYPE-I
		i_type["LB"] = { "000", "0000011" };
		i_type["LH"] = { "001", "0000011" };
		i_type["LW"] = { "010", "0000011" };
		i_type["LBU"] = { "100", "0000011" };
		i_type["LHU"] = { "000", "0000011" };
		i_type["ADDI"] = { "000", "0010011" };
		i_type["SLTI"] = { "010", "0010011" };
		i_type["SLTIU"] = { "011", "0010011" };
		i_type["XORI"] = { "100", "0010011" };
		i_type["ORI"] = { "110", "0010011" };
		i_type["ANDI"] = { "111", "0010011" };
		i_type["JALR"] = { "000", "1100111" };

		// initial of TYPE-S
		s_type["SB"] = "000";
		s_type["SH"] = "001";
		s_type["SW"] = "010";

		// initial of TYPE-U
		u_type["LUI"] = "0110111";
		u_type["AUIPC"] = "0010111";

		// initial of TYPE-SB
		sb_type["BEQ"] = "000";
		sb_type["BNE"] = "001";
		sb_type["BLT"] = "100";
		sb_type["BGE"] = "101";
		sb_type["BLTU"] = "110";
		sb_type["BGEU"] = "111";
	}

	// split the input string in vector
	void load(string input)
	{
		reset();

		string tmp;
		stringstream ss(input);

		ss >> tmp;
		V.emplace_back(tmp);
		while (getline(ss, tmp, ',')) V.emplace_back(tmp);
	}

	// cinvert risc-v code to machine code
	void convert()
	{
		// turn alphabet to capital letter
		for (auto& i : V[0]) i = toupper(i);

		// TYPE-R
		if (r_type.find(V[0]) != r_type.end())
		{
			auto& [funct5_2, funct3, opcode] = r_type[V[0]];
			result = funct5_2 + binary(V[3], 5) + binary(V[2], 5) + funct3 + binary(V[1], 5) + opcode;
		}
		// TYPE-I
		else if (i_type.find(V[0]) != i_type.end())
		{
			auto& [funct3, opcode] = i_type[V[0]];
			result = binary(V[3], 12) + binary(V[2], 5) + funct3 + binary(V[1], 5) + opcode;
		}
		// TYPE-S
		else if (s_type.find(V[0]) != s_type.end())
		{
			string tmp = V[2];
			string rs1, simm;

			rs1 = tmp.substr(tmp.find('(') + 1, tmp.find(')') - tmp.find('(') - 1);
			simm = tmp.substr(0, tmp.find('('));

			result = binary(simm, 12).substr(5, 7) + binary(V[1], 5) + binary(rs1, 5) + s_type[V[0]] + binary(simm, 12).substr(0, 5) + "0100011";
		}
		// TYPE-U
		else if (u_type.find(V[0]) != u_type.end())
		{
			result = binary(V[2], 20) + binary(V[1], 5) + u_type[V[0]];
		}
		// TYPE-UJ
		else if (V[0] == "JAL")
		{
			string imm = binary(V[2], 21);
			result = imm.substr(0, 1) + imm.substr(11, 10) + imm.substr(10, 1) + imm.substr(1, 8) + binary(V[1], 5) + "1101111";
		}
		// TYPE-SB
		else if (sb_type.find(V[0]) != sb_type.end())
		{
			string imm = binary(V[3], 13);
			result = imm.substr(0, 1) + imm.substr(3, 6) + binary(V[2], 5) + binary(V[1], 5) + sb_type[V[0]]
				+ imm.substr(8, 4) + imm.substr(1, 1) + "1100011";

		}
		else result = "unknown";
	}

	// print result
	void print()
	{
		cout << "\n" << result << "\n\n";
	}

	// print details
	void print_detail()
	{
		// TYPE-R
		if (r_type.find(V[0]) != r_type.end())
		{
			cout << "TYPE-R\n";
			cout << "funct5    " << "funct2    " << "rs2       " << "rs1       " << "funct3    " << "rd        " << "opcode\n";
			cout << result.substr(0, 5) << "     " << result.substr(5, 2) << "        " << result.substr(7, 5) << "     "
				<< result.substr(12, 5) << "     " << result.substr(17, 3) << "       " << result.substr(20, 5) << "     "
				<< result.substr(25, 7) << "\n\n";
		}
		// TYPE-I
		else if (i_type.find(V[0]) != i_type.end())
		{
			cout << "TYPE-I\n";
			cout << "simm[11:0]          " << "rs1       " << "funct3    " << "rd        " << "opcode\n";
			cout << result.substr(0, 12) << "        " << result.substr(12, 5) << "     " << result.substr(17, 3)
				<< "       " << result.substr(20, 5) << "     " << result.substr(25, 7) << "\n\n";
		}
		// TYPE-S
		else if (s_type.find(V[0]) != s_type.end())
		{
			cout << "TYPE-S\n";
			cout << "simm[11:5]     " << "rs2       " << "rs1       " << "funct3    " << "simm[4:0]      " << "opcode\n";
			cout << result.substr(0, 7) << "        " << result.substr(7, 5) << "     " << result.substr(12, 5)
				<< "     " << result.substr(17, 3) << "       " << result.substr(20, 5) << "          "
				<< result.substr(25, 7) << "\n\n";
		}
		// TYPE-U
		else if (u_type.find(V[0]) != u_type.end())
		{
			cout << "TYPE-U\n";
			cout << "simm[31:12]              " << "rd        " << "opcode\n";
			cout << result.substr(0, 20) << "     " << result.substr(20, 5) << "     " << result.substr(25, 7) << "\n\n";
		}
		// TYPE-UJ
		else if (V[0] == "JAL")
		{
			cout << "TYPE-UJ\n";
			cout << "simm[20|10:1|11|19:12]   " << "rd        " << "opcode\n";
			cout << result.substr(0, 20) << "     " << result.substr(20, 5) << "     " << result.substr(25, 7) << "\n\n";
		}
		// TYPE-SB
		else if (sb_type.find(V[0]) != sb_type.end())
		{
			cout << "TYPE-SB\n";
			cout << "simm[12|10:5]  " << "rs2       " << "rs1       " << "funct3    " << "simm[4:1|11]   " << "opcode\n";
			cout << result.substr(0, 7) << "        " << result.substr(7, 5) << "     " << result.substr(12, 5)
				<< "     " << result.substr(17, 3) << "       " << result.substr(20, 5) << "          "
				<< result.substr(25, 7) << "\n\n";
		}
	}

private:

	vector<string> V;	
	string result;

	map<string, tuple<string, string, string>> r_type;
	map<string, pair<string, string>> i_type;
	map<string, string> s_type;
	map<string, string> u_type;
	map<string, string> sb_type;

	void reset()
	{
		V.clear();
		result = "";
	}

	string binary(string rs, int len)
	{
		char __ = ' ';
		int reg, i = 0;
		string ret;
		vector<char> tmpv;
		stringstream ss;

		ss << rs;
		if (rs[0] == 'x' || rs[0] == ' ') ss >> __;
		ss >> reg;

		while (reg != 0 || tmpv.size() != len) tmpv.emplace_back(reg % 2), reg /= 2;
		for (int i = tmpv.size() - 1; i >= 0; --i) ret.push_back(tmpv[i] + '0');

		return ret;
	}
};

#endif //RISC_V