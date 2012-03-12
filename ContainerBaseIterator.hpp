#ifndef CONTAINERBASEITERATOR_HPP
#define CONTAINERBASEITERATOR_HPP

#include "Incidents.hpp"
#include <cassert>
#include <iostream>
template <typename T>
class ContainerBaseIterator : public Incidents::BaseIterator {
	public:
		explicit ContainerBaseIterator(const typename T::const_iterator& iter): iter(iter){}
		virtual void operator ++ () {
			++iter;
		}
		virtual size_t operator * () const {
			return *iter;
		}
		virtual bool operator != (const Incidents::BaseIterator& base) const {
			try{
				std::cout<<"ex";
				const ContainerBaseIterator& second = dynamic_cast<const ContainerBaseIterator&>(base);
				return iter != second.iter;
			}
			catch(std::bad_cast){
				assert(false);
			}
		}
	private:
		typename T::const_iterator iter;
};

#endif /* CONTAINERBASEITERATOR_HPP */

