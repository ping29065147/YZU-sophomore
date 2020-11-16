#ifndef KMAP_H
#define KMAP_H

#include <vector>
#include <iostream>
#include <fstream>

using namespace std;

class k_map
{
	public:
		//fast io
		k_map();

		//read k-map from file and store it in square
		void readfile();

		//return the number of cases
		int getnum();

		void readdata();

		//swap the data in the right position
		// m0  m1  m3  m2
		// m4  m5  m7  m6
		// m12 m13 m15 m16
		// m8  m9  m11 m10
		void dealwithkmap(int c);	

		//store both data's binary number in m
		//store ones compliment in ones
		void find_ones_dontcare();

		//compare if the ones and don't care can combine
		void compare();

		//print the result after compare
		void printcompare();

		//use dfs to search minterm of the result after compare
		void searchminterm();

		//print the final results
		void printresults();

		void clear();
			
	private:
		//print the k-map
		void printkmap(int c);

		//print k-map's ones compliment and don't care
		//print binary numbers of ones and don't care
		void print_ones_dontcare();

		//find all the minterm
		void dfs(vector< vector<int> > data, vector< pair< vector<int>, bool > > ones,
			int start, int end, int index, int r, string re);

		ifstream inFile;

		int num, input;

		//store -1 in the position square[0]
		//the input start from square[1]
		vector<int> square;

		//store current data (ones and don't care)
		vector< vector<int> > m;

		//store m's ones compliment
		vector< pair< vector<int>, bool > > ones;	

		vector<string> final_result;
};


#endif KMAP_H
