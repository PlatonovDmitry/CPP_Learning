#include <deque>
#include <iostream>

using namespace std;

int main() {
    deque<int> numbers = {1};
    auto it = numbers.begin();
    cout << *it << endl;

    for (int i = 0; i < 1024; ++i) {
        numbers.push_back(i);
    }
    // попытаемся вывести последний элемент из дека
    cout << *it << " " << *(it + numbers.size() - 1) << endl;
}