#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <tuple>
#include <queue>
#include <stack>
#include <memory>
#define type_t tuple<string, string, size_t, size_t>

using namespace std;

int days[13] = {0, 31 ,28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

struct Link_list
{
	Link_list() : id(), icd(), day() {}
	Link_list(string& ID, string& ICD, size_t& DAY) : id(ID), icd(ICD), day(DAY) {}

	string id, icd;
	size_t day;
	shared_ptr< Link_list > link;
};

class list_sort
{

public:
	list_sort(char* in)
		: argv(in)
	{
		ios::sync_with_stdio(false);
		cin.tie(nullptr);
		cout.tie(nullptr);
		myHead = pos = make_shared<Link_list>();

		readfile();
		split();
		store();
		print();
	}

private:

	void readfile()
	{
		string input;
		ifstream inFile(argv);
		while (getline(inFile, input)) data.emplace_back(input);
	}

	void split()
	{
		for (auto& i : data) {

			size_t datef, dateb;
			vector<string> temp;
			size_t place;

			while ((place = i.find(',')) != -1) {
				temp.emplace_back(place ? i.substr(0, place) : "");
				i.erase(0, place + 1);
			}
			temp.emplace_back(i.empty() ? "" : i);
					
			if (temp[2].empty()) {
				int year = atoi(temp[1].substr(0, 4).c_str());
				int month = atoi(temp[1].substr(4, 2).c_str());
				days[2] = (year % 4 == 0 && (year % 100 != 0 || year % 400 == 0)) ? 29 : 28;
				dateb = year*10000 + month*100 + days[month];
			}
			else dateb = atoi(temp[2].c_str());
			datef = atoi(temp[1].c_str());

			for (size_t i = 3; i < temp.size(); ++i)
				if (!temp[i].empty()) {
					auto tmp = make_tuple(temp[0], temp[i], datef, dateb);
					spdata.emplace(tmp);
				}		
		}

		while (!spdata.empty()) vdata.emplace_back(spdata.top()), spdata.pop();
	}

	void store()
	{
		for (size_t i = 0; i < vdata.size(); ++i) {
			
			string _id, _icd;
			size_t _d1, _d2;
			tie(_id, _icd, _d1, _d2) = vdata[i];

			priority_queue< pair<size_t, size_t> > pq;
			pair<size_t, size_t> tmp = { _d1, _d2 };
			pq.emplace(tmp);

			while (i + 1 < vdata.size()) {

				string id, icd;
				size_t d1, d2;
				tie(id, icd, d1, d2) = vdata[++i];

				if (_id != id || _icd != icd) {
					--i;
					break;
				}

				pair<size_t, size_t> tmp = { d1, d2 };
				pq.emplace(tmp);
			}

			stack< pair<size_t, size_t> > s;
			while (pq.size() > 1) s.push({ pq.top().first, pq.top().second }), pq.pop();

			while (!s.empty()){  

				if (s.top().first <= pq.top().second) {
					if (s.top().second > pq.top().second) {
						auto tmp = pq.top().first;
						pq.pop(), pq.push({ tmp, s.top().second });
					}
				}
				else pq.push({ s.top().first, s.top().second });

				s.pop();
			}

			size_t d = 0;
			while (!pq.empty()) d += countday(pq.top().first, pq.top().second), pq.pop();

			pos->link = make_shared<Link_list>(_id, _icd, d);
			pos = pos->link;
		}
	}

	size_t countday(size_t day1, size_t day2)
	{
		size_t y1 = day1 / 10000;
		size_t m1 = (day1 / 100) % 100;
		size_t d1 = day1 % 100;
		size_t y2 = day2 / 10000;
		size_t m2 = (day2 / 100) % 100;
		size_t d2 = day2 % 100;
		size_t count = 0;

		while (y1 < y2){
			days[2] = (y1 % 4 == 0 && (y1 % 100 != 0 || y1 % 400 == 0)) ? 29 : 28;
			count += days[m1++] - d1;
			while (m1 <= 12) count += days[m1++];
			d1 = 0, m1 = 1, ++y1;
		}
		days[2] = (y1 % 4 == 0 && (y1 % 100 != 0 || y1 % 400 == 0)) ? 29 : 28;
		while (m2 > m1) d2 += days[--m2];
		count += d2 - d1 + 1;

		return count;
	}

	void print()
	{
		auto it = myHead->link;
		while (it) cout << it->id << "," << it->icd << "," << it->day << "\n", it = it->link;
	}

	char* argv;
	vector<string> data;
	priority_queue< type_t, vector<type_t>, greater<type_t> > spdata;
	vector<type_t> vdata;
	shared_ptr<Link_list> myHead, pos;
};

int main(int argc, char* argv[])
{	
	list_sort mylist(argv[1]);
}