#ifndef VERTEXVEC_H_INCLUDED
#define VERTEXVEC_H_INCLUDED

#include "graph.h"

class VertexVec : public Vertex
{
    typedef unsigned int TypeNameVer;
public:
    void add_neighbour(const TypeNameVer& nameVer)
    {
        neighbours.push_back(nameVer);
    }
    void delete_neighbour(const TypeNameVer& nameVer)
    {
        neighbours.erase(find(neighbours.begin(), neighbours.end(), nameVer));
    }
    bool exist_neighbour(const TypeNameVer& nameVer)
    {
        return (find(neighbours.begin(), neighbours.end(), nameVer) != neighbours.end());
    }
    std::vector<TypeNameVer> list_neighbour() const
    {
        std::vector<TypeNameVer> Vec(neighbours);
        return Vec;
    }
    void for_each_neighbour(UnaryFunc& unaryFunc) const
    {
        for(size_t i = 0; i < neighbours.size(); i++)
            unaryFunc(neighbours[i]);
    }
    size_t degree() const
    {
        return neighbours.size();
    }
    ~VertexVec()
    {
        neighbours.clear();
    }
private:
    std::vector<TypeNameVer> neighbours;
};


#endif // VERTEXVEC_H_INCLUDED
