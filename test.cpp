#include "interval.h"
#include <functional>
#include <cstdlib>
#include <algorithm>
#include <iostream>
#include <iomanip>
#include <cmath>

using namespace std;

template <class T>
struct test_op
{
	T operator()(T a, T b)
	{
		return a+b;
	}
};

template <class T>
struct test_mod_op
{
	T operator()(T value, T modify, int left, int right)
	{
		//cerr << value << " " << modify << endl;
		return value + modify * (right - left + 1);
	} 
};

template <class T>
struct test_mod_upd
{
	T operator()(T old_mod, T new_mod)
	{
		return old_mod + new_mod;
	}
};

int main()
{
	srand(43);
	
	typedef double test_type;
	const int test_size = 10;
	const int rand_range = 20;
	const int test_count = 300;
	
	IntervalTree<test_type, test_op<test_type>, test_mod_op<test_type>, test_mod_upd<test_type> >* itree = new IntervalTree<test_type, test_op<test_type>, test_mod_op<test_type>, test_mod_upd<test_type> >(test_size);
	
	test_type* array = new test_type[test_size];
	
	test_type next;
	for(int i = 0; i < test_size; i++)
	{
		next = test_type(rand() % rand_range / (double)rand_range);
		itree->assign(next, i);
		array[i] = next;
	}
	
	itree->print(cout);
	
	int left, right;
	test_type tree_res, array_res;
	
	test_type modif;
	for(int i = 0; i < test_count; i++)
	{
		right = rand() % (test_size - 1) + 1;
		left = rand() % right;
		modif = rand() % rand_range;
		//for(int j = 0; j < test_size; j++)
		//	cerr << array[j] << " ";
		//cerr << endl;
		for(int j = left; j <= right; j++)
			array[j] = test_mod_op<test_type>()(array[j], modif, j, j);
		//for(int j = 0; j < test_size; j++)
		//	cerr << array[j] << " ";
		//cerr << endl;
		itree->modify(left, right, modif);
	}
	
	itree->print(cout);
	
	for(int i = 0; i < test_count; i++)
	{
		right = rand() % (test_size - 1) + 1;
		left = rand() % right;
		tree_res = itree->query(left, right);
		array_res = array[left];
		for(int j = left + 1; j <= right; j++)
		{
			array_res = test_op<test_type>()(array_res, array[j]);
		}
		if(abs(tree_res - array_res) >= 0.001)
		{
			cerr << "Panic! Interval tree result (" << tree_res << ") is incorrect. Correct: " << array_res << endl;
			return -1;
		}
	}
	
	delete itree;
	delete array;
	
	cerr << "Passed _ALL_ the tests" << endl;
	
	return 0;
}
