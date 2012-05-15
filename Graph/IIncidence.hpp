#ifndef IIncidence_HPP
#define IIncidence_HPP

#include "Iterator.hpp"
#include "IteratorWrapper.hpp"

namespace graph
{
template<typename TEdge>
class IIncidence
{
public: // declaration of types
	typedef IteratorWrapper<TEdge> iterator; // TODO: how to make this class private&friend and make public typedef?
	
public: // declaration of methods
	virtual ~IIncidence() {}; // почему нельзя написать деструктор = 0? - DONE

	virtual std::unique_ptr<IIncidence> clone() const = 0;

	virtual void addEdge(TEdge) = 0;
	virtual void delEdge(TEdge) = 0;
	
	virtual size_t size() const = 0;
	
	virtual iterator begin() const = 0; // TODO: make different const and non-const iterators
	virtual iterator end() const = 0;
	
protected:
	//class Iterator;
};
} // namespace graph

#endif
