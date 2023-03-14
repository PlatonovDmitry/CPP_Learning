#include<iostream>

using namespace std;

int Fibonacci(int num){
    if(num < 2){
        return num;
    }

    return Fibonacci(num - 1) + Fibonacci(num - 2);
}

int main() {
    cout << Fibonacci(6) << endl;
}