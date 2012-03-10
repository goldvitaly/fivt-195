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
	iterator(): it(nullptr) {}
	explicit iterator(iterator_base* it_): it(it_) {}
	iterator(const iterator&);
	
	iterator& operator= (const iterator& it_);
	
	size_t operator* () const;
	bool operator!= (const iterator&) const;
	bool operator== (const iterator&) const;
	iterator& operator++ ();
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

#define iter incidence::iterator

iter::iterator(const iter& it_): it(it_.it ? it_.it->copy() : nullptr)
{
}
iter& iter::operator= (const iter& it_)
{
	it = std::unique_ptr<incidence::iterator_base>(it_.it ? it_.it->copy() : nullptr); // why iter::iterator(const iter&) is not enough?
	return *this;
}
size_t iter::operator* () const
{
	m_assert(it, cannot derefernce nullptr);
	return **it;
}
bool iter::operator!= (const iter& it_) const
{
	return (!it ^ !it_.it) || (it && it_.it && (*it) != (*it_.it));
}
bool iter::operator== (const iter& it_) const
{
	return !operator!=(it_);
}
iter& iter::operator++ ()
{
	m_assert(it, cannot increment nullptr);
	++(*it);
	return *this;
}

#undef iter

#endif
