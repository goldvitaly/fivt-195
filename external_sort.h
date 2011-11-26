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
#include <algorithm>
#include "StdSort.h"
namespace ExternalSort {

	template <typename T,  typename Source, typename ExtSorter,typename Sort = StdSort<T> >
	void external_sort(int blockSize,Source& file, ExtSorter Sorter, Sort SortClass=Sort()) {
		T* tmp_range = new T[blockSize];
		auto comparer = [SortClass](std::pair<T, IO<T>* > a, std::pair<T, IO<T>* >b){return SortClass.greater()(a.first, b.first);};
		std::priority_queue <
						std::pair<T, IO<T>* >,
						std::vector<std::pair<T, IO<T>* > >,
						__typeof__(comparer)
						> heap(comparer);
		while (true) {
			IO<T>* out = Sorter.getNew();
			bool finish = false;
			int i;
			for (i = 0; i < blockSize; ++i) {
				file.read(tmp_range[i]);
				if (!file.ok()) {
					finish = true;
					break;
				}
			}
			if (!i)
				break;
			//std::sort(tmp,tmp+i);
			SortClass.sort(tmp_range, tmp_range + i);
			for (int j = 0; j < i; ++j) {
				out->write(tmp_range[j]);
			}
			heap.push(std::make_pair(tmp_range[0], out));
			out->toStart();
			out->read(tmp_range[0]);
			if (finish)
				break;

		}


		delete[] tmp_range;
		file.clear();

		while (!heap.empty()) {
			std::pair<T, IO<T>*> curTop = heap.top();
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
		FileIO<T > input(file, FileIO<T>::type::STABLE);
		external_sort<T> (blockSize, input, FileExtSorter<T > (), StdSort<T > ());
	}

};
#endif	/* EXTERNAL_SORT_H */
