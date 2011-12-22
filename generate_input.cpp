#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <time.h>
#include <vector>

#include "external_sort.hpp"

using namespace std;

int main(int argc, char *argv[]) {
  int n = 0, pnt = 0;
  if (argc > 1) {
    while (pnt < strlen(argv[1])) {
      n *= 10;
      n += argv[1][pnt++] - '0';
    }
  }
  PrintInt(n);
  srand(time(NULL));
  for(int i=0; i<n; i++) {
    int x = rand();
    PrintInt(x);
  }
  return 0;
}
