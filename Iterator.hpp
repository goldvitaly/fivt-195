#ifndef ITERATOR_HPP
#define ITERATOR_HPP

#include "iIncidence.hpp"

class Graph::iIncidence::Iterator
{
public: // declaration of methods
	virtual ~Iterator() {}
	
	virtual std::unique_ptr<Iterator> clone() = 0;
	
	//virtual Iterator& operator= (const Iterator&) = 0; // it seems that default "=" is enough for my needs
	
	virtual Iterator& operator++ () = 0;
	virtual size_t operator* () const = 0;
	virtual bool operator!= (const Iterator&) const = 0;
};

#endif
