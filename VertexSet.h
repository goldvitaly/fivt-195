#ifndef VERTEXSET_H_INCLUDED
#define VERTEXSET_H_INCLUDED

#include "graph.h"
#include "set"
#include "algorithm"
#include "vector"

class VertexSet : public Vertex
{
    typedef unsigned int TypeNameVer;
public:
    void add_neighbour(const TypeNameVer& nameVer)
    {
        neighbours.insert(nameVer);
    }
    void delete_neighbour(const TypeNameVer& nameVer)
    {
        neighbours.erase(nameVer);
    }
    bool exist_neighbour(const TypeNameVer& nameVer)
    {
        return (neighbours.find(nameVer) != neighbours.end());
    }
    std::vector<TypeNameVer> list_neighbour() const
    {
        std::vector<TypeNameVer> neighboursCopy;
        neighboursCopy.resize(neighbours.size());
        std::copy(neighbours.begin(), neighbours.end(), neighboursCopy.begin());
        return neighboursCopy;
    }
    void for_each_neighbour(UnaryFunc& unaryFunc) const
    {
        std::set<TypeNameVer>::iterator it = neighbours.begin();
        while(it != neighbours.end())
        {
            unaryFunc(*it);
            it++;
        }
    }
    size_t degree() const
    {
        return neighbours.size();
    }
private:
    std::set<TypeNameVer> neighbours;
};


#endif // VERTEXSET_H_INCLUDED
