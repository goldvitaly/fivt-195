#include "test_max_flow.h"
#include <map>
#include <vector>
#include <algorithm>
#include <ctime>

using namespace std;


Graph<int> g;

int main()
{
    srand(time(NULL));
    Test_For_Graph<int> test(&g);
    test.run();
    return 0;
}
