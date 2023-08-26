#include <algorithm>
#include <cassert>
#include <cmath>
#include <iostream>
#include <optional>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

using namespace std;

// напишите функцию ComputeStatistics, принимающую 5 параметров:
// два итератора, выходное значение для суммы, суммы квадратов и максимального элемента
  template <typename InputIt, typename OutSum, typename OutSqSum, typename OutMax>
  void ComputeStatistics(
    InputIt first, 
    InputIt last, 
    OutSum& out_sum, 
    OutSqSum& out_sq_sum,
    OutMax& out_max)
{
    using Elem = std::decay_t<decltype(*first)>;
      
    constexpr bool need_sum = !is_same_v<OutSum, const nullopt_t>;
    constexpr bool need_sq_sum = !is_same_v<OutSqSum, const nullopt_t>;
    constexpr bool need_max = !is_same_v<OutMax, const nullopt_t>;
  
    Elem sum_res{};
    Elem sq_sum_res{};
    Elem max_res{};

    if constexpr (need_sum){
        sum_res = *first;
    }

    if constexpr (need_sq_sum){
        sq_sum_res = *first * *first;
    }

    if constexpr (need_max){
        max_res = *first;
    }
      
    first++;
    for(;first != last; ++first){
        if constexpr (need_sum){
            sum_res += *first;
        }
        
        if constexpr (need_sq_sum){
            //sq_sum_res += std::pow(*first, 2);
            sq_sum_res += *first * *first;
        }

        if constexpr (need_max){
            //if(*first > out_max){
            //    out_max = *first;
            //}
            max_res = std::max(max_res, *first);
        }
    }
    
    if constexpr (need_sum){
        out_sum = sum_res;
    }

    if constexpr (need_sq_sum){
        out_sq_sum = sq_sum_res;
    }

    if constexpr (need_max){
        out_max = max_res;
    }
}
  


struct OnlySum {
    int value;
};

OnlySum operator+(OnlySum l, OnlySum r) {
    return {l.value + r.value};
}
OnlySum& operator+=(OnlySum& l, OnlySum r) {
    return l = l + r;
}

template<typename T>
struct LoggerType {
    LoggerType(T val) : val_(val) {}
    LoggerType(const LoggerType&) = default;
    LoggerType() = default;

    LoggerType&
        operator+=(const LoggerType& rhs) {
        ++count_plus_;
        val_ += rhs.val_;
        return *this;
    }

    LoggerType
        operator+(const LoggerType& rhs) const {
        LoggerType temp(*this);
        temp += rhs;
        return temp;
    }

    LoggerType&
        operator*=(const LoggerType& rhs) {
        ++count_multiply_;
        val_ *= rhs.val_;
        return *this;
    }

    LoggerType
        operator*(const LoggerType& rhs) const {
        LoggerType temp(*this);
        temp *= rhs;
        return temp;
    }

    bool
        operator<(const LoggerType& rhs) const {
        ++count_compare_;
        return val_ < rhs.val_;
    }

    bool
        operator<=(const LoggerType& rhs) const {
        return !(rhs < *this);
    }

    bool
        operator>(const LoggerType& rhs) const {
        return rhs < *this;
    }

    bool
        operator>=(const LoggerType& rhs) const {
        return !(*this < rhs);
    }

    friend bool operator==(const LoggerType& lhs, const T& rhs) {
        return lhs.val_ == rhs;
    }
    friend bool operator==(const LoggerType& lhs, const LoggerType& rhs) {
        return lhs.val_ == rhs.val_;
    }
    friend ostream& operator<<(ostream& out, const LoggerType& rhs) {
        out << rhs.val_;
        return out;
    }


    static int count_plus_;
    static int count_multiply_;
    static int count_compare_;

    static void Reset() {
        count_plus_ = 0;
        count_multiply_ = 0;
        count_compare_ = 0;
    }
private:
    T val_ = T(0);
};

template<typename T>
int LoggerType<T>::count_plus_ = 0;
template<typename T>
int LoggerType<T>::count_multiply_ = 0;
template<typename T>
int LoggerType<T>::count_compare_ = 0;

using TYPE = LoggerType<int>;

void TestOnlySum() { 
    TYPE::Reset(); 
    vector<TYPE> v1; 
    v1.reserve(3); 
    v1.emplace_back(100); 
    v1.emplace_back(-100); 
    v1.emplace_back(20); 
    TYPE sum; 
    ComputeStatistics(v1.begin(), v1.end(), sum, nullopt, nullopt); 
    assert(sum == 20); 
    assert(TYPE::count_plus_ == 2);
    assert(TYPE::count_multiply_ == 0);
    assert(TYPE::count_compare_ == 0); 
    cout << "TestOnlySum passed"s << endl; 
}

int main() {
    vector input = {1, 2, 3, 4, 5, 6};
    int sq_sum;
    std::optional<int> max;

    // Переданы выходные параметры разных типов - std::nullopt_t, int и std::optional<int>
    ComputeStatistics(input.begin(), input.end(), nullopt, sq_sum, max);

    assert(sq_sum == 91 && max && *max == 6);

    vector<OnlySum> only_sum_vector = {{100}, {-100}, {20}};
    OnlySum sum;

    // Поданы значения поддерживающие только суммирование, но запрошена только сумма
    ComputeStatistics(only_sum_vector.begin(), only_sum_vector.end(), sum, nullopt, nullopt);

    assert(sum.value == 20);

    TestOnlySum();
}