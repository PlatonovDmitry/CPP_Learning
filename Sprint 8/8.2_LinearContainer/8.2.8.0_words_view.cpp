#include <vector>
#include <string>

#include "./8.2.7_StackVector/log_duration.h"

using namespace std;


vector<string_view> SplitIntoWordsView(const string& str) {
    vector<string_view> result;
    // 1
    int64_t pos = str.find_first_not_of(" ");
    // 2
    const int64_t pos_end = str.npos;
    // 3
    while (pos != pos_end) {
        // 4
        int64_t space = str.find(' ', pos);
        // 5
        result.push_back(space == pos_end ? str.substr(pos) : str.substr(pos, space - pos));
        // 6
        pos = str.find_first_not_of(" ", space);
    }

    return result;
}

vector<string> SplitIntoWords(const string& text) {
    vector<string> words;
    string word;
    for (const char c : text) {
        if (c == ' ') {
            if (!word.empty()) {
                // обратите внимание на это место в коде
                // здесь мы каждый раз копируем найденное слово
                // в новый элемент в векторе
                words.push_back(word);
                word.clear();
            }
        } else {
            word += c;
        }
    }
    if (!word.empty()) {
        words.push_back(word);
    }
    return words;
}

int main() {
    const string text = "a b";
    {
        LOG_DURATION("string");
        const auto words = SplitIntoWords(text);
        cout << words[0] << "\n";
    }
    {
        LOG_DURATION("string_view");
        const auto words = SplitIntoWordsView(text);
        cout << words[0] << "\n";
    }

    return 0;
}