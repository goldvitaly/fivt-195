
#ifndef LAZYPATH_HPP
#define LAZYPATH_HPP

#include <boost/optional.hpp>
#include <memory>
#include <vector>

class Path{
public:
	Path(std::vector< boost::optional<size_t> > parents, size_t to):
		parents(parents), to(to), filled(false)
	{}
	
	size_t operator[] (size_t index) const {
		fill();
		return path[index];
	}
	
	std::vector<size_t> toVector() const {
		fill();
		return path;
	}
	
	size_t size() const {
		fill();
		return path.size();
	}
	
	size_t front() const {
		fill();
		return path.front();
	}
	
	size_t back() const {
		fill();
		return path.back();
	}
private:
	void fill() const {
		if(!filled){
			filled = true;
			size_t cur = to;
			while(parents[cur]){
				path.push_back(cur);
				cur = *parents[cur];
			}
			path.push_back(cur);
			std::reverse(path.begin(), path.end());
		}
	}
	const std::vector<boost::optional<size_t> >& parents;
	size_t to;
	
	//cached path
	mutable bool filled;
	mutable std::vector<size_t> path;
};
#endif /* LAZYPATH_HPP */