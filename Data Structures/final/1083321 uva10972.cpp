#include <iostream>
#include <vector>
#include <algorithm>
#include <map>

using namespace std;

vector< vector<int> > G;
vector<int> dfn;
vector<int> low;
int dep;
vector<int> bridge;

void dfs(int u, int p)
{
	dfn[u] = low[u] = ++dep;

	for (auto& v : G[u])
	{
		if (!dfn[v]) dfs(v, u), low[u] = min(low[u], low[v]);
		else
		{
			if (v != p) low[u] = min(low[u], dfn[v]);
			continue;
		}

		if (low[v] > dfn[u])
		{
			bridge.emplace_back(u);
			bridge.emplace_back(v);
		}
	}
}

int main()
{
	ios::sync_with_stdio(false);
	cin.tie(nullptr);
	cout.tie(nullptr);

	int n, m;

	while (cin >> n >> m)
	{

		G.assign(n + 1, vector<int>());
		dfn.assign(n + 1, 0);
		low.assign(n + 1, 0);
		dep = 0;
		int con0 = 0, con1 = 0;
		bridge.clear();

		while (m--)
		{
			int u, v;
			cin >> u >> v;
			G[u].emplace_back(v);
			G[v].emplace_back(u);
		}

		for (int i = 1; i <= n; ++i)
		{
			if (!dfn[i])
			{
				auto s = bridge.size();
				dfs(i, -1);
				if (s == bridge.size()) ++con0;
			}
		}

		if (bridge.size() == 0 && con0 == 1) cout << "0\n";
		else
		{
			map<int, int> M;

			for (int u = 1; u <= n; ++u)
			{
				for (auto& v : G[u])
				{
					M[low[u]] += 0;
					if (low[u] != low[v]) ++M[low[u]];
				}
			}

			for (auto& [__, i] : M)
			{
				if (i == 1) ++con1;
			}

			cout << con0 + (con1 + 1) / 2 << "\n";
		}
	}

	return 0;
}