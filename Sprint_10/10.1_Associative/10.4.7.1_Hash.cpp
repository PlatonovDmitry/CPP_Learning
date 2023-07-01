#include <algorithm>
#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include <string>
#include <sstream>

using namespace std;

template <typename Hash>
int FindCollisions(const Hash& hasher, istream& text) {
    unordered_map<size_t, unordered_set<string>> now_parked_;
    string cur_word;
    while(text >> cur_word){
        now_parked_[hasher(cur_word)].insert(move(cur_word));
    }

    int output = 0;
    for_each(
        now_parked_.begin(),
        now_parked_.end(),
        [&output](const auto& pair){output += pair.second.size() -1;}
    );

    return output;
}

// Это плохой хешер. Его можно использовать для тестирования.
// Подумайте, в чём его недостаток
struct HasherDummy {
    size_t operator() (const string& str) const {
        size_t res = 0;
        for (char c : str) {
            res += static_cast<size_t>(c);
        }
        return res;
    }
};

struct DummyHash {
    size_t operator()(const string&) const {
        return 42;
    }
};

int main() {
    DummyHash dummy_hash;
    hash<string> good_hash;

    {
        istringstream stream("I love C++"s);
        cout << FindCollisions(dummy_hash, stream) << endl;
    }
    {
        istringstream stream("I love C++"s);
        cout << FindCollisions(good_hash, stream) << endl;
    }
} 