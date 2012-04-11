#include "IteratorWrapper.hpp"
#include "Iterator.hpp"

typedef Graph::iIncidence::IteratorWrapper IteratorWrapper;

IteratorWrapper::IteratorWrapper() {}
IteratorWrapper::IteratorWrapper(const IteratorWrapper& iterator): it(iterator.it ? iterator.it->clone() : nullptr) {}
IteratorWrapper::IteratorWrapper(Iterator* iterator): it(iterator) {} // explicit не нужен в определении конструктора?

IteratorWrapper& IteratorWrapper::operator= (const IteratorWrapper& iterator)
{
	if (iterator.it)
		it = iterator.it->clone(); // в чем отличие от it = std::move(..);
	else
		it.reset(nullptr);
	return *this;
}

IteratorWrapper& IteratorWrapper::operator++ ()
{
	++(*it);
	return *this;
}
size_t IteratorWrapper::operator* () const
{
	return **it;
}
bool IteratorWrapper::operator!= (const IteratorWrapper& iterator) const
{
	return (!it ^ !iterator.it) || (it && iterator.it && *it != *iterator.it);
}
