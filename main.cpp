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
        return cmp()(x.first, y.first);
    }
};

template<class T, class cmp>
void out_sort(std::ifstream &input, std::ofstream &output)
{
    cmp comparator;
    std::vector <T> storage;
    std::priority_queue<std::pair<T, std::ifstream*>, std::vector<std::pair<T, std::ifstream*> >, cmp_for_queue<std::pair<T, std::ifstream*>, cmp> > files_heap;
    T for_input;
    int number_of_files = 0, number_of_elements = 0;
    char temporary_file_name[10];
    while(input >> for_input)
    {
        storage.push_back(for_input);
        number_of_elements++;
        if(number_of_elements == 100000)
        {
            sprintf(temporary_file_name, "%d.txt", number_of_files);
            std::ofstream writer(temporary_file_name);
            std::sort(storage.begin(), storage.end(), comparator);
            for(int i = 0; i < 100000; i++)    writer << storage[i] << ' ';
            writer.close();
            std::pair<T, std::ifstream*> add_to_heap;
            add_to_heap.second = new std::ifstream(temporary_file_name);
            *add_to_heap.second >> add_to_heap.first;
            files_heap.push(add_to_heap);
            number_of_files++;
            number_of_elements = 0;
        }
    }
    std::pair<T, std::ifstream*> pop_from_heap;
    while(files_heap.size())
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

void generate_test(int k, std::ofstream *writer)
{
    for(int i = 0; i < k; i++)
        *writer << rand() << ' ';
}


int main()
{
    srand(time(NULL));
    std::ofstream test_writer("input.txt");
    generate_test(1000000, &test_writer);
    test_writer.close();
    std::ifstream input("input.txt");
    std::ofstream output("output.txt");
    out_sort<int, std::less<int> >(input, output);
    return 0;
}
