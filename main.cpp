#include "graph.h"
#include "shortest_paths_finder.h"
#include "test_short_paths.h"
#include <map>
#include <vector>
#include <algorithm>
#include <ctime>

using namespace std;


Graph<int, int> g;

int main()
{
    srand(time(NULL));
    Test_For_Graph<int, int> test(&g);
    test.run();
    return 0;
}
