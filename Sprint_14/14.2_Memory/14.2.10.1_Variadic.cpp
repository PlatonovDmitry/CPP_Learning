#include <cassert>
#include <string>
#include <string_view>

using namespace std;

/* Напишите вашу реализацию EqualsToOneOf здесь*/
template <typename T, typename... Types>
bool EqualsToOneOf(const T& comp, Types... values) {  
    if constexpr (sizeof...(values) == 0) {
        return false;
    }

    return ((comp == values) || ...); 
} 

int main() {
    assert(EqualsToOneOf("hello"sv, "hi"s, "hello"s));
    assert(!EqualsToOneOf(1, 10, 2, 3, 6));
    assert(!EqualsToOneOf(8));
}