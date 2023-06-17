#include <functional>
#include <future>
#include <iostream>
#include <map>
#include <set>
#include <sstream>
#include <string>
#include <string_view>

using namespace std;

struct Stats {
    map<string, int> word_frequences;

    void operator+=(const Stats& other) {
        for (const auto& cur_pair : other.word_frequences){
            word_frequences[cur_pair.first]+=cur_pair.second;
        }
    }
};

using KeyWords = set<string, less<>>;

vector<string_view> SplitIntoWords(const string_view& text) {
    string word;
    vector<string_view> output;

    int64_t pos = text.find_first_not_of(" ");
    const int64_t pos_end = text.npos;
    while (pos != pos_end) {
        int64_t space = text.find(' ', pos);
        output.push_back(space == pos_end ? text.substr(pos) : text.substr(pos, space - pos));
        pos = text.find_first_not_of(" ", space);
    }

    return output;
}

Stats ExploreKeyWords(const KeyWords& key_words, string line) {
    Stats output;
    auto words = SplitIntoWords(line);
    for(auto cur_word : words){
        if(key_words.count(cur_word)){
            output.word_frequences[string{cur_word}] += 1;
        }
    }

    return output;
}


Stats ExploreKeyWords(const KeyWords& key_words, istream& input) {
    Stats output;
    string cur_line;
    vector<future<Stats>> stats_vector;
    while (getline(input, cur_line)){
        stats_vector.push_back(async([&output, &key_words, cur_line] { return ExploreKeyWords(key_words, cur_line); }));
    }
    for( auto& cur_task : stats_vector){
        output += cur_task.get();
    }

    return output;
}


int main() {
    const KeyWords key_words = {"yangle", "rocks", "sucks", "all"};

    stringstream ss;
    ss << "this new yangle service really rocks\n";
    ss << "It sucks when yangle isn't available\n";
    ss << "10 reasons why yangle is the best IT company\n";
    ss << "yangle rocks others suck\n";
    ss << "Goondex really sucks, but yangle rocks. Use yangle\n";

    for (const auto& [word, frequency] : ExploreKeyWords(key_words, ss).word_frequences) {
        cout << word << " " << frequency << endl;
    }

    return 0;
}