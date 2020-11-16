#include <iostream>
#include <fstream>
#include <string>

using namespace std;

int main(int argc, char* argv[])
{
	int x = 0, count = 0, decimal;
	int temp[8];
	string result;

	ifstream inFile(argv[1], ios::in | ios::binary);

	while (inFile) {
		x++;
		if (x > 54) {
			temp[count++] = (inFile.get() == 0);
			if (count == 8) {
				count = 0;
				decimal = temp[7] * 1 + temp[6] * 2 + temp[5] * 4 + temp[4] * 8 + temp[3] * 16 + temp[2] * 32 + temp[1] * 64 + temp[0] * 128;
				if (decimal > 126)
					break;
				result.push_back(decimal);
			}
			inFile.get();
		}
		inFile.get();
	}
	inFile.close();

	cout << result;

	return 0;
}