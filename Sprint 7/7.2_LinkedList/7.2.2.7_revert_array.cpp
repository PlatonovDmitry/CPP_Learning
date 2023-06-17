#include <cassert>
#include <vector>

using namespace std;

template <typename T>
void ReverseArray(T* start, size_t size) {
    for(int i = 0; i < size / 2; ++i){
        auto temp = start[i];
        start[i] = start[size - i - 1];
        start[size - i - 1] = temp;
    }
}

int main() {
    using namespace std;
    
    vector<int> v = {1, 2};
    ReverseArray(v.data(), v.size());
    assert(v == (vector<int>{2, 1}));
}