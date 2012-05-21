#pragma once

#include <memory>
#include <cmath>
#include <list>

namespace graph
{

template<typename WeightType>
class Path
{
public:
	virtual ~Path() {}
	virtual void addEdge(unsigned to, WeightType edge) = 0;
	virtual bool operator<(const Path<WeightType>& path) const = 0;
	virtual void setInfinity() = 0;
	virtual void setZero(unsigned start) = 0;
	virtual bool worseThan(const Path<WeightType>& path, WeightType edge) const = 0;
}; 

template<typename WeightType>
class SumPath : public Path<WeightType>
{
public:
	SumPath();
	virtual ~SumPath() {}
	virtual void addEdge(unsigned to, WeightType edge);
	virtual bool operator<(const Path<WeightType>& path) const;
	virtual void setInfinity();
	virtual void setZero(unsigned start);
	virtual bool worseThan(const Path<WeightType>& path, WeightType edge) const;
	virtual const WeightType& length() const;

protected:
	WeightType sum;
};

template<typename WeightType>
class TraceSumPath : public SumPath<WeightType>
{
public:
	virtual ~TraceSumPath() {}
	virtual void addEdge(unsigned to, WeightType edge);
	virtual void setZero(unsigned start);
	virtual const std::list<unsigned>& path() const;

private:
	std::list<unsigned> pathList;
};

template<typename WeightType, typename PathType>
class Paths
{
public:
	Paths(size_t size);
	PathType& operator[](int index);
	const PathType& at(int index) const;

private:
	std::vector<PathType> content;
};

template<typename WeightType>
SumPath<WeightType>::SumPath() : sum(0)
{}

template<typename WeightType>
void SumPath<WeightType>::addEdge(unsigned to, WeightType edge)
{
	sum += edge;
}

template<typename WeightType>
bool SumPath<WeightType>::operator<(const Path<WeightType>& path) const
{
	return sum < dynamic_cast<const SumPath<WeightType>&>(path).sum;
}

template<typename WeightType>
void SumPath<WeightType>::setInfinity()
{
	sum = HUGE_VAL;
}

template<typename WeightType>
void SumPath<WeightType>::setZero(unsigned start)
{
	sum = 0;
}

template<typename WeightType>
bool SumPath<WeightType>::worseThan(const Path<WeightType>& path, WeightType edge) const
{
	return sum > dynamic_cast<const SumPath<WeightType>&>(path).sum + edge;
}

template<typename WeightType>
const WeightType& SumPath<WeightType>::length() const
{
	return sum;
}

template<typename WeightType>
void TraceSumPath<WeightType>::addEdge(unsigned to, WeightType edge)
{
	SumPath<WeightType>::addEdge(to, edge);
	pathList.push_back(to);
}

template<typename WeightType>
void TraceSumPath<WeightType>::setZero(unsigned start)
{
	SumPath<WeightType>::setZero(start);
	pathList.clear();
	pathList.push_back(start);
}

template<typename WeightType>
const std::list<unsigned>& TraceSumPath<WeightType>::path() const
{
	return pathList;
}

template<typename WeightType, typename PathType>
Paths<WeightType, PathType>::Paths(size_t size)
{
	for(size_t p = 0; p < size; ++p)
		content.push_back(PathType());
}

template<typename WeightType, typename PathType>
PathType& Paths<WeightType, PathType>::operator[](int index)
{
	return content[index];
}

template<typename WeightType, typename PathType>
const PathType& Paths<WeightType, PathType>::at(int index) const
{
	return content[index];
}

}//namespace graph