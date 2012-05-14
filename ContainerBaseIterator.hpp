#ifndef CONTAINERBASEITERATOR_HPP
#define CONTAINERBASEITERATOR_HPP

#include "Incidents.hpp"
#include <cassert>
template <typename T>
class ContainerBaseIterator : public Incidents<typename T::value_type::Weight>::BaseIterator {
	typedef typename T::const_iterator Iterator;
	typedef typename T::value_type ValueType;
	typedef typename Incidents<typename ValueType::Weight>::BaseIterator BaseIterator;
	public:
		explicit ContainerBaseIterator(const Iterator& iter): iter(iter){}
		virtual void operator ++ () {
			++iter;
		}
		virtual ValueType operator * () const {
			return *iter;
		}
		virtual bool operator != (const BaseIterator& base) const {
			try{
				const ContainerBaseIterator& second = dynamic_cast<const ContainerBaseIterator&>(base);
				return iter != second.iter;
			}
			catch(std::bad_cast){
				assert(false);
			}
		}
		virtual ~ContainerBaseIterator(){}
		
		std::unique_ptr<BaseIterator> copy() const {
			return std::unique_ptr<BaseIterator>(new ContainerBaseIterator(iter));
		}
	private:
		Iterator iter;
};

#endif /* CONTAINERBASEITERATOR_HPP */

