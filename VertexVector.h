#ifndef VERTEXVECTOR_H_INCLUDED
#define VERTEXVECTOR_H_INCLUDED

#include "graph.h"
#include <vector>
#include <algorithm>
#include <utility>

template<class Weight>
class VertexVector : public Vertex<Weight>
{
    typedef unsigned int TypeNameVer;
    typedef typename std::vector<std::pair<TypeNameVer, Weight> >::iterator IteratorNeighbours;
public:
    void add_neighbour(const TypeNameVer& nameVer, const Weight& weight)
    {
        neighbours.push_back(std::pair<TypeNameVer, Weight>(nameVer, weight));
    }
    void delete_neighbour(const TypeNameVer& nameVer)
    {
        neighbours.erase(findFirstKey(nameVer));
    }
    bool exist_neighbour(const TypeNameVer& nameVer)
    {
        return (findFirstKey(nameVer) != neighbours.end());
    }
    Weight weight(const TypeNameVer& nameVer) const
    {
        for(int i = 0; i < neighbours.size(); i++)
        {
            if(neighbours[i].first == nameVer)
                return neighbours[i].second;
        }
    }
    IteratorNeighbours findFirstKey(const TypeNameVer& nameVer) // не могу приписать const
    {
        IteratorNeighbours it = neighbours.begin();
        while(it != neighbours.end())
        {
            if(it->first == nameVer)
                return it;
            it++;
        }
        return it;
    }
    std::vector<std::pair<TypeNameVer, Weight> > list_neighbour() const
    {
        std::vector<std::pair<TypeNameVer, Weight> > neighboursCopy(neighbours);
        return neighboursCopy;
    }
    void for_each_neighbour(UnaryFunc& unaryFunc) const
    {
        for(size_t i = 0; i < neighbours.size(); i++)
            unaryFunc(neighbours[i].first);
    }
    size_t degree() const
    {
        return neighbours.size();
    }
private:
    std::vector<std::pair<TypeNameVer, Weight> > neighbours;
};


#endif // VERTEXVECTOR_H_INCLUDED
