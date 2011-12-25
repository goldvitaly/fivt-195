#include "rmq_assign_modification.h"
#include "rmq_plus_modification.h"

#include <iostream>
#include <cstdlib>

int rand(int l, int r) // [l,r]
{
	return l + rand() % (r - l + 1);
}

template <class T, class Generator, class Comparator = std::less<T> > 
int test_rmq_assign_modification(int size, int requests, const T& initial_value, Generator generator, Comparator comparator = Comparator())
{
	std::vector <T> array(size, initial_value);
	RMQAssignModification<T, Comparator> tree(size, initial_value, comparator); 
	for (int request_number = 0; request_number	< requests; request_number ++)
	{
		int l = rand(0, size - 1);
		int r = rand(l + 1, size);
		if (rand() % 2 == 0) 
		{
			T value = generator();
			for (int i = l; i < r; i ++)
				array[i] = value;
			tree.apply(l, r, value); 
		}
		else
		{
			T tree_request_value = tree.request(l, r);
			T array_request_value = array[l];
			for (int i = l + 1; i < r; i ++)
				if (comparator(array[i], array_request_value))
					array_request_value = array[i];
			if (array_request_value != tree_request_value)
			{
				std::cerr << "Error: incorrent value on step " << request_number + 1 << ". Expected value: " << array_request_value << ", got: " << tree_request_value  << std::endl;
				return 0;
			}
		}
	}
	return 1;
}

template <class T, class Generator, class Comparator = std::less<T>, class BinaryOperation = std::plus<T> > 
int test_rmq_plus_modification(int size, int requests, const T& initial_value, Generator generator, Comparator comparator = Comparator(), BinaryOperation binary_operation = BinaryOperation())
{
	std::vector <T> array(size, initial_value);
	RMQPlusModification<T, Comparator,BinaryOperation> tree(size, initial_value, comparator, binary_operation); 
	for (int request_number = 0; request_number	< requests; request_number ++)
	{
		int l = rand(0, size - 1);
		int r = rand(l + 1, size);
		if (rand() % 2 == 0) 
		{
			T value = generator();
			for (int i = l; i < r; i ++)
				array[i] = binary_operation(array[i],value);
			tree.apply(l, r, value); 
		}
		else
		{
			T tree_request_value = tree.request(l, r);
			T array_request_value = array[l];
			for (int i = l + 1; i < r; i ++)
				if (comparator(array[i], array_request_value))
					array_request_value = array[i];
			if (array_request_value != tree_request_value)
			{
				std::cerr << "Error: incorrent value on step " << request_number + 1 << ". Expected value: " << array_request_value << ", got: " << tree_request_value  << std::endl;
				return false;
			}
		}
	}
	return true;
}

int main()
{
	for (int i = 2; i <= 100; i ++) 
	{
		if (!test_rmq_assign_modification<int>(i, 100, 0, [](){return rand() % 100; }, std::greater<int>())) return 1;
		if (!test_rmq_assign_modification<int>(i, 100, 0, [](){return rand() % 100; }, std::less<int>())) return 1;
		if (!test_rmq_plus_modification<int>(i, 100, 0, [](){return rand() % 100; }, std::greater<int>())) return 1;
		if (!test_rmq_plus_modification<int>(i, 100, 0, [](){return rand() % 100; }, std::less<int>())) return 1;
		if (!test_rmq_plus_modification<long long>(i, 100, 0, [](){return rand() % 10; }, std::greater<long long>(), std::multiplies<long long>())) return 1;
		if (!test_rmq_plus_modification<long long >(i, 10, 0, [](){return rand() % 10; }, std::less<long long>(), std::multiplies<long long>())) return 1;
	}
	std::cerr << "Tests OK" << std::endl;
	return 0;
};
