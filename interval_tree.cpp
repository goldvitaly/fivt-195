#include <iostream>
#include <vector>
#include <cassert>
#include <cstdlib>

// тип, тип модификации, обновление значения от значений в детях, апдэйт одной вершины, композиция
template <typename ValueType, typename ModType, typename Merge,
typename ModFunc, typename CompMod >
class IntervalTree
{
private:
    struct Interval
    {
        size_t l, r;
        Interval leftPart() const
        {
			return Interval(l, (l + r) >> 1);
		}
		Interval rightPart() const
		{
			return Interval(((l + r) >> 1) + 1, r);
		}
		bool inside(const Interval & b) const
		{
			return l >= b.l && r <= b.r;
		}
		bool intersect(const Interval & b) const
		{
			return r >= b.l && l <= b.r;
		}
		size_t length() const
		{
			return r - l + 1;
		}
		Interval(const size_t l, const size_t r) : l(l), r(r)
		{
			assert(l <= r);
			assert(l >= 0);
		}
    };

    struct TreeElement
    {
        ValueType val;
        ModType mod;
        bool isMod;
        bool active;
        TreeElement(const ValueType &value): val(value), isMod(false), active(true) {}
		TreeElement(): isMod(false), active(false) {}
    };

    std::vector<TreeElement> tree; // индексация с 1
	ModFunc modFunc;
	Merge merge;
	CompMod compMod;
    size_t lastLevel;
    size_t qElem;
    const static size_t root = 1;

    Interval fullInterval() const
    {
        return Interval(0, lastLevel - 1);
    }
    inline size_t leftChild(const size_t pos) const
    {
        return pos << 1;
    }
    inline size_t rightChild(const size_t pos) const
    {
        return (pos << 1) + 1;
    }
    inline size_t parent(const size_t pos) const
    {
        return pos >> 1;
    }
    size_t minpower2(const size_t x) const
    {
        size_t p = 1;
        while (p < x)
            p *= 2;
        return p;
    }

    void off(const size_t pos)
    {
        assert(pos < tree.size());
        tree[pos].active = tree[pos].isMod = false;
    }
    void recalc(const size_t pos)
    {
        assert(pos < tree.size());
        if (pos >= lastLevel)
            return;
        assert(tree[leftChild(pos)].active || !tree[rightChild(pos)].active);
        if (!tree[leftChild(pos)].active)
		{
			tree[pos].active = false;
			return;
		}
		tree[pos].active = true;
		if (!tree[rightChild(pos)].active)
            tree[pos].val = tree[leftChild(pos)].val;
        else
            tree[pos].val = merge(tree[leftChild(pos)].val,
                                  tree[rightChild(pos)].val);
    }

//может быть ещё надо передавать интервал, иначе сумма не будет работать
    void updMod (const size_t pos, const ModType &change, const Interval &our)
    {
        if (tree[pos].isMod)
        {
            tree[pos].mod = compMod(tree[pos].mod, change);
        }
        else
        {
            tree[pos].mod = change;
            tree[pos].isMod = true;
        }
        tree[pos].val = modFunc(tree[pos].val, change, our.length());
    }

    void siftDown(const size_t pos, const Interval &our)
    {
        assert(pos < lastLevel);
		if (tree[pos].isMod == false)
			return;
		tree[pos].isMod = false;
        updMod(leftChild(pos), tree[pos].mod, our.leftPart());
        updMod(rightChild(pos), tree[pos].mod, our.rightPart());
    }

//значение в самой вершине считается при апдэйте
//инвариант - отрезки перед каждым вызовом обязательно пересекаются
    ValueType query(const size_t pos, const Interval &our, const Interval &req)
    {
        assert(pos < tree.size());
        assert(req.intersect(our));
        if (our.inside(req))
        {
            return tree[pos].val;
        }
        assert(pos < lastLevel);
        siftDown(pos, our);
        ValueType resLeft, resRight;
        if ((our.leftPart()).intersect(req))
        {
            resLeft = query(leftChild(pos), our.leftPart(), req);
            if ((our.rightPart()).intersect(req))
            {
                resRight = query(rightChild(pos), our.rightPart(), req);
                return merge(resLeft, resRight);
            }
            else
                return resLeft;
        }
        else
        {
            assert((our.rightPart()).intersect(req));
            return query(rightChild(pos), our.rightPart(), req);
        }
    }

//инвариант - отрезки перед каждым вызовом обязательно пересекаются
    void updRange(const size_t pos, const Interval &our, const Interval &req, const ModType &change)
    {
        assert(pos < tree.size());
        if (our.inside(req))
        {
            updMod(pos, change, our);
            return;
        }
        assert(pos < lastLevel);
        siftDown(pos, our);
        if ((our.leftPart()).intersect(req))
        {
            updRange(leftChild(pos), our.leftPart(), req, change);
            if ((our.rightPart()).intersect(req))
            {
                updRange(rightChild(pos), our.rightPart(), req, change);
            }
        }
        else
        {
            assert((our.rightPart()).intersect(req));
            updRange(rightChild(pos), our.rightPart(), req, change);
        }
        recalc(pos);
    }

    void updsingle(const size_t pos)
    {
        assert(pos < tree.size());
        pos = parent(pos);
        while (pos > 0)
        {
            recalc(pos);
            pos = parent(pos);
        }
    }
//надо проверить
    void buildTree()
    {
        for (int i = lastLevel - 1; i >= 1; i--)
        {
            recalc(i);
        }
    };
public:
//конструкторы ещё надо доработать
    explicit IntervalTree (const ValueType &elem, Merge merge_ = Merge(),
    ModFunc modFunc_ = ModFunc(), CompMod compMod_ = CompMod()):
    lastLevel(1), qElem(1), merge(merge_), modFunc(modFunc_), compMod(compMod_)
    {
        tree.resize(2);
        tree[1](elem);
    }

    template <typename Iterator>
    IntervalTree (Iterator begin, Iterator end, Merge merge_ = Merge(),
    ModFunc modFunc_ = ModFunc(), CompMod compMod_ = CompMod() ):
    lastLevel(1), merge(merge_), modFunc(modFunc_), compMod(compMod_)
    {
        qElem = std::distance(begin, end);
        lastLevel = minpower2(qElem);
        tree.resize(2 * lastLevel);
        std::copy(begin, end, tree.begin() + lastLevel);
        buildTree();
    }

    size_t size() const
    {
        return qElem;
    }

    ValueType get(const size_t numELem)
    {
        return get(numELem, numELem);
    }

    ValueType get(const size_t left, const size_t right)
    {
        assert(left >= 0);
        assert(right < qElem);
        return query(root, fullInterval(), Interval(left, right));
    }


    void update(const size_t left, const size_t right, const ModType &newMod)
    {
        assert(left >= 0);
        assert(right < qElem);
        updRange(root, fullInterval(), Interval(left, right), newMod);
    }

    void update(const int numElem, const ModType &newMod)
    {
        update(numElem, numElem, newMod);
    }
    void add(const ValueType &newElem)
    {
        if (qElem == lastLevel - 1)
        {
            int x = lastLevel;
            lastLevel <<= 1;
            tree.resize(lastLevel << 1);
            for (int i = lastLevel; i < lastLevel << 1; i++)
                off(i);
            for (int i = lastLevel - 1; i >= 1; i--)
            {
                tree[x + i] = tree[i];
                if (i == x)
                    x >>= 1;
                off(i);
            }
            // в 1 вершине может быть пусто
        }
		qElem++;
		tree[qElem - 1].active = true;
		update(qElem - 1, newElem);
	}
};

class fmerge {
  public:
  int operator()(int A, int B) const
  {
    return std::min(A, B);
  }
};
class fmod1 {
  public:
  int operator()(int A, int B, size_t C) const
  {
    return B;
  }
};
class fcomp {
  public:
  int operator()(int A, int B) const 
  {
    return B;
  }
};


void testeasy()
{
	int n = 3000, k = 3000;
    srand(n);
    std::vector<int> a(n);
    for (int i = 0; i < n; i++)
        a[i] = rand() % n;
    IntervalTree<int, int, fmerge, fmod1, fcomp> t(a.begin(), a.end());
    for (int i = 0; i < k; i++)
	{
		int l = rand() % n, r = rand() % n;
		if (l > r)
			std::swap(l, r);
		int ans = a[l];
		for (int j = l + 1; j <= r; j++)
			ans = std::min(ans, a[j]);
		int x = t.get(l, r);
		if (x != ans)
			std::cout << l << " " << r << std::endl;
	}
}


void z(const std::vector<int> &a, IntervalTree<int, int, fmerge, fmod1, fcomp> &t, int l, int r)
{
	int ans = a[l];
	for (int j = l + 1; j <= r; j++)
		ans = std::min(ans, a[j]);
	int x = t.get(l, r);
	if (x != ans)
		std::cout << l << " " << r << std::endl;
}
void testmedium()
{
	int n = 2100, k = 4000;
	srand(n);
    std::vector<int> a(n);
	for (int i = 0; i < n; i++)
        a[i] = i;
    IntervalTree<int, int, fmerge, fmod1, fcomp> t(a.begin(), a.end());
	for (int i = 0; i < k; i++)
	{
		int y = rand() % 2;
		if (y == 0)
		{
			int l = rand() % n;
			int r = rand() % n;
			if (l > r)
				std::swap(l, r);
			z(a, t, l, r);
		}
		if (y == 1)
		{
			int l = rand() % n;
			int r = rand() % n;
			if (l > r)
				std::swap(l, r);
			int x = rand() % n;
			t.update(l, r, x);
			for (int j = l; j <= r; j++)
				a[j] = x;
		}
	}
}
void testhard()
{
	int n = 2500, k = 4000;
	srand(n);
    std::vector<int> a(n);
	for (int i = 0; i < n; i++)
        a[i] = i;
    IntervalTree<int, int, fmerge, fmod1, fcomp> t(a.begin(), a.end());
	for (int i = 0; i < k; i++)
	{
		int y = rand() % 3;
		if (y == 0)
		{
			int l = rand() % n;
			int r = rand() % n;
			if (l > r)
				std::swap(l, r);
			z(a, t, l, r);
		}
		if (y == 1)
		{
			int l = rand() % n;
			int r = rand() % n;
			if (l > r)
				std::swap(l, r);
			int x = rand() % n;
			t.update(l, r, x);
			for (int j = l; j <= r; j++)
				a[j] = x;
		}
		if (y == 2)
		{
			std::cout << "-------------------" << std::endl;
			int x = rand() % n;
			n++;
			a.push_back(x);
			t.add(x);
		}
	}
}
int main()
{
	//testeasy();
	//testmedium();
	testhard();
	return 0;
}
