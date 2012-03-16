#ifndef SETINCIDENTS_HPP
#define SETINCIDENTS_HPP

#include "ContainerBaseIterator.hpp"
#include "Incidents.hpp"
#include <set>
class SetIncidents : public Incidents {
public:
	SetIncidents(){}
	explicit SetIncidents(const std::set<size_t>& v): incidents(v) {}
	
	virtual Iterator begin() const{
		return Iterator(std::unique_ptr<BaseIterator>(new BaseIterator(incidents.begin())));
	}
	virtual Iterator end() const{
		return Iterator(std::unique_ptr<BaseIterator>(new BaseIterator(incidents.end())));
	}
	
	size_t size() const {
		return incidents.size();
	}
	
	virtual void addEdge(size_t to){
		incidents.insert(to);
	}
	
	virtual void removeEdge(size_t to){
		incidents.erase(to);
	}
	
	virtual bool checkEdge(size_t to) const {
		return incidents.find(to) != incidents.end();
	}
	
private:
	typedef ContainerBaseIterator<std::set<size_t>> BaseIterator;
	std::set<size_t> incidents;
};

#endif /* SETINCIDENTS_HPP */

