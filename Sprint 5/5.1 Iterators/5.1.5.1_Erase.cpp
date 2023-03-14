#include <iostream>
#include <set>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

template <typename Iterator>
void PrintRange(Iterator begin, Iterator end){
    for(auto it = begin; it != end; ++it){
        cout << *it << ' ';
    }

    cout << endl;
}

template <typename Range>
void EraseAndPrint(Range& range, int erase_num, int sub_begin, int sub_end){
    range.erase(range.begin() + erase_num);
    PrintRange(range.begin(), range.end());
    range.erase(range.begin() + sub_begin, range.begin() + sub_end);
    PrintRange(range.begin(), range.end());
}

int main() {
    vector<string> langs = {"Python"s, "Java"s, "C#"s, "Ruby"s, "C++"s};
    EraseAndPrint(langs, 0, 0, 2);
    return 0;
}