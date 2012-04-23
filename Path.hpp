#ifndef PATH_HPP
#define PATH_HPP

#include <boost/optional.hpp>
#include <memory>
#include <vector>

class Path{
public:
	Path(const std::vector< boost::optional<size_t> >& parents, size_t to):
		parents(parents), to(to)
	{}
		
	std::vector<size_t> path() const {
		std::vector<size_t> curpath;
		size_t cur = to;
		while(parents[cur]){
			curpath.push_back(cur);
			cur = *parents[cur];
		}
		curpath.push_back(cur);
		std::reverse(curpath.begin(), curpath.end());
		return curpath;
	}
private:
	const std::vector<boost::optional<size_t> >& parents;
	size_t to;
};
#endif /* PATH_HPP */