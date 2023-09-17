#include <cstdint>
#include <iostream>
#include <climits>

using namespace std;

int main() {
    int64_t a;
    int64_t b;

    cin >> a >> b;

    if(a > 0 && b > 0 && INT64_MAX - a < b){
        cout << "Overflow!"s << endl;
    } else if (a < 0 && b < 0 && b < INT64_MIN - a)
    {
        cout << "Overflow!"s << endl;
    } else{
        cout << a + b << endl;
    }
}