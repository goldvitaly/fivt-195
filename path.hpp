#pragma once

#include <memory>

namespace graph
{

template<typename WeightType>
class Path
{
public:
	virtual ~Path() {};
	virtual void addEdge(WeightType edge) = 0;
	virtual bool operator<(const Path<WeightType>& path) const = 0;
}; 

template<typename WeightType>
class SumPath : public Path<WeightType>
{
public:
	SumPath();
	virtual ~SumPath() {};
	virtual void addEdge(WeightType edge);
	virtual bool operator<(const Path<WeightType>& path) const;

private:
	WeightType sum;
};

template<typename WeightType>
SumPath<WeightType>::SumPath() : sum(0)
{}

template<typename WeightType>
void SumPath<WeightType>::addEdge(WeightType edge)
{
	sum += edge;
}

template<typename WeightType>
bool SumPath<WeightType>::operator<(const Path<WeightType>& path) const
{
	return sum < ((SumPath<WeightType>&)path).sum;
}

}