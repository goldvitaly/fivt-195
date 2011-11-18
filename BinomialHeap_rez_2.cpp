#include <string>
#include <vector>
#include <iostream>
#include <fstream>

using namespace std;

template<class T>
class BinTree{
private:
    T key_;
    vector<BinTree<T>*> children;
public:
    BinTree(T key){
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
        if(tree1->min() > tree2->min())   // выбирает что из деревьев корень
        {
            swap(tree1, tree2);
        }
        tree1->children.push_back(tree2);
        return tree1;
    }
    int num_children()const{return children.size();}
    int size(){return num_children();}
    vector<BinTree<T>*>& get_children() {return children;} //возврат списка детей
    void children_NULL()
    {
        children.clear();
    }
};

template<class T>
class BinHeap{
private:
    vector<BinTree<T>*> link;   // хранит деревья
    int min_id()
    {
        int id = 0;
        bool mark = true;
        for(size_t it = 0; it < link.size(); it++)
        {
            if((link[it] != NULL) && (mark || link[it]->min() < link[id]->min()))
            {
                id = it;
                mark = false;
            }
        }
        if(mark)
        {
            cout << "Empty heap" <<endl;
            exit(1);
        }
        return id;
    }
public:
    BinHeap(){link.resize(0);};
    BinHeap(T key){
        link.push_back(new BinTree<T>(key));
    }
    BinHeap(BinTree<T>* tree){
        link.resize(tree->num_children());
        link.push_back(tree);
    }
    ~BinHeap(){
        for(size_t i = 0; i < link.size(); i++)
        {
            delete link[i];
        }
    }
    T min()               //возвращает минимальное значение
    {
        return link[min_id()]->min();
    }
    BinHeap<T>* pop()          // возвращает кучу без минимального значения
    {
        int id = min_id();
        BinTree<T>* ans_tree = link[id];
        vector<BinTree<T>*> children_temp = ans_tree->get_children();
        ans_tree -> children_NULL(); //подготовка к  удалению мин вершины
        delete ans_tree;
        link[id] = NULL;
        BinHeap<T>* rez = new BinHeap<T>;
        for(size_t i = 0; i < children_temp.size(); i++) // слияние с детьми удаляемой вершины
        {
            BinHeap<T>* temp = new BinHeap<T>(children_temp[i]);
            rez = merge(rez, new BinHeap<T>(children_temp[i]));
        }
        rez = merge(rez, this); //слияние с тек кучей без элемента
        return rez;
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
            if(i < heap1->link.size() && heap1->link[i] != NULL)  //выбор ненулевых деревьев
                trees.push_back(heap1->link[i]);
            if(i < heap2->link.size() && heap2->link[i] != NULL)
                trees.push_back(heap2->link[i]);
            if(flag != NULL)
                trees.push_back(flag);
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
        int now_size = ans->link.size();
        while(now_size > 0 && ans->link[now_size-1] == NULL)
        {
            now_size--;
        }
        ans->link.resize(now_size);
        return ans;
    }
    BinHeap<T>* push(T key)     //возв кучу с доб ключом
    {
        BinHeap<T>* first = new BinHeap<T>(key);
        BinHeap<T>* ans = merge(this, first);
        return ans;
    }
    bool empty(){return(link.size() == 0);}         //O(1)
    int size()                                      // O(link)
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


int main()
{
    return 0;
}
