#include <cassert>
#include <iostream>
#include <string_view>
#include <vector>

using namespace std;

vector<string_view> SplitIntoWordsView(string_view str) {
    vector<string_view> result;    
    while (!str.empty()) {
        int64_t next_char = str.find_first_not_of(" "); 
        if(next_char < 0){
            break;
        }
        str.remove_prefix(next_char);

        int64_t space = str.find(' ');
        if(space < 0)
            space = str.length();

        result.push_back(str.substr(0, space));
        str.remove_prefix(space);
    }

    return result;
}

int main() {
    assert((SplitIntoWordsView("") == vector<string_view>{}));
    assert((SplitIntoWordsView("     ") == vector<string_view>{}));
    assert((SplitIntoWordsView("aaaaaaa") == vector{"aaaaaaa"sv}));
    assert((SplitIntoWordsView("a") == vector{"a"sv}));
    assert((SplitIntoWordsView("a b c") == vector{"a"sv, "b"sv, "c"sv}));
    assert((SplitIntoWordsView("a    bbb   cc") == vector{"a"sv, "bbb"sv, "cc"sv}));
    assert((SplitIntoWordsView("  a    bbb   cc") == vector{"a"sv, "bbb"sv, "cc"sv}));
    assert((SplitIntoWordsView("a    bbb   cc   ") == vector{"a"sv, "bbb"sv, "cc"sv}));
    assert((SplitIntoWordsView("  a    bbb   cc   ") == vector{"a"sv, "bbb"sv, "cc"sv}));
    cout << "All OK" << endl;
}