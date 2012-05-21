#include <iostream>
#include "Graph/Graph.hpp"
#include "incidences/VectorIncidence.hpp"
#include "algorithm/dijkstra/dijkstra.hpp"
#include "algorithm/maxflow/maxflow.hpp"
#include "tests/maxflow_tester.hpp"

using namespace graph;

typedef std::pair<size_t, int> Edge;


int main()
{
    bool failed = false;
    for (int i = 0; i < 20; i++)
    {
        int vertexNum = rand()%30 + 10;
        int pathsNum = rand()%30 + 10;
        std::cout << "Test #" << i+1 << " (" << vertexNum << ", " << pathsNum << "): ";
        std::cout << std::flush;
        if (maxflow_tests::test(vertexNum, pathsNum))
            std::cout << "OK" << std::endl;
        else
        {
            failed = true;
            std::cout << "WA" << std::endl;
        }
    }
    if (failed)
        std::cout << "Some tests have failed!" << std::endl;
    else
        std::cout << "All tests passed successfully!" << std::endl;
	return 0;
}
