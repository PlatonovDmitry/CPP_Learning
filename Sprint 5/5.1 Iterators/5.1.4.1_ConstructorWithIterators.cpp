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

template <typename Iterator>
auto MakeVector(Iterator begin, Iterator end){
    return vector(begin, end);
}

int main() {
    set<string> unique_langs = {"Python"s, "Java"s, "C#"s, "Ruby"s, "C++"s};
    vector<string> langs = MakeVector(unique_langs.begin(), unique_langs.end());
    PrintRange(langs.begin(), langs.end());
}