#ifndef SETINCIDENTS_HPP
#define SETINCIDENTS_HPP
#include <set>
class SetIncidents : public Incidents {
public:
	class BaseIterator : public Incidents::BaseIterator {
	public:
		explicit BaseIterator(const std::set<size_t>::const_iterator& iter): iter(iter){}
		virtual void operator ++ () {
			++iter;
		}
		virtual size_t operator * () const {
			return *iter;
		}
		virtual bool operator != (const Incidents::BaseIterator& base) const {
			try{
				const BaseIterator& second = dynamic_cast<const BaseIterator&>(base);
				return iter != second.iter;
			}
			catch(std::bad_cast){
				return false;
			}
		}
	private:
		std::set<size_t>::const_iterator iter;
	};

	SetIncidents(){}
	explicit SetIncidents(const std::set<size_t>& v): incidents(v) {}
	
	virtual Iterator begin() const{
		return Iterator(new BaseIterator(incidents.begin()));
	}
	virtual Iterator end() const{
		return Iterator(new BaseIterator(incidents.end()));
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

