#ifndef SHORTESTPATHSINFO_HPP
#define SHORTESTPATHSINFO_HPP

#include <boost/optional.hpp>

template <typename Length>
class ShortestPathsInfo{
	typedef boost::optional< std::vector<size_t> > PathType;
public:
	ShortestPathsInfo(
			const std::vector<boost::optional<Length> >& v,
			const std::vector<boost::optional<size_t> >& p
			): lengths(v), previous(p){}
	ShortestPathsInfo(
			std::vector<boost::optional<Length> >&& v,
			std::vector<boost::optional<size_t> >&& p
	): lengths(v), previous(p){}
	boost::optional<Length> length(size_t to){
		return lengths[to];
	}
	PathType path(size_t to){
		if(!previous[to])
			return PathType();
		std::vector<size_t> path;
		do{
			path.push_back(to);
			to = *previous[to];
		}
		while(previous[to]);
		std::reverse(previous.begin(), previous.end());
		return PathType(path);
	}
private:
	std::vector< boost::optional<Length> > lengths;
	std::vector< boost::optional<size_t> > previous;
};

#endif /* SHORTESTPATHSINFO_HPP */

