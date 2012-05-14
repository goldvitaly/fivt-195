#ifndef PATH_HPP
#define PATH_HPP

#include <boost/optional.hpp>
#include <memory>
#include <vector>

template<typename Weight>
class Path{
public:
	typedef typename Incidents<Weight>::Iterator Iterator;
	Path(const std::vector< boost::optional<size_t> >& parents, size_t to, const std::vector<Iterator>& lastEdge):
		parents(parents), lastEdge(lastEdge), to(to)
	{}
		
	std::vector<Iterator> path() const {
		std::vector<Iterator> curpath;
		size_t cur = to;
		while(parents[cur]){
			curpath.push_back(lastEdge[cur].copy());
			cur = *parents[cur];
		}
		std::reverse(curpath.begin(), curpath.end());
		return curpath;
	}
private:
	const std::vector<boost::optional<size_t> >& parents;
	const std::vector<Iterator>& lastEdge;
	size_t to;
};
#endif /* PATH_HPP */