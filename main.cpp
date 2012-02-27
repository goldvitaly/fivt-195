#include "graph.h"
#include <map>
using namespace std;


Graph <int> a;
int res, n, x, m;



void dfs(Graph<int>::Node* u, int col)
{
    u -> color_  = col;
    for(map<Graph<int>::Node*, int>::iterator it = u -> out_edges_.begin(); it != u -> out_edges_.end(); it++)
        if(it -> first -> color_ == 0)  dfs(it -> first, col + 1);
    res = max(res, col);
}
int main()
{
    cin >> n;
    for(int i = 1; i <= n; i++)
        a.add_node(i, 0);
    for(int i = 1; i <= n; i++)
        {
            cin >> x;
            if(x != -1)   a.add_edge(x, i);
        }
    for(int i = 1; i <= n; i++)
        if(a.find(i) -> num_of_inc_edges() == 0)
            dfs(a.find(i), 1);
    cout << res;
    return 0;
}
