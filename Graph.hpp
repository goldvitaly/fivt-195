#include <iostream>
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
};

class Graph
{
private:
	bool _IsTransposed;
	std::vector <Incident*> _Inc;
public:
	Graph (){_IsTransposed = false;}
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
		if (_IsTransposed) std::swap(VertexA, VertexB);
		_Inc[VertexA]->AddIncident(VertexB);
	}
	bool CheckIncident (size_t VertexA, size_t VertexB) const
	{
		if (_IsTransposed) std::swap(VertexA, VertexB);
		if (VertexA >= _Inc.size()) return false;
		return _Inc[VertexA]->CheckIncident(VertexB);
	}
	void Transpose ()
	{
		_IsTransposed ^= true;
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

void TopSort (const Graph &G, size_t VertexR, std::vector<size_t> &ord, bool ToClean = true)
{
	static std::vector<bool> used;
	
	if (ToClean) ord.resize(0), used.assign(G.VertexNum(), false);
	
	const Incident *Inc = G.GetIncident(VertexR);
	
	used[VertexR] = true;
	for (size_t v = Inc->Begin(); v != Inc->End(); v = Inc->Next(v))
	{
		if (!used[v]) TopSort(G, v, ord, false);
	}
	
	ord.push_back(VertexR);
}

void FindStronglyConnectedComponents (const Graph &G)
{
	//if ()
	std::vector <size_t> comp(G.IncidentNum(), 0);
	
}
