#include <chrono>
#include <iostream>
#include <thread>

using namespace std;
using namespace chrono;
// хотите немного магии? тогда используйте namespace literals
using namespace literals;

int main() {
    const auto duration = steady_clock::duration(10h);
    cerr << "Продолжительность сна: "s << chrono::duration_cast<chrono::nanoseconds>(duration).count() << " ns"s << endl;
}