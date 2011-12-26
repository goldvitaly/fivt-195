/**
 * @note 	Big Endian is used (most significant bytes have less numbers)
 */

#include <cassert>
#include <cstdlib>
#include <typeinfo>
#include <fstream>
#include "myio.h"

using namespace std;

/**
 * @param argv[1]	number of integers to be generated
 * @param argv[2]	randseed (optional, default = 1984202)
 */
int main(int argc, char* argv[])
{
	ios :: sync_with_stdio(false);
	
	assert(argc > 2);
	
	fstream out (argv[1], ios :: out | ios :: binary);
	long long n = readI64(argv[2]);
	int seed = (argc > 3 ? atoi(argv[3]) : 1984202);
	
	srand(seed);
	
	writeI64(out, n);
	for (int i = 0; i < n; i++)
		writeInt(out, rand());
	
	out.close();
	
	
	return 0;
}
