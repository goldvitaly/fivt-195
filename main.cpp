#include "graph.h"
#include "algo_for_graph.h"
#include "test_for_graph.h"
#include <map>
#include <vector>
#include <algorithm>
#include <ctime>

using namespace std;


Graph<int> g;

int main(int argc, char** args)
{
    srand(time(0));
    Test_For_Graph<int> test = Test_For_Graph<int>(&g);
    if(test.run())  cout << "FAIL";
    else            cout << "OK";
    return 0;
}
