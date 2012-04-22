#ifndef VERTEXMAP_H_INCLUDED
#define VERTEXMAP_H_INCLUDED

#include <map>
#include "graph.h"
#include <set>
#include <algorithm>
#include <vector>

template<class Weight>
class VertexMap : public Vertex<Weight>
{
    typedef unsigned int TypeNameVer;
    typedef typename std::map<TypeNameVer, Weight>::iterator IteratorNeighbours;
public:
    void add_neighbour(const TypeNameVer& nameVer, const Weight& weight)
    {
        neighbours.insert(std::pair<TypeNameVer, Weight>(nameVer, weight));
    }
    void delete_neighbour(const TypeNameVer& nameVer)
    {
        neighbours.erase(nameVer);
    }
    bool exist_neighbour(const TypeNameVer& nameVer)
    {
        return (neighbours.find(nameVer) != neighbours.end());
    }
    std::vector<std::pair<TypeNameVer, Weight> > list_neighbour() const
    {
        std::vector<std::pair<TypeNameVer, Weight> > neighboursCopy;
        neighboursCopy.resize(neighbours.size());
        std::copy(neighbours.begin(), neighbours.end(), neighboursCopy.begin());
        return neighboursCopy;
    }
    void for_each_neighbour(UnaryFunc& unaryFunc) const
    {
        std::vector<std::pair<TypeNameVer, Weight> > neighboursCopy;
        neighboursCopy.resize(neighbours.size());
        std::copy(neighbours.begin(), neighbours.end(), neighboursCopy.begin());
        for(int i= 0; i < neighboursCopy.size(); i++)
        {
            unaryFunc(neighboursCopy[i].first);
        }
    }
    size_t degree() const
    {
        return neighbours.size();
    }
private:
    std::map<TypeNameVer, Weight> neighbours;
};


#endif // VERTEXMAP_H_INCLUDED
