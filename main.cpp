#include <iostream>
#include <vector>
#include <typeinfo>
#include "graph.hpp"
#include "vector_incidence.hpp"
#include "set_incidence.hpp"
#include "tarjan.hpp"

using namespace std;

class foo
{
public:
	incidence::iterator it;
	foo () {}
	foo (incidence::iterator it_)
	{
		cout << *it_ << endl;
		it = it_;
		cout << "copy done!\n";
		cout << *it << endl;
	}
};

int main()
{
	freopen("input.txt", "r", stdin);
	//freopen("output.txt", "w", stderr);
	graph g;
	
	int n, m;
	cin >> n >> m;
	for (int i = 0; i < n; i++)
		g.add_vertex(new vector_incidence());
	for (int i = 0; i < m; i++)
	{
		int u, v;
		cin >> u >> v;
		g.add_edge(u, v);
	}
	
	//foo(g[0].begin());
	
	auto res = tarjan_scc(g);
	cout << res.size() << endl;
	
	return 0;
}
