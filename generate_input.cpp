#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <time.h>
#include <vector>
#include <sstream>

#include "external_sort.hpp"

using namespace std;

int main(int argc, char *argv[]) {
  srand(42);
  int n = 100, pnt = 0;
  if (argc > 1) {
    istringstream stream(argv[1], istringstream::in);
    stream >> n;
  }
  fstream output("input.txt", ios::out | ios::binary | ios::trunc);
  PrintInt(n, output);
  for(int i=0; i<n; i++) {
    int x = rand();
    PrintInt(x, output);
  }
  return 0;
}
