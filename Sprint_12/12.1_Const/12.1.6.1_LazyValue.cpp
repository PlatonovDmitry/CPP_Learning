#include <cassert>
#include <functional>
#include <mutex>
#include <optional>
#include <string>

using namespace std;

template <typename T>
class LazyValue {
public:
    explicit LazyValue(function<T()> init):
        init_(init)
    {

    }

    bool HasValue() const{
        return stored_value_.has_value();
    }

    const T& Get() const{
        if (!stored_value_.has_value()){
            lock_guard lock(value_guard_);
            stored_value_ = init_();
        }

        return stored_value_.value();
    }

private:
    function<T()> init_;
    mutable optional<T> stored_value_;
    mutable mutex value_guard_;
};

void UseExample() {
    const string big_string = "Giant amounts of memory"s;

    LazyValue<string> lazy_string([&big_string] {
        return big_string;
    });

    assert(!lazy_string.HasValue());
    assert(lazy_string.Get() == big_string);
    assert(lazy_string.Get() == big_string);
}

void TestInitializerIsntCalled() {
    bool called = false;

    {
        LazyValue<int> lazy_int([&called] {
            called = true;
            return 0;
        });
    }
    assert(!called);
}

int main() {
    UseExample();
    TestInitializerIsntCalled();
}