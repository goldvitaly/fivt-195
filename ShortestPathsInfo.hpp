#ifndef SHORTESTPATHSINFO_HPP
#define SHORTESTPATHSINFO_HPP
#include "Path.hpp"
#include <boost/optional.hpp>
#include <vector>

template <typename Length>
class ShortestPathsInfo{
	typedef boost::optional< std::vector<size_t> > PathType;
public:
	ShortestPathsInfo(
			const std::vector<boost::optional<Length> >& v,
			const std::vector<boost::optional<size_t> >& p
			): lengths(v), previous(p){}
	std::vector< boost::optional<Length> > allLengths() const {
		return lengths;
	}
	boost::optional<Length> length(size_t to) const {
		return lengths[to];
	}
	PathType path(size_t to) const {
		if(!lengths[to])
			return PathType();
		std::vector<size_t> path = Path(previous, to).toVector();
		return path;
	}
private:
	std::vector< boost::optional<Length> > lengths;
	std::vector< boost::optional<size_t> > previous;
};

#endif /* SHORTESTPATHSINFO_HPP */
