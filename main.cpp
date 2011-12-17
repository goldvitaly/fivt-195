#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <queue>
#include <ctime>


template <class T, class cmp>
class reverse_cmp
{
public:
    bool operator()(const T &x, const T &y) const
    {
        return cmp()(y.first, x.first);
    }
};



template<class T, class cmp>
void out_sort(std::ifstream &input, std::ofstream &output, size_t block_size)
{
    cmp comparator;
    std::vector <T> storage;
    std::priority_queue<std::pair<T, std::ifstream*>, std::vector<std::pair<T, std::ifstream*> >, reverse_cmp<std::pair<T, std::ifstream*>, cmp> > files_heap;
    T for_input;
    int number_of_files = 0;
    char temporary_file_name[10];
    while(input >> for_input)
    {
        storage.push_back(for_input);
        if(storage.size() == block_size  ||  (input.eof()  &&  !storage.empty()))
        {
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

    while(!files_heap.empty())
    {
        std::pair<T, std::ifstream*> next_to_output;
        next_to_output = files_heap.top();
        files_heap.pop();
        output << next_to_output.first << ' ';
        if(!next_to_output.second -> eof())
        {
            *next_to_output.second >> next_to_output.first;
            files_heap.push(next_to_output);
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
    out_sort<int, std::less<int> >(input, output, 100000);
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
