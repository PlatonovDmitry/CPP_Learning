#include <algorithm>
#include <cassert>
#include <iterator>
#include <string>
#include <vector>

using namespace std;

// Объявляем Sentence<Token> для произвольного типа Token
// синонимом vector<Token>.
// Благодаря этому в качестве возвращаемого значения
// функции можно указать не малопонятный вектор векторов,
// а вектор предложений — vector<Sentence<Token>>.
template <typename Token>
using Sentence = vector<Token>;

template <typename TokenForwardIt>
TokenForwardIt FindSentenceEnd(TokenForwardIt tokens_begin, TokenForwardIt tokens_end) {
    const TokenForwardIt before_sentence_end
        = adjacent_find(tokens_begin, tokens_end, [](const auto& left_token, const auto& right_token) {
              return left_token.IsEndSentencePunctuation() && !right_token.IsEndSentencePunctuation();
          });
    return before_sentence_end == tokens_end ? tokens_end : next(before_sentence_end);
}

// Класс Token имеет метод bool IsEndSentencePunctuation() const
template <typename Token>
vector<Sentence<Token>> SplitIntoSentences(vector<Token> tokens) {
    // Напишите реализацию функции, не копируя объекты типа Token
    vector<Sentence<Token>> output;
    auto start_it = tokens.begin();
    while (start_it != tokens.end())
    {
        auto end_it = FindSentenceEnd(start_it, tokens.end());
        output.push_back({make_move_iterator(start_it), make_move_iterator(end_it)});
        start_it = end_it;
    }
    
    return output;
}

struct TestToken {
    string data;
    bool is_end_sentence_punctuation = false;

    bool IsEndSentencePunctuation() const {
        return is_end_sentence_punctuation;
    }
    bool operator==(const TestToken& other) const {
        return data == other.data && is_end_sentence_punctuation == other.is_end_sentence_punctuation;
    }
};

ostream& operator<<(ostream& stream, const TestToken& token) {
    return stream << token.data;
}

// Тест содержит копирования объектов класса TestToken.
// Для проверки отсутствия копирований в функции SplitIntoSentences
// необходимо написать отдельный тест.
void TestSplitting() {
    assert(SplitIntoSentences(vector<TestToken>({{"Split"s}, {"into"s}, {"sentences"s}, {"!"s}}))
           == vector<Sentence<TestToken>>({{{"Split"s}, {"into"s}, {"sentences"s}, {"!"s}}}));

    assert(SplitIntoSentences(vector<TestToken>({{"Split"s}, {"into"s}, {"sentences"s}, {"!"s, true}}))
           == vector<Sentence<TestToken>>({{{"Split"s}, {"into"s}, {"sentences"s}, {"!"s, true}}}));

    assert(SplitIntoSentences(vector<TestToken>(
               {{"Split"s}, {"into"s}, {"sentences"s}, {"!"s, true}, {"!"s, true}, {"Without"s}, {"copies"s}, {"."s, true}}))
           == vector<Sentence<TestToken>>({
               {{"Split"s}, {"into"s}, {"sentences"s}, {"!"s, true}, {"!"s, true}},
               {{"Without"s}, {"copies"s}, {"."s, true}},
           }));
}

int main() {
    TestSplitting();
}