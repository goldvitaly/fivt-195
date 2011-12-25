#ifndef EXTERNAL_SORT_HPP
#define EXTERNAL_SORT_HPP 

#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <vector>
#include <queue>
#include <fstream>
#include <algorithm>

using namespace std;

const int block_size = 1000000;

int ReadInt(istream& stream) {
  int result = 0;
  for(int i = 0; i < 4; i++)
    result = (result << 8) | stream.get();
  return result;
}

void PrintInt(int x, ostream& F) {
  vector<int> digit;
  for(int i = 3; i >= 0; i--) {
    char NextChar = (x >> (8 * i)) & ((1 << 8) - 1);
    F.put(NextChar);
  }
}

void ExternalSort(const string& source_filename, const string& destination_filename) {
  fstream storage("storage.txt", ios::trunc | ios::out | ios::binary);
  fstream source(source_filename.c_str(), ios::in | ios::binary);
  fstream destination(destination_filename.c_str(), ios::out | ios::trunc | ios::binary);
  int element_count = 0;
  element_count = ReadInt(source);
  
  int block_count = (element_count + block_size - 1) / block_size;

  vector<int> block_storage(min(block_size, element_count));
  vector< pair<fstream*, int> > block_stream;

  priority_queue< pair<int, int> , vector<pair<int, int> >, greater<pair<int, int> > > Q;

  for(int i=0; i < block_count; i++) {
    int block_element_count = min(block_size, element_count - i * block_size);
    for(int j=0; j < block_element_count; j++) {
      block_storage[j] = ReadInt(source);
    }
    sort(block_storage.begin(), block_storage.end());

    block_stream.push_back(
    make_pair(
     new fstream("storage.txt", ios::in | ios::out | ios::trunc | ios::binary),
     min(block_size, element_count - i * block_size)
    ));
    block_stream[i].first->seekg(storage.tellp());

    for(int j=0; j < block_element_count; j++) {
      PrintInt(block_storage[j], storage);
    }
  }
  source.close();
  storage.close();
  storage.open("storage.txt", ios::in | ios::binary);
  for(int i=0; i < block_count; i++) {
    Q.push(make_pair(ReadInt(*block_stream[i].first), i));
  }

  PrintInt(element_count, destination);
  while (!Q.empty()) {
    pair<int,int> top_element = Q.top(); Q.pop();
    PrintInt(top_element.first, destination);
    int block_number = top_element.second;
    if (--block_stream[block_number].second)
    {
      int next = ReadInt(*block_stream[block_number].first);
      Q.push(make_pair(next, top_element.second));
    }
  }
  for(int i = 0; i < block_count; i++)
    delete block_stream[i].first;
  unlink("storage.txt");
}


#endif /* EXTERNAL_SORT_HPP */
