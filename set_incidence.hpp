#ifndef SET_INCIDENCE_HPP
#define SET_INCIDENCE_HPP

#include <set>
#include "incidence.hpp"

class set_incidence: public incidence
{
private:
	std::set<size_t> adjacent;
public:
	class iterator_base;
	
	~set_incidence() {}
	
	void add_edge(size_t to)
	{
		adjacent.insert(to);
	}
	void delete_edge(size_t to)
	{
		adjacent.erase(to);
	}
	
	iterator begin() const;
	iterator end() const;
};

class set_incidence::iterator_base: public incidence::iterator_base
{
private:
	std::set<size_t>::const_iterator it;
public:
	iterator_base(std::set<size_t>::const_iterator it_)
	{
		it = it_;
	}
	~iterator_base() {}
	
	incidence::iterator_base* copy() const
	{
		return new iterator_base(it);
	}
	
	size_t operator* ()
	{
		return *it;
	}
	bool operator!= (const incidence::iterator_base& it_) const
	{
		return it != dynamic_cast<const iterator_base&>(it_).it;
	}
	iterator_base& operator++ ()
	{
		++it;
		return *this;
	}
};

incidence::iterator set_incidence::begin() const
{
	return incidence::iterator(new set_incidence::iterator_base(adjacent.begin()));
}
incidence::iterator set_incidence::end() const
{
	return incidence::iterator(new set_incidence::iterator_base(adjacent.end()));
}

#endif
