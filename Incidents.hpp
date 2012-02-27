#ifndef INCIDENTS_HPP
#define INCIDENTS_HPP
//dummy
class Incidents {
public:
	class Iterator {
	public:
		virtual bool is_valid() const = 0;
		virtual void increase() = 0;
		virtual size_t value() const = 0;
		virtual ~Iterator(){}
	};
	virtual Iterator* createIterator() const = 0;
	virtual size_t count() const = 0;
	virtual void addEdge(size_t to) = 0;
	virtual void removeEdge(size_t to) = 0;
	virtual bool checkEdge(size_t to) const = 0;
	virtual ~Incidents(){}
};

#endif /* INCIDENTS_HPP */

