#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <queue>
#include <ctime>


template <class T, class cmp>
class cmp_for_queue
{
public:
    bool operator()(const T &x, const T &y) const
    {
        return cmp()(y.first, x.first);
    }
};



template<class T, class cmp>
void out_sort(std::ifstream &input, std::ofstream &output)
{
    cmp comparator;
    std::vector <T> storage;
    std::priority_queue<std::pair<T, std::ifstream*>, std::vector<std::pair<T, std::ifstream*> >, cmp_for_queue<std::pair<T, std::ifstream*>, cmp> > files_heap;
    T for_input;
    int number_of_files = 0;
    char temporary_file_name[10];
    while(input >> for_input)
    {
        storage.push_back(for_input);
        if(storage.size() == 100000)
        {
            add_file:
            sprintf(temporary_file_name, "%d.txt", number_of_files);
            std::ofstream writer(temporary_file_name);
            std::sort(storage.begin(), storage.end(), comparator);
            for(size_t i = 0; i < storage.size(); i++)    writer << storage[i] << ' ';
            writer.close();
            storage.clear();
            std::pair<T, std::ifstream*> add_to_heap;
            add_to_heap.second = new std::ifstream(temporary_file_name);
            *add_to_heap.second >> add_to_heap.first;
            files_heap.push(add_to_heap);
            number_of_files++;
        }
    }
    if(!storage.empty())
        goto add_file;
    std::pair<T, std::ifstream*> pop_from_heap;
    while(!files_heap.empty())
    {
        pop_from_heap = files_heap.top();
        files_heap.pop();
        output << pop_from_heap.first << ' ';
        if(!pop_from_heap.second -> eof())
        {
            *pop_from_heap.second >> pop_from_heap.first;
            files_heap.push(pop_from_heap);
        }
    }
}



int main()
{
    srand(time(NULL));
    std::ofstream test_writer("input.txt");
    std::vector <int> ans;
    for(int i = 0; i < 1000000; i++)
    {
        int z = rand();
        ans.push_back(z);
        test_writer << z << ' ';
    }
    sort(ans.begin(), ans.end());
    test_writer.close();
    std::ifstream input("input.txt");
    std::ofstream output("output.txt");
    out_sort<int, std::less<int> >(input, output);
    input.close();
    output.close();
    std::ifstream for_test("output.txt");
    for(int i = 0; i < 1000000; i++)
    {
        int z;
        for_test >> z;
        if(z != ans[i]) std::cout << z << ' ' << ans[i] << std::endl;
    }
    return 0;
}
