#ifndef ITERATOR_WRAPPER_HPP
#define ITERATOR_WRAPPER_HPP

#include "Iterator.hpp"

#include <memory>

namespace graph
{
class IteratorWrapper
{
public: // declaration and definition of methods
	IteratorWrapper() {} 
	IteratorWrapper(const IteratorWrapper& iterator): it(iterator.it ? iterator.it->clone() : nullptr) {}
	explicit IteratorWrapper(Iterator* iterator): it(iterator) {}
	
	IteratorWrapper& operator= (const IteratorWrapper& iterator)
	{
		if (iterator.it)
			it = iterator.it->clone(); // в чем отличие от it = std::move(..)? Вроде бы здесь должен move вызываться, а не copy
		else
			it.reset(nullptr);
		return *this;
	}

	IteratorWrapper& operator++ () {
		++(*it); return *this;
	}
	size_t operator* () const {
		return **it;
	}
	bool operator!= (const IteratorWrapper& iterator) const {
		return (!it ^ !iterator.it) || (it && iterator.it && *it != *iterator.it);
	}
	
private:
	std::unique_ptr<Iterator> it;
};
} // namespace graph

#endif
