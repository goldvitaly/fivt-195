#ifndef IINCIDENCE_HPP
#define IINCIDENCE_HPP

#include "Graph.hpp"

class Graph::iIncidence
{
public: // declaration of types
	class IteratorWrapper;
	typedef IteratorWrapper iterator; // TODO: how to make this class private&friend and make public typedef?
	
public: // declaration of methods
	virtual ~iIncidence() {}; // почему нельзя написать деструктор = 0?

	virtual std::unique_ptr<iIncidence> clone() const = 0;

	virtual void addEdge(size_t) = 0;
	virtual void delEdge(size_t) = 0;
	
	virtual size_t size() const = 0;
	
	virtual IteratorWrapper begin() const = 0; // TODO: make all iterators constant
	virtual IteratorWrapper end() const = 0;
	
protected:
	class Iterator;
};


#endif
