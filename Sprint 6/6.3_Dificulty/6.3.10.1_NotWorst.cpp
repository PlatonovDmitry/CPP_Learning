#include <algorithm>
#include <cmath>
#include <cstdint>
#include <iostream>
#include <random>
#include <vector>

using namespace std;

int EffectiveCount(const vector<int>& v, int n, int i) {
    double expected_result = static_cast<int64_t>(v.size()) * (i + 1) / (n + 1);
    double log_speed = log2(v.size());

    if(log_speed > expected_result){
        auto f_if = find_if(v.begin(), v.end(), [i](int curElement){return curElement > i;});
        return f_if - v.begin();
    } else {
        auto up_b = upper_bound(v.begin(), v.end(), i);
        return up_b - v.begin();
    }
}

int main() {
    static const int NUMBERS = 1'000'000;
    static const int MAX = 1'000'000'000;

    mt19937 r;
    uniform_int_distribution<int> uniform_dist(0, MAX);

    vector<int> nums;
    for (int i = 0; i < NUMBERS; ++i) {
        int random_number = uniform_dist(r);
        nums.push_back(random_number);
    }
    sort(nums.begin(), nums.end());

    int i;
    cin >> i;
    int result = EffectiveCount(nums, MAX, i);
    cout << "Total numbers before "s << i << ": "s << result << endl;
}