#include <iostream>
#include <string>

using namespace std;

template <typename TFunc>
void RunTestImpl(TFunc func, const string& func_name) {
    func();
    cerr << func_name << " OK"s << endl;
}

#define RUN_TEST(func)  RunTestImpl(func, #func)

void Test1() {
}

int main() {
    RUN_TEST(Test1);
}