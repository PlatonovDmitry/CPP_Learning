#include <cstdint>
#include <iostream>

using namespace std;

// упростите эту экспоненциальную функцию,
// реализовав линейный алгоритм
int64_t T(int i) {
    if (i <= 1) {
        return 0;
    }
    if (i == 2) {
        return 1;
    }

    int64_t t1 = 0;
    int64_t t2 = 0;
    int64_t t3 = 1;
    int64_t result = 1;

    for(int j = 2; j < i; ++j){
        result = t1 + t2 + t3;
        t1 = t2;
        t2 = t3;
        t3 = result;
    }

    return result;
}

int main() {
    int i;

    while (true) {
        cout << "Enter index: "s;
        if (!(cin >> i)) {
            break;
        }

        cout << "Ti = "s << T(i) << endl;
    }
}