//Понимаю, порнография. Прошу прощения: потом поправлю. Пока при проверке программы полезно.

#define private public
#include "rmq_assign_modification.h"


#include <iostream>
#include <cstdlib>

int rand(int l, int r) // [l,r]
{
	return l + rand() % (r - l + 1);
}

template <class T, class Comparator, class Generator> 
int test_rmq_assign_modification(int size, int requests, Generator generator = Generator(),  Comparator comparator = Comparator())
{
	T initial_value;
	std::vector <T> array(size, initial_value);
	RMQAssignModification<T, Comparator> tree(size, initial_value); 
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
				return 1;
			}
		}
	}
}

int main()
{
	for (int i = 2; i <= 50; i ++) 
		if (!test_rmq_assign_modification<int>(i, 100, std::less<int>())) return 1;
	std::cerr << "Tests OK" << std::endl;
	return 0;
};
