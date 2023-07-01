#include <cmath>
#include <iostream>
#include <string>

using namespace std;

struct Circle {
    double x;
    double y;
    double r;
};

struct Dumbbell {
    Circle circle1;
    Circle circle2;
    string text;
};

struct CircleHasher {
    size_t operator() (const Circle& circle) const {
        size_t h_x = d_hasher_(circle.x);
        size_t h_y = d_hasher_(circle.y);
        size_t h_r = d_hasher_(circle.r);
        
        return h_x + h_y * 37 + h_r * (37 * 37);
    }

private:
    hash<double> d_hasher_;
};

struct DumbbellHash {
    size_t operator() (const Dumbbell& dumbbell) const {
        size_t c1_hash = circle_hasher_(dumbbell.circle1);
        size_t c2_hash = circle_hasher_(dumbbell.circle2);
        size_t text_hash = string_hash_(dumbbell.text);
        
        return text_hash + c1_hash * 37 + c2_hash * 37 * 37 * 37 * 37;
    }
private:
    CircleHasher circle_hasher_;
    hash<string> string_hash_;
};

int main() {
    DumbbellHash hash;
    Dumbbell dumbbell{{10, 11.5, 2.3}, {3.14, 15, -8}, "abc"s};
    cout << "Dumbbell hash "s << hash(dumbbell);
}