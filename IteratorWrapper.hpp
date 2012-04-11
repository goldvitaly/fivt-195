#ifndef ITERATOR_WRAPPER_HPP
#define ITERATOR_WRAPPER_HPP

#include "iIncidence.hpp"

class Graph::iIncidence::IteratorWrapper
{
public: // declaration of methods
	IteratorWrapper();
	IteratorWrapper(const IteratorWrapper&);
	explicit IteratorWrapper(Iterator*);
	
	IteratorWrapper& operator= (const IteratorWrapper&);

	IteratorWrapper& operator++ ();
	size_t operator* () const;
	bool operator!= (const IteratorWrapper&) const;
	
private:
	std::unique_ptr<Iterator> it;
};

#endif
