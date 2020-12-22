#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <memory.h>
#include <vector>
#define _USE_MATH_DEFINES
#include <math.h>

using namespace std;

void FFT(vector<float> xi, vector<float> xj, int n)
{
    int p = 0;
    for (int i = 1; i < n; i <<= 1) ++p;
    for (int i = 0; i < n; ++i)
    {
        int a = i, b = 0;
        for (int j = 0; j < p; ++j) b = (b << 1) + (a & 1), a >>= 1;
        if (b > i) swap(xi[i], xi[b]), swap(xj[i], xj[b]);
    }
       
    float arg = -2 * M_PI / n;
    float ti = cos(arg);
    float tj = sin(arg);
    vector<float> wi, wj;
    wi.assign(xi.size() / 2, 0);
    wj.assign(xi.size() / 2, 0);
    wi[0] = 1.0;
    wj[0] = 0.0;

    for (int j = 1; j < n / 2; ++j)
    {
        wi[j] = wi[j - 1] * ti - wj[j - 1] * tj;
        wj[j] = wi[j - 1] * tj + wj[j - 1] * ti;
    }

    for (int m = 2; m <= n; m <<= 1) for (int k = 0; k < n; k += m)
    {
        for (int j = 0; j < m / 2; ++j)
        {
            int index1 = k + j;
            int index2 = index1 + m / 2;
            int t = n * j / m;
            float ui = xi[index1];
            float uj = xj[index1];

            ti = wi[t] * xi[index2] - wj[t] * xj[index2];
            tj = wi[t] * xj[index2] + wj[t] * xi[index2];

            xi[index1] = ui + ti;
            xj[index1] = uj + tj;
            xi[index2] = ui - ti;
            xj[index2] = uj - tj;
        }   
    }
}

int main(int argc, char* argv[])
{
	ios::sync_with_stdio(false);
	cin.tie(nullptr);
	cout.tie(nullptr);

	int cases = 0;
	string str, cut;
	//ifstream inFile("ex2.csv", ios::binary);
    ifstream inFile(argv[1], ios::binary);

	while (inFile >> str)
	{
        vector<float> xi, xj;
        float tmp;
        stringstream ss(str);

        while (getline(ss, cut, ','))
        {
            stringstream sss(cut);
            sss >> tmp;
            xi.emplace_back(tmp);
        }
        xj.assign(xi.size(), 0);

        FFT(xi, xj, xi.size());

        cout << "Data " << ++cases << " :\n";        
        for (int i = 0; i <= xi.size(); ++i) cout << fixed << setprecision(4) << xi[i] << " " << xj[i] << "\n";
		cout << "\n";
	}

	return 0;
}