#ifndef SETINCIDENTS_HPP
#define SETINCIDENTS_HPP

#include "ContainerBaseIterator.hpp"
#include "Incidents.hpp"
#include <set>
template<typename Weight>
class SetIncidents : public Incidents<Weight> {
public:
	SetIncidents(){}
	explicit SetIncidents(const std::set<Vertex<Weight> >& v): incidents(v) {}
	
	virtual typename Incidents<Weight>::Iterator begin() const{
		return typename Incidents<Weight>::Iterator(std::unique_ptr<typename Incidents<Weight>::BaseIterator>(new BaseIterator(incidents.begin())));
	}
	virtual typename Incidents<Weight>::Iterator end() const{
		return typename Incidents<Weight>::Iterator(std::unique_ptr<typename Incidents<Weight>::BaseIterator>(new BaseIterator(incidents.end())));
	}
	
	size_t size() const {
		return incidents.size();
	}
	
	virtual void addEdge(size_t to, const Weight& weight = Weight()){
		incidents.insert(Vertex<Weight>(to, weight));
	}
	
	virtual void removeEdge(size_t to){
		incidents.erase(Vertex<Weight>(to));
	}
	
	virtual bool checkEdge(size_t to) const {
		return incidents.find(Vertex<Weight>(to)) != incidents.end();
	}
	
private:
	typedef ContainerBaseIterator<std::set<Vertex<Weight> > > BaseIterator;
	std::set<Vertex<Weight> > incidents;
};

#endif /* SETINCIDENTS_HPP */

