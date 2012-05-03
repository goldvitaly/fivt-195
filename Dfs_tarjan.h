#ifndef DFS_TARJAN_H_INCLUDED	
#define DFS_TARJAN_H_INCLUDED

#include <vector>
#include <stack>
#include <limits>
#include "Graph.h"

typedef std::vector<size_t> TypeComponent;
const size_t nonParent = std::numeric_limits<size_t>::max();
const size_t infinity = std::numeric_limits<size_t>::max();

struct StatusVertex
{
	bool used;
	bool isRoot;
	size_t lowLink;
	StatusVertex() : used(false), isRoot(false), lowLink(0){}
};

class InfoDFSTarjan
{
public:
	std::vector<StatusVertex> statusVertices;
	size_t timer;
	std::stack<size_t> stackVertices;
	std::vector<TypeComponent> components;
   
	InfoDFSTarjan () {}
	explicit InfoDFSTarjan (size_t numVertices): statusVertices(numVertices), timer(0) {}
};

class DFSTarjan : public IncidenceCallBack
{
private:
	Graph* G;
	InfoDFSTarjan* info;
	size_t par;
	
	//updating lowLink[par]
	void updLowLink(size_t child, size_t par, std::vector<StatusVertex> &statusVertices)
   	{
      		if (par != nonParent) // if parent exists  
      		{
         		if (statusVertices[child].lowLink < statusVertices[par].lowLink)
			{
				statusVertices[par].lowLink = statusVertices[child].lowLink;
				statusVertices[par].isRoot = false;
		   	}
      		}
   	}
public:
	DFSTarjan (Graph &_G, size_t _par, InfoDFSTarjan &_info)
	{G = &_G; par = _par; info = &_info;}
	~DFSTarjan() {}
	void operator()(size_t v) override
	{
		StatusVertex& statusCurVertex = info->statusVertices[v];
		if (statusCurVertex.used)
		{
         		updLowLink(v, par, info->statusVertices);
         		return;
      		}
		statusCurVertex.lowLink = info->timer++;
		statusCurVertex.used = true;
		info->stackVertices.push(v);
		statusCurVertex.isRoot = true;

		DFSTarjan newDFS(*this);
	        newDFS.par = v;
		G->foreachIncidence(v, newDFS);

		if (statusCurVertex.isRoot)
		{
			TypeComponent component;
			while (!info->stackVertices.empty())
			{
				size_t k = info->stackVertices.top();
				info->stackVertices.pop();
				component.push_back(k);
				info->statusVertices[k].lowLink = infinity;
				if (k == v)
					break;
			}
			info->components.push_back(component);
		}

      		updLowLink(v, par, info->statusVertices);
	}
};

inline void findStroglyConnectedComponents(Graph &G, std::vector<TypeComponent> &components)
{
	InfoDFSTarjan info(G.numVertices());
	DFSTarjan dfs(G, nonParent, info);
	for (size_t v = 0; v < G.numVertices();  v++)
		if (!info.statusVertices[v].used)
			dfs(v);
	components = info.components;
}

#endif
