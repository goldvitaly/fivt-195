#ifndef CONTAINERBASEITERATOR_HPP
#define CONTAINERBASEITERATOR_HPP

#include "Incidents.hpp"
#include <cassert>
template <typename T>
class ContainerBaseIterator : public Incidents::BaseIterator {
	typedef typename T::const_iterator Iterator;
	public:
		explicit ContainerBaseIterator(const typename T::const_iterator& iter): iter(iter){}
		virtual void operator ++ () {
			++iter;
		}
		virtual typename std::iterator_traits<Iterator>::value_type operator * () const {
			return *iter;
		}
		virtual bool operator != (const Incidents::BaseIterator& base) const {
			try{
				const ContainerBaseIterator& second = dynamic_cast<const ContainerBaseIterator&>(base);
				return iter != second.iter;
			}
			catch(std::bad_cast){
				assert(false);
			}
		}
	private:
		Iterator iter;
};

#endif /* CONTAINERBASEITERATOR_HPP */

