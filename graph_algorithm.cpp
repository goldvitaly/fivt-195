std::vector<size_t> GraphAlgorithm::FindStronglyConnectedComponents (const Graph& G)
{
    _Used.assign(G.VertexNum(), false);
    _Comp.assign(G.VertexNum(), 0);
    _Tin.assign(G.VertexNum(), 0);
    _Stack.clear();
    _Timer = 0;
    _Color = 0;
    
    for (size_t i = 0; i < G.VertexNum(); ++i)
        if (!_Used[i])
            FSCC_Dfs(G, i);
    
    return _Comp;
}

void GraphAlgorithm::StronglyConnectedComponentsDFS (const Graph& G, size_t v)
{
    _Used[v] = true;
    _Stack.push_back(v);
    _Tin[v] = _Timer++;
    
    bool IsRoot = true;
    const Incident* inc = G.GetIncident(v);
    for (size_t u = inc->Begin(); u != inc->End(); u = inc->Next(u))
    {
        if (!_Used[u])
            StronglyConnectedComponentsDFS(G, u);
        if (_Tin[v] > _Tin[u])
        {
            _Tin[v] = _Tin[u];
            IsRoot = false;
        }
    }
    
    if (IsRoot)
    {
        size_t u;
        do
        {
            u = _Stack.back();
            _Stack.pop_back();
            
            _Comp[u] = _Color;                
            
        } while (u != v);
        _Color++;
    }
}
};