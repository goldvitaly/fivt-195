#ifndef IIncidence_HPP
#define IIncidence_HPP

#include "Graph.hpp"

class Graph::IIncidence
{
public: // declaration of types
	class IteratorWrapper;
	typedef IteratorWrapper iterator; // TODO: how to make this class private&friend and make public typedef?
	
public: // declaration of methods
	virtual ~IIncidence() {}; // почему нельзя написать деструктор = 0?

	virtual std::unique_ptr<IIncidence> clone() const = 0;

	virtual void addEdge(size_t) = 0;
	virtual void delEdge(size_t) = 0;
	
	virtual size_t size() const = 0;
	
	virtual IteratorWrapper begin() const = 0; // TODO: make all iterators constant
	virtual IteratorWrapper end() const = 0;
	
protected:
	class Iterator;
};


#endif
