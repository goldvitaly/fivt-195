#include <iostream>
#include <vector>
#include <algorithm>
#include <ctime>
#include <numeric>
#include <cassert>
#include <cstdlib>
#include "interval_tree.h"
using namespace std;

void correctness_test ()
{
	const int len = 1000;
	const int num = 1000;
	
	srand(100500);
	
	rq :: RangeQuery<long long, rq :: sum<long long>, long long, rq :: sumAdder<long long> > rsq(0, len-1);
	vector<long long> rsq1(len);
	for (int i = 0; i < num; i++)
	{
		int l = rand()%len, r = rand()%len;
		if (l > r)
			swap(l, r);
		if (i%2)
		{
			long long x = ((long long)rand() << 16ll) | rand();
			for (int i = l; i <= r; i++)
				rsq1[i] += x;
			rsq.update(l, r, x);
		}
		else
		{
			long long res = rsq.get(l, r);
			long long res1 = accumulate(rsq1.begin()+l, rsq1.begin()+r+1, 0ll);
			if (res != res1)
			{
				cout << "WA test " << i << endl;
				cout << "Correct: " << res1 << ", your: " << res << endl;
				assert(0);
			}
		}
	}
	cerr << "Test passed!\n";
}

void perfomance_test()
{
	const int len = 1000000;
	const int num = 1000000;
	
	rq :: RangeQuery<int, rq :: min<int>, int, rq :: minPainter<int> > rsq(0, len-1);	
	
	int hash = 0;
	
	time_t start = clock();
	
	for (int i = 0; i < num; i++)
	{
		int l = rand()%len, r = rand()%len;
		if (l > r)
			swap(l, r);
		if (i%2)
		{
			int x = rand();
			rsq.update(l, r, x);
		}
		else
		{
			int res = rsq.get(l, r);
			hash = ((hash<<2)^res)-17;
		}
	}
	
	cerr << "RMQ has just done " << num << " operations with " << len << " elements in " << (clock()-start)/1000 << " ms!\n";
	
	//cerr << hash << endl;
}

int main()
{
	correctness_test();
	perfomance_test();
}
