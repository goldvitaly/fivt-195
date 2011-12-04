#include "filemanager.h"
#include <ios>
#include <vector>
#include <iostream>

template <typename Type, class Sorter>
void externalSort(std::istream& input, std::ostream& output, Sorter sorter, int blocksize = 3)
{
	filemanager<Type> mng;
	std::vector<Type> buffer;
	Type next;
	int id;
	while(!input.eof())
	{
		std::cerr << "Reading!" << std::endl;
		while(input >> next && buffer.size() < blocksize);
			std::cout << next;
			buffer.push_back(next);
		std::cerr << buffer.size() << std::endl;
		sorter(&buffer[0], &buffer[buffer.size() - 1]);
		id = mng.open_next();
		for(int i = 0; i < buffer.size(); i++)
		{
			mng.write(id, buffer[i]);
		}
	}
	for(int i=0; i < id; i++)
	{
		mng.close(i);
	}
}
