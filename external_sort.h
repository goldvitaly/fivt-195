/* 
 * File:   external_sort.h
 * Author: riad
 *
 * Created on October 3, 2011, 10:42 AM
 */

#ifndef EXTERNAL_SORT_H
#define	EXTERNAL_SORT_H

#include "FileExtSorter.h"
#include <iostream>
#include <queue>
#include <vector>
#include <algorithm>
#include "StdSort.h"

template <typename T,  typename Source, typename ExtSorter,typename Sort = StdSort<T> >
void external_sort(int blockSize,Source& file, ExtSorter Sorter, Sort SortClass=Sort()) {
	std::vector<T> range;
	typedef IO<T>* File;
	typedef std::pair<T, File> FileValue;

	//Function to compare Files by value
	auto comparer = [SortClass](FileValue a, FileValue b){return SortClass.greater()(a.first, b.first);};
	std::priority_queue <
					FileValue,
					std::vector<FileValue>,
					__typeof__(comparer)
					>
					heap(comparer);
	while (true) {
		File out = Sorter.getNextFile();
		bool finish = false;
		int curPos;
		for (curPos = 0; curPos < blockSize; ++curPos) {
			file.read(range[curPos]);
			if (!file.ok()) {
				finish = true;
				break;
			}
		}
		if (!curPos)
			break;

		SortClass.sort(range.begin(), range.end() + curPos);
		for (int j = 0; j < curPos; ++j) {
			out->write(range[j]);
		}
		heap.push(std::make_pair(range[0], out));
		out->toStart();
		out->read(range[0]);
		if (finish)
			break;

	}


	file.clear();

	while (!heap.empty()) {
		FileValue curTop = heap.top();
		heap.pop();
		file.write(curTop.first);
		curTop.second->read(curTop.first);
		if (curTop.second->ok()) {
			heap.push(curTop);
		}
	}
}

template <typename T>
void default_external_sort(int blockSize, std::string file) {
	FileIO<T > input(file, FileIO<T>::permanence::PERMANENT);
	external_sort<T> (blockSize, input, FileExtSorter<T> (), StdSort<T> ());
}
#endif	/* EXTERNAL_SORT_H */
