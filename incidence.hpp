#ifndef INCIDENCE_HPP
#define INCIDENCE_HPP

#include <algorithm>
#include <memory>
#include "macro.hpp"

class incidence
{
public:
	class iterator;
	class iterator_base;
	
	incidence() {}
	virtual ~incidence() {}
	
	virtual void add_edge(size_t) = 0;
	virtual void delete_edge(size_t) = 0;
	
	virtual iterator begin() const = 0;
	virtual iterator end() const = 0;
};

class incidence::iterator
{
private:
	std::unique_ptr<iterator_base> it;

public:
	explicit iterator(iterator_base* it_): it(it_) {}
	iterator(const iterator&);
	
	size_t operator* () const;
	bool operator!= (const iterator&) const;
	iterator& operator++ ();
	
	decltype(it.get()) get() {return it.get();}
};
class incidence::iterator_base
{
public:
	virtual ~iterator_base() {}
	virtual iterator_base* copy() const = 0;
	
	virtual size_t operator* () = 0;
	virtual bool operator!= (const iterator_base&) const = 0;
	virtual iterator_base& operator++ () = 0;
};

incidence::iterator::iterator(const incidence::iterator& it_): it(it_.it->copy())
{
}
size_t incidence::iterator::operator* () const
{
	return **it;
}
bool incidence::iterator::operator!= (const iterator& it_) const
{
	return (*it) != (*it_.it);
}
incidence::iterator& incidence::iterator::operator++ ()
{
	++(*it);
	return *this;
}

#endif
