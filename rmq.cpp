#include <iostream>
#include <ctime>
#include <vector>
#include <algorithm>

using namespace std;

#define Max_inf -1e9
#define Max_NULL NULL

template <typename T>
class rmq
{
	size_t l, r;
	T Max, MaxAdd;
	rmq *L, *R;
public:
	rmq(size_t sz)
	{
		l = 0, r = sz - 1;
		Max = Max_NULL;
		MaxAdd = Max_NULL;
		if (l != r)
			L = new rmq(l, (l + r) / 2), R = new rmq((l + r) / 2 + 1, r);
	}
	rmq(size_t b, size_t e)
	{
		l = b, r = e;
		Max = Max_NULL;
		MaxAdd = Max_NULL;
		if (l != r)
			L = new rmq(l, (l + r) / 2), R = new rmq((l + r) / 2 + 1, r);
	}
	void ADD (size_t b, size_t e, T x)
	{
		if (b > r || e < l) return;
		if (b <= l && e >= r)
		{
			MaxAdd += x;
			return;
		}
		L->ADD(b, e, x), R->ADD(b, e, x);
		Max = max(L->Max + L->MaxAdd, R->Max + R->MaxAdd);
	}
	T MAX (size_t b, size_t e)
	{
		if (b > r || e < l) return Max_inf;
		if (b <= l && e >= r) return Max + MaxAdd;
		return max(L->MAX(b, e), R->MAX(b, e));
	}
};

int main ()
{
	rmq<int> *r = new rmq<int>(100);
	r->ADD(0, 3, 1);
	cout << r->MAX(3, 4);
	
	return 0;
}
