/* 
 * File:   FileStorageManager.h
 * Author: riad
 *
 * Created on November 27, 2011, 10:48 PM
 */

#ifndef FILESTORAGEMANAGER_H
#define	FILESTORAGEMANAGER_H
#include <string>
#include <cstdio>


template <typename T, typename Storage>
class FileStorageManager {
	std::string prevName;

	void nextName(std::string& name) const {
		for (int i = 0; i < name.length(); ++i) {
			if (name[i] != 'z') {
				++name[i];
				return;
			}
			name[i] = 'a' ;
		}
		name += 'a';
	}
	public:
	Storage newStorage() {
		nextName(prevName);
		return Storage(prevName);
	}

	~FileStorageManager(){
		std::string clean;
		while(clean!=prevName){
			nextName(clean);
			remove(clean.c_str());
		}
	}

};

#endif	/* FILESTORAGEMANAGER_H */

