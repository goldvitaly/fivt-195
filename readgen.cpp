// Not useful, just for testing
// First 8 bytes: n = number of following integers (Big Endian)
// Next n*4 bytes: integers

#include <fstream>
#include <vector>
#include "myio.h"

using namespace std;

int main()
{
	fstream in ("array.txt", fstream :: in);
	
	int n = readI64(in);
	for (int i = 0; i < n; i++)
		cout << readInt(in) << " ";
		
	
	
	return 0;
}
