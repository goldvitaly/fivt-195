#ifndef VECTOR_INCIDENCE_HPP
#define VECTOR_INCIDENCE_HPP

#include <vector>
#include "incidence.hpp"

class vector_incidence: public incidence
{
private:
	std::vector<size_t> adjacent;
public:
	class iterator_base;
	
	~vector_incidence() {}
	
	void add_edge(size_t to)
	{
		adjacent.push_back(to);
	}
	void delete_edge(size_t to)
	{
		adjacent.erase(std::remove(adjacent.begin(), adjacent.end(), to), adjacent.begin());
	}
	
	iterator begin() const;
	iterator end() const;
};

class vector_incidence::iterator_base: public incidence::iterator_base
{
private:
	std::vector<size_t>::const_iterator it;
public:
	iterator_base(std::vector<size_t>::const_iterator it_)
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

incidence::iterator vector_incidence::begin() const
{
	return incidence::iterator(new vector_incidence::iterator_base(adjacent.begin()));
}
incidence::iterator vector_incidence::end() const
{
	return incidence::iterator(new vector_incidence::iterator_base(adjacent.end()));
}

#endif
