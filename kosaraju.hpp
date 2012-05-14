#pragma once

#include "graph.hpp"
#include "base.hpp"

#include <list>

namespace graph
{
namespace algo
{

class KosarajuMaker
{
public:
	explicit KosarajuMaker(const Graph& gr);
	const std::list<std::list<unsigned>>& make();
	
private:
	const Graph& g;
	Graph rev;
	std::list<std::list<unsigned>> result;
};

}
}
