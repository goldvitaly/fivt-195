#include <cstdio>
#include <iostream>
#include <algorithm>
#include <ctime>
#include <queue>
#include <cassert>
#include <cstdlib>

#include "bheap.h"

using namespace std;

void correctness_test()
{
	const int sz = 100;
	bheap<int> q[sz];
	std :: vector<int> v[sz];
	
	for (int i = 0; i < 1000; i++)
	{
		int id = rand() % sz;
		int val = rand();
		q[id].push(val);
		v[id].push_back(val);
	}
	for (int i = 0; i < 10; i++)
	{
		int id1 = rand()%sz, id2 = rand()%sz;
		while (id1 == id2)
			id2 = rand()%sz;
		q[id1].merge(q[id2]);
		v[id1].insert(v[id1].end(), v[id2].begin(), v[id2].end());
		v[id2].clear();
	}
	for (int i = 0; i < 1000; i++)
	{
		int id1 = rand()%sz;
		assert(q[id1].size() == v[id1].size());
		if (q[id1].empty())
			continue;
		int a = q[id1].pop();
		vector<int> :: iterator it = min_element(v[id1].begin(), v[id1].end());
		assert(a == *it);
		v[id1].erase(it);
	}
}

template<typename T>
void perfomance_test(T q, string name, int num)
{
	srand(100500);
	vector<int> vals(num);
	generate(vals.begin(), vals.end(), rand);
	time_t begin = clock();
	for (int i = 0; i < num; i++)
		q.push(vals[i]);
	while (!q.empty())
		q.pop();
	time_t end = clock();
	cerr << name << " inserted and erased " << num << " elements in " << (end-begin)/1000 << " ms!\n";
	
}

int main()
{
	correctness_test();
	perfomance_test(priority_queue<int>(), "std :: priority_queue", 500000); // 220 ms
	perfomance_test(bheap<int>(), "my bheap",                       500000); // 2520 ms
	
}
