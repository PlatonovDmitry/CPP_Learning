#include <algorithm>
#include <execution>
#include <future>
#include <iostream>
#include <list>
#include <random>
#include <string>
#include <string_view>
#include <type_traits>
#include <vector>

#include "../log_duration.h"

using namespace std;

string GenerateWord(mt19937& generator, int max_length) {
    const int length = uniform_int_distribution(1, max_length)(generator);
    string word;
    word.reserve(length);
    for (int i = 0; i < length; ++i) {
        word.push_back(uniform_int_distribution('a', 'z')(generator));
    }
    return word;
}

template <template <typename> typename Container>
Container<string> GenerateDictionary(mt19937& generator, int word_count, int max_length) {
    vector<string> words;
    words.reserve(word_count);
    for (int i = 0; i < word_count; ++i) {
        words.push_back(GenerateWord(generator, max_length));
    }
    return Container(words.begin(), words.end());
}

struct Reverser {
    void operator()(string& value) const {
        reverse(value.begin(), value.end());
    }
};

template <typename Container, typename Function>
void Test(string_view mark, Container keys, Function function) {
    LOG_DURATION(mark);
    function(keys, Reverser{});
}

#define TEST(function) Test(#function, keys, function<remove_const_t<decltype(keys)>, Reverser>)

template <typename ForwardRange, typename Function>
void ForEach(ForwardRange& range, Function function) {
    int core_count = 4;
    int core_size = ceil(range.size() *1.0 / core_count);
    auto range_start = range.begin();
    auto range_end = range.begin();
    vector<future<void>> tasks;
    for(size_t i = 0; i < range.size(); i++){
        ++range_end;
        if(i % core_size == 0){
            tasks.push_back(async([range_start, range_end, &function]{ for_each(range_start, range_end, function);}));
            range_start = range_end;
        }
    }
    
    for_each(range_start, range.end(), function);

    for(auto& cur_task : tasks){
        cur_task.get();
    }
}

int main() {
    // для итераторов с произвольным доступом тоже должно работать
    vector<string> strings = {"cat", "dog", "code"};

    ForEach(strings, [](string& s) {
        reverse(s.begin(), s.end());
    });

    for (string_view s : strings) {
        cout << s << " ";
    }
    cout << endl;
    // вывод: tac god edoc

    mt19937 generator;
    const auto keys = GenerateDictionary<list>(generator, 50'000, 5'000);

    TEST(ForEach);

    return 0;
}