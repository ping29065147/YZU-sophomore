#include <iostream>
#include <stack>
#include <string>

using namespace std;

int main()
{
	ios::sync_with_stdio(false);
	cin.tie(nullptr);
	cout.tie(nullptr);

	int T;
	string str;
	cin >> T;
	cin.ignore();

	while (T--)
	{
		stack<char> S;
		getline(cin, str);

		for (auto& c : str)
		{
			if (c == ']')
			{
				if (!S.empty() && S.top() == '[') S.pop();
				else (S.emplace(c));
			}
			else if (c == ')')
			{
				if (!S.empty() && S.top() == '(') S.pop();
				else (S.emplace(c));
			}
			else S.emplace(c);
		}

		//while (!S.empty()) cout << S.top(), S.pop();
		cout << (S.empty() ? "Yes" : "No") << "\n";
	}


	return 0;
}