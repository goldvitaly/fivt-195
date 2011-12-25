#include <iostream>
#include <vector>
#include <stdexcept>
#include <exception>

#include "external_sort.hpp"

using namespace std;

bool SortedFileTest(const string& target_filename) {
  fstream file_stream(target_filename, ios::in | ios::binary); 
  int element_count = ReadInt(file_stream);
  int previous = 0, next = 0;
  for(int i = 0; i < element_count; i++) {
    next = ReadInt(file_stream);
    if (i > 0 && previous > next)
      return 0;
    previous = next;
  }
  return 1;
}

int main() {
  string source_filename = "input.txt";
  string destination_filename = "output.txt";
  double t1 = clock();
  ExternalSort(source_filename, destination_filename);
  double t2 = clock();

  if (!SortedFileTest(destination_filename))
    throw logic_error("Wrong sorting order");

  printf("Execution time: %.lf\n", (t2-t1)/1000.0);
  return 0;
}

