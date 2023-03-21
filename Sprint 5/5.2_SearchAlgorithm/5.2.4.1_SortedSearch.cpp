#include <algorithm>
#include <iostream>
#include <string>
#include <set>

using namespace std;

set<int>::const_iterator FindNearestElement(const set<int>& numbers, int border) {
    // set<int>::const_iterator — тип итераторов для константного множества целых чисел
    auto result = numbers.lower_bound(border);
    if(*result == border || result == numbers.begin()){
        return result;
    }

    if(result == numbers.end()){
        return --result;
    }
 
    auto low_result = result;
    int delta = *result - border;
    int delta2 = border - *(--low_result);
    
    if(delta >= delta2){
        return low_result;
    } else{
        return result;
    }
}

int main() {
    set<int> numbers = {1, 4, 6};
    cout << *FindNearestElement(numbers, 0) << " " << *FindNearestElement(numbers, 3) << " "
         << *FindNearestElement(numbers, 5) << " " << *FindNearestElement(numbers, 6) << " "
         << *FindNearestElement(numbers, 100) << endl;
    set<int> empty_set;
    cout << (FindNearestElement(empty_set, 8) == end(empty_set)) << endl;
    return 0;
}