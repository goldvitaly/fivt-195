#ifndef INCIDENTS_HPP
#define INCIDENTS_HPP
#include "Edge.hpp"
#include <memory>
#include <utility>
template <typename Weight>
class Incidents {
public:
	class BaseIterator {
	public:
		virtual void operator ++ () = 0;
		virtual Edge<Weight> operator * () const = 0;
		virtual bool operator != (const BaseIterator&) const = 0;
		virtual std::unique_ptr<BaseIterator> copy() const = 0;
		virtual ~BaseIterator(){}
	};
	class Iterator {
		typedef std::unique_ptr<BaseIterator> IteratorPointer;
	public:
		IteratorPointer it;
		
		explicit Iterator(IteratorPointer iter): it(std::move(iter)){}
		void operator ++ (){
			++(*it);
		}
		Edge<Weight> operator * () const {
			return **it;
		}
		bool operator != (const Iterator& second) const {
			return (*it) != *(second.it);
		}
		Iterator (Iterator&& m){
			it = std::move(m.it);
		}
		Iterator& operator=(Iterator&& m){
			it = std::move(m.it);
			return *this;
		}
		Iterator copy() const {
			return Iterator(IteratorPointer(it->copy()));
		}
		Iterator(const Iterator&) = delete;
		Iterator& operator=(const Iterator&) = delete;
	};
	virtual Iterator begin() const = 0;
	virtual Iterator end() const = 0;
	virtual size_t size() const = 0;
	virtual void addEdge(size_t to, const Weight& = Weight()) = 0;
	virtual void removeEdge(size_t to) = 0;
	virtual bool checkEdge(size_t to) const = 0;
	virtual ~Incidents(){}
};

#endif /* INCIDENTS_HPP */