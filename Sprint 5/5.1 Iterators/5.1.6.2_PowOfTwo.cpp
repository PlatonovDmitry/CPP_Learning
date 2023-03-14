#include<iostream>
#include<iomanip>

using namespace std;

bool IsPowOfTwo(int num){
    if(num < 1){
        return false;
    }
    
    if(num == 1){
        return true;
    }
    
    if(num % 2 != 0){
        return false;
    }

    return IsPowOfTwo(num / 2);
}

int main() {
    int result = IsPowOfTwo(1024);
    cout << boolalpha;
    cout << result << endl;
}