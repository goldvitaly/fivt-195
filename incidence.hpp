#ifndef INCIDENCE_HPP
#define INCIDENCE_HPP

#include "macro.hpp"

/**
 * @brief	Abstract virtual class representing graph's vertex's incidence list
 */
class Incidence
{
public:
	class Iterator_base
	{
	public:
		virtual size_t operator* () = 0;
		virtual Iterator_base& operator++ () = 0;
		virtual bool operator != (const Iterator_base&) const = 0;
		virtual ~Iterator_base () {}
	};
	class Iterator
	{
	public:
		Iterator_base *iterator;

		Iterator(Iterator_base *iterator_)
		{
			iterator = iterator_;
		}
		
		size_t operator*()
		{
			return **iterator;
		}
		Iterator operator++ ()
		{
			++(*iterator);
			return *this;
		}
		bool operator != (Iterator it) const
		{
			return *iterator != *(it.iterator);
		}
		
		~Iterator ()
		{
			//delete iterator;
		}
	};
	
	virtual Iterator begin() = 0;
	virtual Iterator end() = 0;
};

#endif
