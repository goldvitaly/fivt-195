#ifndef INCIDENTS_HPP
#define INCIDENTS_HPP
#include <memory>
#include <utility>
class Incidents {
public:
	class BaseIterator {
	public:
		virtual void operator ++ () = 0;
		virtual size_t operator * () const = 0;
		virtual bool operator != (const BaseIterator&) const = 0;
		virtual ~BaseIterator(){}
	};
	class Iterator {
		typedef std::unique_ptr<BaseIterator> Pointer;
	public:
		Pointer it;
		//reaches ownership
		explicit Iterator(Pointer iter): it(std::move(iter)){}
		void operator ++ (){
			++(*it);
		}
		size_t operator * () const {
			return **it;
		}
		bool operator != (Iterator& second) const {
			return (*it) != *(second.it);
		}
		Iterator (Iterator&& m){
			it = std::move(m.it);
		}
		Iterator& operator=(Iterator&& m){
			it = std::move(m.it);
			return *this;
		}
		Iterator(const Iterator&) = delete;
		Iterator& operator=(const Iterator&) = delete;
	};
	virtual Iterator begin() const = 0;
	virtual Iterator end() const = 0;
	virtual size_t size() const = 0;
	virtual void addEdge(size_t to) = 0;
	virtual void removeEdge(size_t to) = 0;
	virtual bool checkEdge(size_t to) const = 0;
	virtual ~Incidents(){}
};

#endif /* INCIDENTS_HPP */