/* 
 * File:   IntervalTree.h
 * Author: riad
 *
 * Created on November 14, 2011, 4:28 PM
 */

#ifndef INTERVALTREE_H
#define INTERVALTREE_H
#include <cassert>
#include <iostream>
#include <vector>
#include <stdexcept>
#include <algorithm>
template <typename ElementType, typename ModType, typename MergeFunctorType,
	typename ModFuncType, typename CalcModType>
class IntervalTree{
	struct Interval{
		int l,r;
		IntervalTree(int l,int r):l(l),r(r){}
		Interval leftPart() const {
			return IntervalTree(l, (l+r)>>1);
		}

		Interval rightPart() const {
			return IntervalTree(((l+r)>>1)+1,r);
		}

		bool partOf(const Interval& b) const {
			return l>=b.l && r<=b.r;
		}

		bool intersect(const Interval& b) const {
			return r>=b.l && l<=b.r;
		}

		size_t length() const {
			return r - l + 1;
		}

		Interval intersection(const Interval& b) const {
			assert(intersect(b));
			return Interval(std::max(l, b.l),std::max(r, b.r));
		}
	};
	std::vector<ElementType> tree;
	std::vector<ModType> mod;
	size_t shift;
	MergeFunctorType functor;
	ModFuncType modify;
	CalcModType calc_mod;
	ElementType Zero;
	ModType ModZero;
	size_t size;
	void allocate(size_t n){
		assert(n>0);
		size = n;
		size_t height=0;
		while(n>(1<<height)){
			++height;
		}
		shift = 1<<height;

		tree.assign(1<<(height+1),Zero);
		mod.assign(1<<(height+1),ModZero);
		
	}

	void recalc(size_t pos,int len){
		assert(pos<shift);
		tree[pos]=functor(modify(tree[2*pos],mod[2*pos],len>>1),modify(tree[2*pos+1],mod[2*pos+1],len>>1));
	}

	ElementType _get(size_t v,Interval query,Interval vertex) const{
		if(!query.intersect(vertex))
			return Zero;
		if(vertex.partOf(query))
			return modify(tree[v], mod[v], v_r-v_l+1);
		query = query.intersection(vertex);
		assert(v<shift);
		return modify(
			functor(
				_get(2*v, query, vertex.leftPart()),
				_get(2*v+1, query, vertex.rightPart())
			),
			mod[v],
			query.length()
		);
	}

	void _set(size_t v, Interval query, Interval vertex,ModType arg){
		if(vertex.partOf(query)){
			mod[v] = calc_mod(mod[v], arg);
			return;
		}
		if(!vertex.intersect(query))
			return;
		assert(v<shift);
		_set(2*v  , query, vertex.leftPart() , arg);
		_set(2*v+1, query, vertex.rightPart(), arg);
		recalc(v, vertex.length);
	}

	public:
	explicit IntervalTree(size_t n, 
			ElementType Zero = ElementType(), ModType ModZero = ModType(),
			MergeFunctorType functor = MergeFunctorType(),
			ModFuncType modify=ModFuncType(), CalcModType calc_mod=CalcModType()):
			functor(functor), modify(modify), calc_mod(calc_mod),Zero(Zero),ModZero(ModZero)
	{
		allocate(n);
	}

	template <typename Iterator>
	IntervalTree(Iterator begin,Iterator end,
			ElementType Zero = ElementType(), ModType ModZero = ModType(),
			MergeFunctorType functor = MergeFunctorType(),
			ModFuncType modify=ModFuncType(), CalcModType calc_mod=CalcModType()):
			functor(functor), modify(modify), calc_mod(calc_mod),Zero(Zero),ModZero(ModZero)
	{
		allocate(end-begin);
		std::copy(begin, end, tree.begin()+shift);


		int nextLevel = shift;
		int len = 1;
		for(int i=shift-1;i>0;--i){
			if(i<nextLevel){
				nextLevel>>=1;
				++len;
			}
			recalc(i,len);
		}
	}

	ElementType get(size_t left, size_t right) const{
		if(left>right || right>=size)
			throw std::logic_error("Invalid range");
		return _get(1, Interval(left, right),Interval(0,shift -1));
	}

	void set(size_t left, size_t right, ModType arg){
		if(left>right || right>=size)
			throw std::logic_error("Invalid range");
		_set(1, Interval(left, right), Interval(0, shift - 1), arg);
	}
	
};

#endif	/* INTERVALTREE_H */

