#include <algorithm>
#include <iostream>
#include <vector>
#include <random>
#include <map>

#include "../6.1_profile/log_duration.h"

using namespace std;

// функция возвращает true, если векторы из одинаковых элементов
// перепишите эту функцию, улучшив её асимптотическую сложность
bool TestPermut(const vector<int>& v1, const vector<int>& v2) {
    // если они разной длины, элементы заведомо разные
    if (v1.size() != v2.size()) {
        return false;
    }

    vector<int> v1c;
    vector<int> v2c;
    copy(v1.begin(), v1.end(), back_inserter(v1c));
    copy(v2.begin(), v2.end(), back_inserter(v2c));
    
    sort(v1c.begin(), v1c.end());
    sort(v2c.begin(), v2c.end());

    for(int i = 0; i < v1c.size(); ++i){
        if(v1c[i]!= v2c[i]){
            return false;
        }
    }
    
    return true;
}

bool TestPermut2(const vector<int>& v1, const vector<int>& v2) {
    // если они разной длины, элементы заведомо разные
    if (v1.size() != v2.size()) {
        return false;
    }

    map<int, int> dictionary;
    for(int i = 0; i < v1.size(); ++i){
        dictionary[v1[i]]++;
        dictionary[v2[i]]--;
    }
    
    for(auto [key, value] : dictionary){
        if(value != 0){
            return false;
        }
    }

    return true;
}

int main() {
    std::mt19937 g;

    int n;
    cin >> n;
    vector<int> v1, v2;
    v1.reserve(n);
    v2.reserve(n);

    for (int i = 0; i < n; ++i) {
        v1.push_back(rand());
        v2.push_back(rand());
    }

    {
        // оба вектора случайны, вряд ли они совпадут
        cout << "Random vectors match? "s << flush;
        LOG_DURATION("TestPermut 1"s);
        cout << (TestPermut(v1, v2) ? "Yes"s : "No"s) << endl;
    }

    {
        // оба вектора случайны, вряд ли они совпадут
        cout << "Random vectors match? "s << flush;
        LOG_DURATION("TestPermut 2"s);
        cout << (TestPermut2(v1, v2) ? "Yes"s : "No"s) << endl;
    }
    
    // делаем один перестановкой другого явным образом
    v2 = v1;
    shuffle(v2.begin(), v2.end(), g);
    cout << "Permuted vectors match? "s << flush;
    cout << (TestPermut(v1, v2) ? "Yes"s : "No"s) << endl;
}