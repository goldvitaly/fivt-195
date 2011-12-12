/* 
 * File:   IntervalTree.h
 * Author: riad
 *
 * Created on November 14, 2011, 4:28 PM
 */

#ifndef INTERVALTREE_H
#define INTERVALTREE_H
#include <cassert>
#include <vector>
#include <stdexcept>
#include <algorithm>
#include <boost/optional.hpp>

/*
 * Element - тип элемена
 * Modificarion - тип модификации
 * Merge - функтор (Element, Element) - объединение двух отрезков
 * ModFunc - функтор (Element, Modification, size_t l) - применить модификацию к отрезку длины l
 * CalcMod - функтор (Modification old, Modification new) - объединить две модификации.
 */
template <typename Element, typename Modification, typename Merge,
typename ModFunc, typename CalcMod>
class IntervalTree {

	struct Interval {
		int l, r;

		Interval(int l, int r) : l(l), r(r) {
			assert(l <= r);
		}

		Interval leftPart() const {
			return Interval(l, (l + r) >> 1);
		}

		Interval rightPart() const {
			return Interval(((l + r) >> 1) + 1, r);
		}

		bool partOf(const Interval & b) const {
			return l >= b.l && r <= b.r;
		}

		bool intersects(const Interval & b) const {
			return r >= b.l && l <= b.r;
		}

		size_t length() const {
			return r - l + 1;
		}
	};
	std::vector<Element> tree;
	std::vector<boost::optional<Modification >> mod;
	size_t shift;
	Merge merge;
	ModFunc modify;
	CalcMod calcMod;
	size_t size;

	void allocate(size_t n, const Element& defaultElement) {
		assert(n > 0);
		size = n;
		size_t height = 0;
		while (n > (1 << height)) {
			++height;
		}
		shift = 1 << height;

		tree.assign(1 << (height + 1), defaultElement);
		mod.assign(1 << (height + 1), boost::optional<Modification > ());
	}

	Element apply_modification(const Element& element, const boost::optional<Modification>& modification, size_t len) const {
		if (modification) {
			return modify(element, *modification, len);
		} else
			return element;
	}

	boost::optional<Element> optional_merge(const boost::optional<Element>& x, const boost::optional<Element>& y) const {
		if (!x)
			return y;
		if (!y)
			return x;
		return merge(*x, *y);
	}

	void add_modification(size_t v, const Modification& modification) {
		if (mod[v])
			mod[v] = calcMod(*mod[v], modification);
		else
			mod[v] = modification;
	}

	void recalc(size_t pos, size_t len) {
		assert(pos < shift);
		tree[pos] = merge(apply_modification(tree[2 * pos], mod[2 * pos], len >> 1),
				  apply_modification(tree[2 * pos + 1], mod[2 * pos + 1], len >> 1));
	}

	void mod_down(size_t v, size_t len) {
		if (!mod[v])
			return;
		if (v >= shift) {
			tree[v] = apply_modification(tree[v], mod[v], len);
		} else {
			add_modification(2 * v, *mod[v]);
			add_modification(2 * v + 1, *mod[v]);
			recalc(v, len);
		}
		mod[v].reset();
	}

	boost::optional<Element> _get(size_t v, const Interval& query, const Interval& vertex) {
		if (!query.intersects(vertex))
			return boost::optional<Element > ();
		mod_down(v, vertex.length());
		if (vertex.partOf(query))
			return tree[v];
		assert(v < shift);
		return optional_merge(
				  _get(2 * v, query, vertex.leftPart()),
				  _get(2 * v + 1, query, vertex.rightPart())
				  );
	}

	void _set(size_t v, const Interval& query, const Interval& vertex, const Modification& arg) {
		if (!vertex.intersects(query))
			return;
		if (vertex.partOf(query)) {
			add_modification(v, arg);
			return;
		}
		mod_down(v, vertex.length());
		assert(v < shift);
		_set(2 * v, query, vertex.leftPart(), arg);
		_set(2 * v + 1, query, vertex.rightPart(), arg);
		recalc(v, vertex.length());
	}

	void recalc_all() {
		int nextLevel = shift;
		int len = 1;
		for (int i = shift - 1; i > 0; --i) {
			if (i < nextLevel) {
				nextLevel >>= 1;
				++len;
			}
			recalc(i, len);
		}
	}

public:

	explicit IntervalTree(size_t n, Element zero = Element(),
			  Merge merge = Merge(),
			  ModFunc modify = ModFunc(), CalcMod calcMod = CalcMod()) :
	merge(merge), modify(modify), calcMod(calcMod) {
		allocate(n, zero);
		recalc_all();
	}

	template <typename Iterator>
	IntervalTree(Iterator begin, Iterator end,
			  Merge merge = Merge(),
			  ModFunc modify = ModFunc(), CalcMod calcMod = CalcMod()) :
	merge(merge), modify(modify), calcMod(calcMod) {
		allocate(end - begin, Element());
		std::copy(begin, end, tree.begin() + shift);
		recalc_all();
	}

	Element get(size_t left, size_t right) {
		if (left > right || right >= size)
			throw std::logic_error("Invalid range");
		return *_get(1, Interval(left, right), Interval(0, shift - 1));
	}

	void set(size_t left, size_t right, const Modification& arg) {
		if (left > right || right >= size)
			throw std::logic_error("Invalid range");
		_set(1, Interval(left, right), Interval(0, shift - 1), arg);
	}

};

#endif /* INTERVALTREE_H */

