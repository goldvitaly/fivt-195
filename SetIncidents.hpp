#ifndef SETINCIDENTS_HPP
#define SETINCIDENTS_HPP
#include <set>
class SetIncidents : public Incidents {
public:
	SetIncidents(){}
	explicit SetIncidents(const std::set<size_t>& v): incidents(v) {}
	
	virtual Iterator begin() const{
		return Iterator(new ContainerBaseIterator<std::set<size_t>>(incidents.begin()));
	}
	virtual Iterator end() const{
		return Iterator(new ContainerBaseIterator<std::set<size_t>>(incidents.end()));
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
	std::set<size_t> incidents;
};

#endif /* SETINCIDENTS_HPP */

