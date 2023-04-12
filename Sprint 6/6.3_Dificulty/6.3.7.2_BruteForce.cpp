#include <iostream>
#include <string>

using namespace std;

template <typename F>
string BruteForce(F check) {
    // верните строку str, для которой check(str) будет true
    string password = "AAAAA"s;
    int krat1 = 26;
    int krat2 = krat1 * krat1;
    int krat3 = krat2 * krat1;
    int krat4 = krat3 * krat1;
    int krat5 = krat4 * krat1;

    for(int i =0; i < krat5; ++i){
        password = char(i / krat4 + 'A');
        password += char((i % krat4)/krat3 + 'A');
        password += char((i % krat3)/krat2 + 'A');
        password += char((i % krat2)/krat1 + 'A');
        password += char(i % krat1+ 'A');

        if(check(password))
            return password;
    }
    
    return password;
}

int main() {
    string pass = "ZAAAZ"s;
    auto check = [pass](const string& s) {
        return s == pass;
    };
    cout << BruteForce(check) << endl;
}