#ifndef GRAPHALGO_H_INCLUDED
#define GRAPHALGO_H_INCLUDED

#include <vector>
#include <stack>
#include <limits>
#include "Graph.h"

typedef std::vector<size_t> TypeComponent;

struct StatusVertice
{
    bool used;
    bool isRoot;
    size_t lowLink;
    StatusVertice() : used(false), isRoot(false), lowLink(0) {}
};

class InfoDFSTarjan
{
public:
    std::vector<StatusVertice> statusVertices;
    size_t timer;
    std::stack<size_t> stackVertices;
    std::vector<TypeComponent> components;

    InfoDFSTarjan (size_t numVertices): statusVertices(numVertices) {}
};

class DFSTarjan : public IncidenceCallBack
{
private:
    Graph* G;
    InfoDFSTarjan* info;
    size_t par;

    void updLowLink(size_t v, size_t par, std::vector<StatusVertice> &statusVertices)
    {
        if (par < G->numVertices()) // it should be in DFS for parent after recursive from its child v
        {
            if (statusVertices[v].lowLink < statusVertices[par].lowLink)
            {
                statusVertices[par].lowLink = statusVertices[v].lowLink;
                statusVertices[par].isRoot = false;
            }
        }
    }
public:
    DFSTarjan (Graph &_G, size_t _par, InfoDFSTarjan &_info)
    {
        G = &_G;
        par = _par;
        info = &_info;
    }
    ~DFSTarjan() {}
    void operator()(size_t v) override
    {
        if (info->statusVertices[v].used)
        {
            updLowLink(v, par, info->statusVertices);
            return;
        }
        info->statusVertices[v].lowLink = info->timer++;
        info->statusVertices[v].used = true;
        info->stackVertices.push(v);
        info->statusVertices[v].isRoot = true;
        DFSTarjan newDFS(*this);
        newDFS.par = v;
        G->foreachIncidence(v, newDFS);

        if (info->statusVertices[v].isRoot)
        {
            TypeComponent component;
            while (!info->stackVertices.empty())
            {
                size_t k = info->stackVertices.top();
                info->stackVertices.pop();
                component.push_back(k);
                info->statusVertices[k].lowLink = std::numeric_limits<size_t>::max();
                if (k == v)
                    break;
            }
            info->components.push_back(component);
        }

        updLowLink(v, par, info->statusVertices);
    }
};


#endif
