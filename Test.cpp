#include <iostream>
#include <map>
#include <vector>
#include <set>
#include <algorithm>
#include <ctime>

#include "graph.h"
#include "VertexVec.h"
#include "VertexSet.h"
#include "AlgTarStrConComp.h"
#include "SlowAlg.h"

void make_random_graph(Graph<Vertex>& graph, int numVer)
{
    srand(time(NULL));
    for(int i = 0; i < numVer; i++)
    {
        if(i%2 == 0)
            graph.add_vertex(i, new VertexVec());
        else
            graph.add_vertex(i, new VertexSet());
    }
    for(int i = 0; i < 3*numVer; i++)
    {
        int in_ver = rand() % numVer;
        int out_ver = rand() % numVer;
        graph.add_edge(out_ver, in_ver);
    }
}

void sortVectorOfVector(std::vector<std::vector<unsigned int> >& Vec)
{
    for(size_t i = 0; i < Vec.size(); i++)
    {
        sort(Vec[i].begin(), Vec[i].end());
    }
    sort(Vec.begin(), Vec.end());
}

bool equiv(std::vector<std::vector<unsigned int> >& strConCom, std::vector<std::vector<unsigned int> >& strConCom2)
{
    if(strConCom.size() != strConCom2.size())
        return false;
    for(size_t i = 0; i < strConCom.size(); i++)
    {
        if(strConCom[i] != strConCom2[i])
        {
            return false;
        }
    }
    return true;
}

int main()
{
    for(int trial = 0; trial < 1000; trial++)
    {
        Graph<Vertex> graph;
        const int numVer = 5;
        make_random_graph(graph, numVer);

        AlgTarStrConComp algTar(graph);
        std::vector<std::vector<unsigned int> > strConCom = algTar.str_con_com();

        SlowAlg slowAlg(graph);
        std::vector<std::vector<unsigned int> > strConCom2 = slowAlg.str_con_com();

        sortVectorOfVector(strConCom);
        sortVectorOfVector(strConCom2);
        if(equiv(strConCom, strConCom2) == false)
        {
            for(int i = 0; i < (int)strConCom.size(); i++)
            {
                std::cout << "component " << i + 1 << " : ";
                for(int j = 0; j < (int)strConCom[i].size(); j++)
                    std::cout << strConCom[i][j] << " ";
                std::cout << std::endl;
            }
            std::cout << std::endl;
            for(int i = 0; i < (int)strConCom2.size(); i++)
            {
                std::cout << "component " << i + 1 << " : ";
                for(int j = 0; j < (int)strConCom2[i].size(); j++)
                    std::cout << strConCom2[i][j] << " ";
                std::cout << std::endl;
            }
            return 0;
        }
    }
    std::cout << "Ok" << std::endl;
    return 0;
}
