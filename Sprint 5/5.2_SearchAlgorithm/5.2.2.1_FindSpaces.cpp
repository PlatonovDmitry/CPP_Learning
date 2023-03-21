#include <algorithm>
#include <iostream>
#include <string>

using namespace std;

void PrintSpacesPositions(string& str) {
    auto search_result = find(str.begin(), str.end(), ' ');

    while (search_result != str.end()){
        cout << search_result - str.begin() << endl;
        search_result = find(search_result + 1, str.end(), ' ');
    }     
}

int main() {
    string str = "He said: one and one and one is three"s;
    PrintSpacesPositions(str);
    return 0;
}