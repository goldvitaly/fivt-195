#include <fstream> 
#include <sstream>
#include "external_sort.h"
#include "ByteFileSorter.h"
using namespace ExternalSort;
int main() {
	default_external_sort<int> (5, "test");
	
	ByteFileIO input("second", ByteFileIO::type::STABLE);
	external_sort<unsigned> (100, input, ByteFileSorter (), StdSort<int> ());
	return 0;
}
