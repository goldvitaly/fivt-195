#include <ctime>
#include <cassert>
#include <fstream>
#include <vector>
#include <string>
#include <queue>
#include "myio.h"
#include "sort_algo.h"

using namespace std;

enum{blockSize = 1000000};
const char *file1, *file2;

int main(int argc, char* argv[])
{
	assert(argc > 1);
	file1 = argv[1];
	
	file2 = (argc > 2 ? argv[2] : "temp.txt");
	
	fstream in(file1, ios :: in | ios :: binary);
	long long n = readI64(in);
	
	cerr << "N = " << n << endl;
	
	int numBlocks = n / blockSize;
	
	vector<fstream*> blocks;
	vector<int> leftInBlock;
	for (int i = 0; i < numBlocks; i++)
	{
		blocks.push_back(new fstream(file2, ios :: in | ios :: out | ios :: binary));
		blocks.back()->seekg(4 * (2 + i * blockSize), ios :: beg);
		leftInBlock.push_back(min((long long) blockSize, n - i * blockSize));
		sortSegment(in, *blocks.back(), leftInBlock.back());
		blocks.back()->seekg(4 * (2 + i * blockSize), ios :: beg);
	}
	
	in.close();
	in.open(file1, ios :: out | ios :: binary);
	writeI64(in, n);
	
	priority_queue<pair<int, int>, vector<pair<int, int> >, greater<pair<int, int> > > q;
	for (int i = 0; i < numBlocks; i++)
	{
		q.push(make_pair(readInt(*blocks[i]), i));
	}
	while (!q.empty())
	{
		writeInt(in, q.top().first);
		int t = q.top().second;
		q.pop();
		if (--leftInBlock[t])
			q.push(make_pair(readInt(*blocks[t]), t));
	}
	
	for (int i = 0; i < numBlocks; i++)
	{
		blocks[i]->close();
		delete blocks[i];
	}
	in.close();
	
	cerr << "Sorted " << n << " integers in " << clock() / 1000 << " ms!!!\n";
	cerr << "Block size = " << blockSize << endl;
	
	return 0;
}
