#include "filemanager.h"
#include <ios>
#include <vector>
#include <fstream>
#include <iostream>
#include <cassert>
#include <queue>

template <class Sorter>
void externalSort(std::fstream& input, std::fstream& output, Sorter sorter, int blocksize = 10000)
{
	typedef typename Sorter::type type;
	typedef typename Sorter::comp comp;
	filemanager<type> mng;
	std::vector<type> buffer;
	type next;
	int id = -1;
	while(!input.eof())
	{
		buffer.clear();
		while(buffer.size() < blocksize && input >> next)
		{
			buffer.push_back(next);
			//std::cerr << next << std::endl;
		}
		if(buffer.size() == 0)
			break;
		sorter(&buffer[0], &buffer[buffer.size() - 1] + 1);
		id = mng.open_next();
		for(int i = 0; i < buffer.size(); i++)
		{
			mng.write(id, buffer[i]);
			//std::cerr << buffer[i] << std::endl;
		}
	}
	for(int i = 0; i <= id; i++)
	{
		mng.reset(i);
		//std::cerr << mng.read(0) << "!!!!!!!" << mng.read_success << std::endl;
	}
	//std::cerr << mng.read(0) << "!!!!!!!" << mng.read_success << std::endl;
	assert(id >= 0);
	size_t chunk = blocksize / (id+1);
	assert(chunk > 0);
	std::priority_queue<std::pair<type, int> > merge; 
	for(int i = 0; i <= id; i++)
	{
		for(int c = 0; c < chunk; c++)
		{
			next = mng.read(i);
			if(mng.read_success)
				merge.push(std::make_pair(next,i));
			//else
			//	break;
		}
	}
	std::pair<type, int> best;
	while(!merge.empty())
	{
		best = merge.top();
		merge.pop();
		next = mng.read(best.second);
		if(mng.read_success)
			merge.push(std::make_pair(next,best.second));
		output << best.first << " ";
	}
	for(int i = 0; i <= id; i++)
	{
		mng.close(i);
	}
}
