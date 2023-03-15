#include <algorithm>
#include <iostream>
#include <numeric>
#include <sstream>
#include <vector>

using namespace std;

template <typename Iterator>
void PrintRange(Iterator begin, Iterator end){
    for(auto it = begin; it != end; ++it){
        cout << *it << ' ';
    }

    cout << endl;
}

template <typename RandomIt>
void SortedUnion(RandomIt range_begin, RandomIt range_end, RandomIt range_middle){
    RandomIt range_left = range_begin;
    RandomIt range_right = range_middle;
    PrintRange(range_begin, range_end);
    while (range_left != range_middle && range_right <=range_end)
    {
        if(*range_left > *range_right){
            iter_swap(range_left, range_right);
            range_right++;
        } else{
            range_left++;
        }
    }
    PrintRange(range_begin, range_end);
    
}

template <typename RandomIt>
void MergeSort(RandomIt range_begin, RandomIt range_end){
    if(range_end - range_begin > 1){
        RandomIt range_middle = range_begin + (range_end - range_begin) /2;
        MergeSort(range_begin, range_middle);
        MergeSort(range_middle, range_end);
        SortedUnion(range_begin, range_end, range_middle);
    } else if(range_begin != range_end){
        if(*range_begin > *range_end){
            iter_swap(range_begin, range_end);
        }
    }
}


int main() {
    vector<int> test_vector(10);
    // iota             -> http://ru.cppreference.com/w/cpp/algorithm/iota
    // Заполняет диапазон последовательно возрастающими значениями
    iota(test_vector.begin(), test_vector.end(), 1);
    // random_shuffle   -> https://ru.cppreference.com/w/cpp/algorithm/random_shuffle
    // Перемешивает элементы в случайном порядке
    random_shuffle(test_vector.begin(), test_vector.end());
    // Выводим вектор до сортировки
    PrintRange(test_vector.begin(), test_vector.end());
    // Сортируем вектор с помощью сортировки слиянием
    MergeSort(test_vector.begin(), test_vector.end());
    // Выводим результат
    PrintRange(test_vector.begin(), test_vector.end());
    return 0;
}