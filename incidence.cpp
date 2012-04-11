#include "incidence.hpp"
#include "macro.hpp"
#include <algorithm>
#include <memory>

typedef incidence::iterator iter;

iter::iterator(const iter& it_): it(it_.it ? it_.it->copy() : nullptr)
{
}
iter& iter::operator= (const iter& it_)
{
	it = std::unique_ptr<incidence::iterator_base>(it_.it ? it_.it->copy() : nullptr);
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
