#ifndef ITERATOR_HPP
#define ITERATOR_HPP

#include <memory>

namespace graph
{
template<typename TEdge>
class Iterator
{
public: // declaration of methods
	virtual ~Iterator() {}
	
	virtual std::unique_ptr<Iterator> clone() const = 0;
	
	//virtual Iterator& operator= (const Iterator&) = 0; // it seems that default "=" is enough for my needs
	
	virtual Iterator& operator++ () = 0;
	virtual const TEdge& operator* () const = 0; // TODO: add duplicated non-const method to be able to change info in edges
	virtual bool operator!= (const Iterator&) const = 0;
};
}
#endif
