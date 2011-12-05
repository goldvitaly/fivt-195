#include "filemanager.h"
#include <ios>
#include <vector>
#include <fstream>
#include <iostream>

template <typename Type,class Comparator>
void merge(Type** arrays, size_t* sizes, int count){
	
}

template <class Sorter>
void externalSort(std::fstream& input, std::fstream& output, Sorter sorter, int blocksize = 3)
{
	typedef typename Sorter::type type;
	typedef typename Sorter::comp comp;
	filemanager<type> mng;
	std::vector<type> buffer;
	type next;
	int id;
	while(!input.eof())
	{
		buffer.clear();
		while(buffer.size() < blocksize && input >> next)
		{
			buffer.push_back(next);
		}
		if(buffer.size() == 0)
			break;
		sorter(&buffer[0], &buffer[buffer.size() - 1] + 1);
		id = mng.open_next();
		for(int i = 0; i < buffer.size(); i++)
		{
			mng.write(id, buffer[i]);
			//std::cerr << "Writing " << buffer[i] << " to " << id << std::endl;
		}
	}
	for(int i=0; i <= id; i++)
	{
		mng.reset(i);
	}
	
	for(int i=0; i <= id; i++)
	{
		mng.close(i);
	}
}
