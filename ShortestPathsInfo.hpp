#ifndef SHORTESTPATHSINFO_HPP
#define SHORTESTPATHSINFO_HPP
#include "Path.hpp"
#include <boost/optional.hpp>
#include <stdexcept>
#include <vector>

template <typename Length, typename Weight>
class ShortestPathsInfo{
	typedef typename Incidents<Weight>::Iterator Iterator;
	typedef std::vector<Iterator> PathType;
public:
	ShortestPathsInfo(
			const std::vector<boost::optional<Length> >& v,
			const std::vector<boost::optional<size_t> >& p,
			const std::vector<Iterator>& lastEdge
			): lengths(v), previous(p), lastEdge(lastEdge){}
	std::vector< boost::optional<Length> > allLengths() const {
		return lengths;
	}
	boost::optional<Length> length(size_t to) const {
		return lengths[to];
	}
	PathType path(size_t to) const {
		if(!lengths[to])
			throw std::invalid_argument("No path");
		PathType path = Path<Weight>(previous, to, lastEdge).path();
		return std::move(path);
	}
private:
	std::vector< boost::optional<Length> > lengths;
	std::vector< boost::optional<size_t> > previous;
	const std::vector<Iterator>& lastEdge;
};

#endif /* SHORTESTPATHSINFO_HPP */
