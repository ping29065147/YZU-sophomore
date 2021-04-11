#include <iostream>
#include <vector>

using namespace std;

int main()
{
	int n;

	while (cin >> n)
	{
		vector<int> str(n);
		for (int i = 0; i < n; ++i) cin >> str[i];

		vector<int> lis;
		vector<int> lds;
		lis.emplace_back(str[0]);
		lds.emplace_back(str[n - 1]);

		vector<int> num1(n);
		vector<int> num2(n);
		num1[0] = 1;
		num2[n - 1] = 1;
		int cnt1 = 1;
		int cnt2 = 1;

		for (int i = 1; i < n; ++i)
		{
			//lis
			if (str[i] > lis.back())
			{
				lis.emplace_back(str[i]);
				num1[i] = ++cnt1;
			}
			else
			{
				auto it = lower_bound(lis.begin(), lis.end(), str[i]);
				*it = str[i];
				num1[i] = (it - lis.begin() + 1);
			}

			//lds
			int j = n - i - 1;
			if (str[j] > lds.back())
			{
				lds.emplace_back(str[j]);
				num2[j] = ++cnt2;
			}
			else
			{
				auto it = lower_bound(lds.begin(), lds.end(), str[j]);
				*it = str[j];
				num2[j] = (it - lds.begin() + 1);
			}
		}

		int t = min(cnt1, cnt2);

		for (; t > 0; --t) for (int i = 0; i < n; ++i)
			if (num1[i] >= t && num2[i] >= t) goto out;

	out:
		cout << t * 2 - 1 << "\n";
	}
}
