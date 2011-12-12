#include <iostream>
#include <algorithm>
#include <ctime>
#include <cstdlib>
#include "MinSumTree.h"

using namespace std;

int main()
{
    srand(time(NULL));
    MinSumTree intervalTree(100);
    SlowlyTree slowlyTree(100);
    for(size_t i = 0; i < 500; i++)
    {
        int limit1 = rand() % 100 + 1;
        int limit2 = rand() % 100 + 1;
        if(limit2 < limit1)
            swap(limit1, limit2);
        if(limit1 != limit2)
        {
            int mod = rand() % 100 - 50;
            int choose = rand() % 3;
            if(choose == 0)         // +=
            {
                intervalTree.update_add(mod, limit1, limit2);
                slowlyTree.update_add(mod, limit1, limit2);
            }
            else if(choose == 1)    // :=
            {
                intervalTree.update_assign(mod, limit1, limit2);
                slowlyTree.update_assign(mod, limit1, limit2);
            }
            else
            {
                if(intervalTree.query_sum(limit1, limit2) != slowlyTree.query_sum(limit1, limit2))
                    exit(1);
                if(intervalTree.query_min(limit1, limit2) != slowlyTree.query_min(limit1, limit2))
                    exit(1);
            }
        }
        else
            i--;
    }
    cout << "Ok" << endl;
   return 0;
}
