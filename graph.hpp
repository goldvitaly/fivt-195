#include <vector>

class Incident
{
public:
	virtual void   AddIncident    (size_t Vertex)       = 0;
	virtual void   RemoveIncident (size_t Vertex)       = 0;
	virtual bool   CheckIncident  (size_t Vertex) const = 0;
	virtual size_t IncidentNum    ()              const = 0;
	virtual size_t Begin          ()              const = 0;
	virtual size_t Next           (size_t Vertex) const = 0;
	virtual size_t End            ()              const = 0;

	virtual ~Incident (){};
};

class MyIncident : public Incident
{
private:
	size_t _IncidentNum;
	std::vector <bool> _a;
public:
	MyIncident ()
	{
		_IncidentNum = 0;
	}
	void AddIncident (size_t Vertex)
	{
		if (Vertex >= _a.size())
			_a.resize(Vertex + 1, false);
		if (!_a[Vertex]) _IncidentNum++;
		_a[Vertex] = true;
	}
	void RemoveIncident (size_t Vertex)
	{
		if (Vertex < _a.size())
		{
			if (_a[Vertex]) _IncidentNum--;
			_a[Vertex] = false;
		}
	}
	bool CheckIncident (size_t Vertex) const
	{
		if (Vertex >= _a.size())
			return false;
		return _a[Vertex];
	}
	size_t IncidentNum () const
	{
		return _IncidentNum;
	}
	size_t Begin () const
	{
		for (size_t i = 0; i < _a.size(); ++i)
			if (_a[i])
				return i;
		return End();
	}
	size_t Next (size_t Vertex) const
	{
		for (size_t i = Vertex + 1; i < _a.size(); ++i)
			if (_a[i])
				return i;
		return End();
	}
	size_t End () const
	{
		return _a.size();
	}
	~MyIncident (){}
};

class Graph
{
private:
	std::vector <Incident*> _Inc;
public:
	Graph (){}
	template <typename IncidentClass> void AddVertex (size_t Vertex)
	{
		while (Vertex >= _Inc.size()) _Inc.push_back(new IncidentClass());
	}
	void RemoveVertex (size_t Vertex)
	{
		for (size_t i = 0; i < Vertex; ++i)
			for (size_t j = Vertex; j < _Inc.size(); ++j)
				_Inc[i]->RemoveIncident(j);

		while (Vertex < _Inc.size()) _Inc.pop_back();
	}
	void AddIncident (size_t VertexA, size_t VertexB)
	{
		_Inc[VertexA]->AddIncident(VertexB);
	}
	bool CheckIncident (size_t VertexA, size_t VertexB) const
	{
		if (VertexA >= _Inc.size()) return false;
		return _Inc[VertexA]->CheckIncident(VertexB);
	}
	size_t VertexNum () const
	{
		return _Inc.size();
	}
	size_t IncidentNum () const
	{
		size_t num = 0;
		for (size_t i = 0; i < _Inc.size(); ++i)
			num += _Inc[i]->IncidentNum();
		return num;
	}
	const Incident* GetIncident (size_t Vertex) const
	{
		return _Inc[Vertex];
	}
};

class GraphAlgorithm
{
private:
    size_t _Timer, _Color;
    std::vector<size_t> _Comp, _Tin;
    std::vector<bool> _Used;
    std::vector<size_t> _Stack;
    
    void FSCC_Dfs (const Graph& G, size_t v)
    {
        _Used[v] = true;
        _Stack.push_back(v);
        _Tin[v] = _Timer++;
        
        bool IsRoot = true;
        const Incident* inc = G.GetIncident(v);
        for (size_t u = inc->Begin(); u != inc->End(); u = inc->Next(u))
        {
            if (!_Used[u])
                FSCC_Dfs(G, u);
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
public:
    std::vector<size_t> FindStronglyConnectedComponents (const Graph& G)
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
};