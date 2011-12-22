#include <iostream>
#include <vector>
#include "external_sort.hpp"

using namespace std;

vector<int> ReadFile(string target_filename) {
  vector<int> result;
  fstream file_stream(target_filename, ios::in | ios::binary); 
  int element_count = ReadInt(file_stream);
  for(int i = 0; i < element_count; i++)
    result.push_back(ReadInt(file_stream));
  return result;
}

bool Sorted(const vector<int>& array) {
  for(int i = 0; i < array.size() - 1; i++)
    if (array[i] > array[i + 1])
      return 0;
  return 1;
}

int main() {
  string source_filename = "input.txt";
  string destination_filename = "output.txt";
  double t1 = clock();
  ExternalSort(source_filename, destination_filename);
  double t2 = clock();

  if (Sorted(ReadFile(destination_filename)))
    printf("ok\n");
  else
    printf("fail\n");
  printf("Execution time: %.lf\n", (t2-t1)/1000.0);
  return 0;
}

