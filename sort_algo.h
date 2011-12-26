#ifndef SORT_ALGO_H
#define SORT_ALGO_H

#include <fstream>
#include <algorithm>
#include <vector>
#include "myio.h"
using namespace std;

void sortSegment (istream& in, ostream& out, int n)
{
	vector<int> a(n);
	for (int i = 0; i < n; i++)
		a[i] = readInt(in);
	sort(a.begin(), a.end());
	for (int i = 0; i < n; i++)
		writeInt(out, a[i]);
	out.flush();
}

void merge (istream& in1, istream& in2, ostream& out, int n1, int n2)
{
	int ptr1 = 0, ptr2 = 0;
	while (ptr1 < n1 && ptr2 < n2)
	{
		if (ptr2 == n2 || peekInt(in1) < peekInt(in2))
			writeInt(out, readInt(in1)), ptr1++;
		else
			writeInt(out, readInt(in2)), ptr2++;
	}
}

#endif
