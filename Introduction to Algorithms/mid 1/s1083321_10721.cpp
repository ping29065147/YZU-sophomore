#include <iostream>
#include <memory.h>

using namespace std;

long long dp[51][51][51];

int main()
{
	int n, k, m;
	memset(dp, 0, sizeof(dp));

	for (int i = 0; i < 51; ++i) dp[0][0][i] = 1;

	for (int i = 1; i < 51; ++i) for (int j = 1; j < 51; ++j) for (int k = 0; k < 51; ++k)
		for (int x = 1; x <= k && x <= i; ++x) dp[i][j][k] += dp[i - x][j - 1][k];

	while (cin >> n >> k >> m) cout << dp[n][k][m] << "\n";
}