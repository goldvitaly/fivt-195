/* 
 * File:   external_sort.h
 * Author: riad
 *
 * Created on October 3, 2011, 10:42 AM
 */

#ifndef EXTERNAL_SORT_H
#define	EXTERNAL_SORT_H

#include <queue>
#include <vector>
#include "StdSort.h"

//типы Source и тип Storage не обязаны совпадать.
//Manager::newStorage должен возвращать тип Storage
template <typename T, typename Storage, typename Manager, typename Source, typename Sort = StdSort<T> >
void external_sort(int blockSize, Source source, Manager manager=Manager(), Sort sortWay=Sort()) {

	std::vector<T> range(blockSize);
	struct StorageValue{
		Storage storage;
		T value;
		StorageValue(Storage& storage, const T& val):storage(storage),value(val){
			storage.toStart();
			storage.read(value);
		}
	};
	//Function to compare Storages by value
	auto greater = [sortWay](const StorageValue& a,const StorageValue& b){
		return sortWay.less()(b.value, a.value);
	};
	std::priority_queue <
		StorageValue,
		std::vector<StorageValue>,
		__typeof__(greater)
		>
	heap(greater);
	while (true) {
		Storage out = manager.newStorage();
		bool finish = false;
		int curPos;
		for (curPos = 0; curPos < blockSize; ++curPos) {
			source.read(range[curPos]);
			if (!source.ok()) {
				finish = true;
				break;
			}
		}
		if (!curPos)
			break;

		sortWay.sort(range.begin(), range.begin() + curPos);
		for (int j = 0; j < curPos; ++j) {
			out.write(range[j]);
		}
		heap.push(StorageValue(out, range[0]));
		out.toStart();
		out.read(range[0]);
		if (finish)
			break;

	}

	source.clear();

	while (!heap.empty()) {
		StorageValue curTop = heap.top();
		heap.pop();
		source.write(curTop.value);
		curTop.storage.read(curTop.value);
		if (curTop.storage.ok()) {
			heap.push(curTop);
		}
	}
	
}
#include "SimpleFileStorage.h"
#include "FileStorageManager.h"
template <typename T>
void default_external_sort(int blockSize, const std::string& file) {
	SimpleFileStorage<T> input(file);
	
	external_sort<T, SimpleFileStorage<T>> (
		blockSize,
		input,
		FileStorageManager<SimpleFileStorage<T>>()
	);
}
#endif	/* EXTERNAL_SORT_H */
