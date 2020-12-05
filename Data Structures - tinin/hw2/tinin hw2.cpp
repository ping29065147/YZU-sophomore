#include <iostream>
#include <vector>
#include <fstream>
#include <string>

using namespace std;

class JSON
{
public:

    JSON(vector<string> d, vector<string> s)
        :data(d), search(s)
    {
        storedata();
        print();
    }

private:

    void storedata()
    {
        int finish = -1;
        bool isword = false;

        for (auto& v : data) for (auto& c : v)
        {
            if (isword && !(c == '"' && str.back() != '\\'))
            {
                str.push_back(c);
                continue;
            }

            switch (c)
            {
            case '"':
                isword = (isword == false ? true : false);
                if (isword) str.push_back(c);
                break;

            case ':':
                if (!str.empty() && str.front() == '"') str.erase(str.begin());
                pop_key();
                tmpd.first += str + '>';
                str.clear();
                break;

            case ',':
                push_value();
                tmpd.second.clear();
                str.clear();
                break;

            case ' ':
                if (isword) str.push_back(c);
                break;

            case '{':
                (finish == -1) ? finish = 1 : ++finish;
                tmpd.first += " >";
                break;

            case '}':
                push_value();
                --finish;
                tmpd.second.clear();
                str.clear();
                pop_key();
                break;

            case '[':
            case ']':
                break;

            default:
                str.push_back(c);
                break;
            }

            if (!finish) return;
        }
    }

    void push_value()
    {
        if (!str.empty() && str.front() == '"') str.erase(str.begin());
        if (!str.empty())
        {
            auto t = tmpd;

            t.second = str;
            t.first.pop_back();
            dl.emplace_back(t);

            while (!t.first.empty())
            {
                while (!t.first.empty() && t.first[0] != '>') t.first.erase(t.first.begin());
                if (t.first.empty()) break;
                t.first.erase(t.first.begin());
                dl.emplace_back(t);
            }
        }
    }

    void pop_key()
    {
        tmpd.first.pop_back();
        while (!tmpd.first.empty() && tmpd.first[tmpd.first.size() - 1] != '>')
            tmpd.first.pop_back();
    }

    void print()
    {
        //for (auto& i : dl) cout << i.first << "  " << i.second << "\n";
        //cout << "\n\n";

        int f = 0;
        for (auto& i : search)
        {
            bool output = false;

            if (f++) cout << "\n";
            for (auto& [key, value] : dl)
                if (i == key) cout << value << "\n", output = true;
            if (!output) cout << "\n";
        }
    }

    vector<string> data; // case.json
    vector<string> search; // search.txt

    // store data
    // dl.first = key, dl.second = value
    vector< pair<string, string> > dl;

    string str;
    pair<string, string> tmpd;
};

int main(int argc, char* argv[])
{
    // fast io
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cout.tie(nullptr);

    string str;
    vector<string> d, s;

    ifstream in1(argv[1]);
    ifstream in2(argv[2]);
    //ifstream in1("case4.json");
    //ifstream in2("search4.txt");

    while (getline(in1, str)) d.emplace_back(str);
    while (getline(in2, str)) s.emplace_back(str);

    JSON json(d, s);
}