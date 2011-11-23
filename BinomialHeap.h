#ifndef BINOMIALHEAP_H_INCLUDED
#define BINOMIALHEAP_H_INCLUDED

#include <vector>

using namespace std;

template<class T>
class BinTree{
private:
    T key_;
    vector<BinTree<T>*> children;
public:
    explicit BinTree(T key){
        children.resize(0);
        key_ = key;
    }
    ~BinTree(){
        for(size_t i = 0; i < children.size(); i++)
        {
            delete children[i];
        }
    }
    T min() const{return key_;}     //минимум в поддереве - корень
    static BinTree<T>* merge(BinTree<T>* tree1, BinTree<T>* tree2){
        assert(tree1->num_children() == tree2->num_children());
        if(tree1->min() > tree2->min())   // выбирает что из деревьев корень
        {
            swap(tree1, tree2);
        }
        tree1->children.push_back(tree2);
        return tree1;
    }
    int num_children()const{return children.size();}
    int size() const {return num_children();}
    vector<BinTree<T>*>& get_children() {return children;} //возврат списка детей
    void clear_children() {children.clear(); }
};

template<class T>
class BinHeap{
private:
    vector<BinTree<T>*> link;   // хранит деревья
    int min_id() const
    {
        int id = 0;
        bool found = false;
        for(size_t it = 0; it < link.size(); it++)
        {
            if((link[it] != NULL) && ( !found || link[it]->min() < link[id]->min()))
            {
                id = it;
                found = true;
            }
        }
        if(!found)
        {
            cout << "Empty heap" <<endl;
            exit(1);
        }
        return id;
    }
    void del_last_zero()
    {
        int now_size = link.size();
        while(now_size > 0 && link[now_size-1] == NULL)
        {
            now_size--;
        }
        link.resize(now_size);
    }
    bool exist_tree(const int& i) const
    {
        if(i < link.size() && link[i] != NULL)
            return true;
        else
            return false;
    }
public:
    BinHeap(){};
    explicit BinHeap(const T& key){
        link.push_back(new BinTree<T>(key));
    }
    explicit BinHeap(BinTree<T>* tree){
        link.resize(tree->num_children());
        link.push_back(tree);
    }
    ~BinHeap(){
        for(size_t i = 0; i < link.size(); i++)
        {
            delete link[i];
        }
    }
    T min() const               //возвращает минимальное значение
    {
        return link[min_id()]->min();
    }
    BinHeap<T>* pop()          // возвращает кучу без минимального значения
    {
        int id = min_id();
        BinTree<T>* ans_tree = link[id];
        vector<BinTree<T>*> children_remove_link = ans_tree->get_children();
        ans_tree->clear_children(); //подготовка к  удалению мин вершины
        delete ans_tree;
        link[id] = NULL;
        BinHeap<T>* res = new BinHeap<T>;
        for(size_t i = 0; i < children_remove_link.size(); i++) // слияние с детьми удаляемой вершины
        {
            BinHeap<T>* temp = new BinHeap<T>(children_remove_link[i]);
            res = merge(res, new BinHeap<T>(children_remove_link[i]));
        }
        res = merge(res, this); //слияние с тек кучей без элемента
        return res;
    }
    static BinHeap<T>* merge(BinHeap<T>* heap1, BinHeap<T>* heap2){

        BinHeap<T>* ans = new BinHeap<T>;
        BinTree<T>* flag = NULL;
        for(size_t i = 0; i < heap1->link.size() || i < heap2->link.size() || flag != NULL; i++)
        {
            if(ans->link.size() <= i)
            {
                ans->link.resize(i + 1);
            }
            vector<BinTree<T>*> trees;
            trees.reserve(3);
            if(heap1->exist_tree(i))
                trees.push_back(heap1->link[i]);
            if(heap2->exist_tree(i))
                trees.push_back(heap2->link[i]);
            if(flag != NULL)
                trees.push_back(flag);

            for(int j = 0; j < trees.size(); j++)
                assert(trees[j]->size() == i);

            if(trees.size() == 1)                 //разбор случаев кол-ва деревьев для суммы
            {
                ans->link[i] = trees[0];
                flag = NULL;
            }
            else if(trees.size() >= 2)
            {
                flag = BinTree<T> :: merge(trees[0], trees[1]);
                if(trees.size() == 3)
                {
                    ans->link[i] = trees[2];
                }
            }
        }
        ans->del_last_zero();
        assert(ans->size() == heap1->size() + heap2->size());
        return ans;
    }
    BinHeap<T>* push(T key)     //возв кучу с доб ключом
    {
        BinHeap<T>* first = new BinHeap<T>(key);
        BinHeap<T>* ans = merge(this, first);
        return ans;
    }
    bool empty() const{return(link.size() == 0);}         //O(1)
    int size() const                                      // O(link)
    {
        int ans = 0;
        for(size_t i = 0; i < link.size(); i++)
        {
            if(link[i] != NULL)
                ans += 1 << i;
        }
        return ans;
    }
};

#endif // BINOMIALHEAP_H_INCLUDED
