#include <algorithm>
#include <iostream>
#include <numeric>
#include <sstream>
#include <vector>

using namespace std;

// функция, записывающая элементы диапазона в строку
template <typename It>
string PrintRangeToString(It range_begin, It range_end) {
    // удобный тип ostringstream -> https://ru.cppreference.com/w/cpp/io/basic_ostringstream
    ostringstream out;
    for (auto it = range_begin; it != range_end; ++it) {
        out << *it << " "s;
    }
    out << endl;
    // получаем доступ к строке с помощью метода str для ostringstream
    return out.str();
}

template <typename It>
void GetPermutation(It range_begin, It range_end, It range_middle, vector<string>& output){
    if(range_middle == range_end){
        output.push_back(PrintRangeToString(range_begin, range_end));
    } else{
        for(int i = 0; i < range_end - range_middle; ++i){    
            rotate(range_middle, range_middle + 1, range_end);
            GetPermutation(range_begin, range_end, range_middle + 1, output);
        }
    }
}

template <typename It>
vector<string> GetPermutations(It range_begin, It range_end){
    vector<string> output;
    
    GetPermutation(range_begin, range_end, range_begin, output);
    
    return output;
}

int main() {
    vector<int> permutation(4);
    // iota             -> http://ru.cppreference.com/w/cpp/algorithm/iota
    // Заполняет диапазон последовательно возрастающими значениями
    iota(permutation.begin(), permutation.end(), 1);
    auto result = GetPermutations(permutation.begin(), permutation.end());
    for (const auto& s : result) {
        cout << s;
    }
    return 0;
}