#include <iostream>
#include <set>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

template <typename Iterator>
void PrintRange(Iterator begin, Iterator end){
    while(begin != end){
        cout << *begin++ << ' ';
    }

    cout << endl;
}

template <typename Range, typename RangeIteType>
void FindAndPrint(Range range, RangeIteType value){
    auto result = find_if(range.begin(), range.end(), [value](RangeIteType cur_val) {
        return cur_val == value;
    });

    PrintRange(range.begin(), result);
    PrintRange(result, range.end());
}

int main() {
    set<int> test = {1, 1, 1, 2, 3, 4, 5, 5};
    cout << "Test1"s << endl;
    FindAndPrint(test, 3);
    cout << "Test2"s << endl;
    FindAndPrint(test, 0); // элемента 0 нет в контейнере
    cout << "End of tests"s << endl;
}