#include "graph.h"
#include <map>
#include <vector>
#include <algorithm>

using namespace std;


int n, m, val;
Graph<int> g;
int main(int argc, char** args) //simple program getting number of components in non-oriented graph...
{
    cin >> n;
    for(int i = 0; i < n; i++)
    {
        g.add_node(i, new SetNode<int>(i));
    }
    for(int i = 0; i < n; i++)
        for(int j = 0; j < n; j++)
        {
            cin >> val;
            if(val)
                g.add_edge(i, j);
        }
    int ans = 0;
    g.make_dfs(NULL, &ans);
    cout << ans;
    return 0;
}



