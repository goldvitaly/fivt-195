#include <iostream>
#include <vector>
#include <cassert>

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
	Interval(const int l, const int r) : l(l), r(r)
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
        TreeElement(const ValueType &value): val(value), isMod(false), active(false) {}
    };

    std::vector<TreeElement> tree; // индексация с 1
	ModFunc modFunc;
	Merge merge;
	CompMod compMod;
    size_t lastLevel;
    size_t kolElem;
    const static size_t root = 1;

    Interval fullInterval() const
    {
        return Interval(1, lastLevel - 1);
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
        assert(tree[leftChild(pos)].active);
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
        tree[pos].value = modFunc(tree[pos].value, change, our.length());
    }

    void siftDown(const size_t pos)
    {
        assert(pos < lastLevel);
        tree[pos].isMod = false;
        updMod(leftChild(pos), tree[pos].mod);
        updMod(rightChild(pos), tree[pos].mod);
    }

//значение в самой вершине считается при апдэйте
//инвариант - отрезки перед каждым вызовом обязательно пересекаются
    ValueType query(const size_t pos, const Interval &our, const Interval &req)
    {
        assert(pos < tree.size());
        assert(req.intersect(our));
        if (req.inside(our))
        {
            return tree[pos].val;
        }
        assert(pos < lastLevel);
        siftDown(pos);
        ValueType resLeft, resRight;
        if ((our.leftpart()).intersect(req))
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
            assert((our.rightpart()).intersect(req));
            return query(rightChild(pos), our.rightPart(), req);
        }
    }

//инвариант - отрезки перед каждым вызовом обязательно пересекаются
    void updRange(const size_t pos, const Interval &our, const Interval &req, const ModType &change)
    {
        assert(pos < tree.size());
        if (req.inside(our))
        {
            updMod(pos);
            return;
        }
        assert(pos < lastLevel);
        siftDown(pos);
        if ((our.leftpart()).intersect(req))
        {
            updRange(leftChild(pos), our.leftPart(), req, change);
            if ((our.rightPart()).intersect(req))
            {
                updRange(rightChild(pos), our.rightPart(), req, change);
            }
        }
        else
        {
            assert((our.rightpart()).intersect(req));
            updRange(rightChild(pos), our.rightPart(), req);
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
            recalc(tree[i]);
            tree[i](tree[i].val);
        }
    };
public:
//конструкторы ещё надо доработать
    explicit IntervalTree (const ValueType &elem, Merge merge = Merge(),
    ModFunc modFunc = ModFunc(), CompMod compMod = CompMod()):
    lastLevel(1), merge(merge), modFunc(modFunc), compMod(compMod)
    {
        tree.resize(2);
        tree[1] = TreeElelm(elem);
    }

    template <typename Iterator>
    IntervalTree (Iterator begin, Iterator end, Merge merge = Merge(),
    ModFunc modFunc = ModFunc(), CompMod compMod = CompMod() ):
    lastLevel(1), merge(merge), modFunc(modFunc), compMod(compMod)
    {
        int sz = std::distance(begin, end);
        lastLevel = minpower2(sz);
        tree.resize(2 * lastLevel);
        std::copy(begin, end, tree.begin() + lastLevel);
        buildTree();
    }

    size_t size() const
    {
        return kolElem;
    }

    ValueType get(const int num)
    {
        return get(num, num);
    }

    ValueType get(const int left, const int right)
    {
        assert(left >= 0);
        assert(right < kolElem);
        return query(root, fullInterval(), Interval(left, right));
    }


    void update(const int left, const int right, const ModType &newMod)
    {
        assert(left >= 0);
        assert(right < kolElem);
        updRange(root, fullInterval(), Interval(left, right), newMod);
    }

    void update(const int num, const ModType &newMod)
    {
        update(num, num, newMod);
    }
    void add(const ValueType &newElem)
    {
        if (kolElem == lastLevel - 1)
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
        update(kolElem + lastLevel, newElem);
        kolElem++;
    }
};

int main()
{
    std::cout << "Hello world!";
    return 0;
}
