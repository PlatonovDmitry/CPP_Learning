#include <iostream>
#include <map>
#include <set>
#include <sstream>
#include <string>
#include <cstdlib>
#include <iomanip>
#include <vector>

using namespace std;

template <typename container_type>
ostream& Print(ostream& out, const container_type& container){
    bool not_first = false;
    for (const auto& element : container) {
        if(not_first){
            out << ", "s;
        }
        else{
            not_first = true;
        }

        out << element;
    }
    return out;
}

template <typename type>
ostream& operator<<(ostream& out, const vector<type>& container) {
    out << "["s;
    Print(out, container);
    out << "]"s;
    
    return out;
}  

template <typename type>
ostream& operator<<(ostream& out, const set<type>& container) {
    out << "{"s;
    Print(out, container);
    out << "}"s;
    
    return out;
}  

template <typename key_type, typename value_type>
ostream& operator<<(ostream& out, const map<key_type, value_type>& container) {
    out << "{"s;
    Print(out, container);
    out << "}"s;
    
    return out;
}  

template <typename key_type, typename value_type>
ostream& operator<<(ostream& out, const pair<key_type, value_type>& pair) {
    out << pair.first;
    out << ": "s;
    out << pair.second;
        
    return out;
}  


template <typename T, typename U>
void AssertEqualImpl(const T& t, const U& u, const string& t_str, const string& u_str, const string& file,
                     const string& func, unsigned line, const string& hint) {
    if (t != u) {
        cout << boolalpha;
        cout << file << "("s << line << "): "s << func << ": "s;
        cout << "ASSERT_EQUAL("s << t_str << ", "s << u_str << ") failed: "s;
        cout << t << " != "s << u << "."s;
        if (!hint.empty()) {
            cout << " Hint: "s << hint;
        }
        cout << endl;
        abort();
    }
}

#define ASSERT_EQUAL(a, b) AssertEqualImpl((a), (b), #a, #b, __FILE__, __FUNCTION__, __LINE__, ""s)

#define ASSERT_EQUAL_HINT(a, b, hint) AssertEqualImpl((a), (b), #a, #b, __FILE__, __FUNCTION__, __LINE__, (hint))

void AssertImpl(bool value, const string& expr_str, const string& file, const string& func, unsigned line,
                const string& hint) {
    if (!value) {
        cout << file << "("s << line << "): "s << func << ": "s;
        cout << "ASSERT("s << expr_str << ") failed."s;
        if (!hint.empty()) {
            cout << " Hint: "s << hint;
        }
        cout << endl;
        abort();
    }
}

#define ASSERT(expr) AssertImpl(!!(expr), #expr, __FILE__, __FUNCTION__, __LINE__, ""s)

#define ASSERT_HINT(expr, hint) AssertImpl(!!(expr), #expr, __FILE__, __FUNCTION__, __LINE__, (hint))

class Synonyms {
public:
    void Add(const string& first_word, const string& second_word) {
        synonyms_[first_word].insert(second_word);
        synonyms_[second_word].insert(first_word);
    }

    size_t GetSynonymCount(const string& word) const {
        if (synonyms_.count(word) != 0) {
            return synonyms_.at(word).size();
        }
        return 0;
    }

    bool AreSynonyms(const string& first_word, const string& second_word) const {
        if(synonyms_.count(first_word) > 0){
            if(synonyms_.at(first_word).count(second_word) > 0){
                return true;
            }
        }

        return false;
    }

private:
    map<string, set<string>> synonyms_;
};

void TestAddingSynonymsIncreasesTheirCount() {
    Synonyms synonyms;
    ASSERT(synonyms.GetSynonymCount("music"s) == 0);
    ASSERT(synonyms.GetSynonymCount("melody"s) == 0);

    synonyms.Add("music"s, "melody"s);
    ASSERT(synonyms.GetSynonymCount("music"s) == 1);
    ASSERT(synonyms.GetSynonymCount("melody"s) == 1);

    synonyms.Add("music"s, "tune"s);
    ASSERT(synonyms.GetSynonymCount("music"s) == 2);
    ASSERT(synonyms.GetSynonymCount("tune"s) == 1);
    ASSERT(synonyms.GetSynonymCount("melody"s) == 1);
}

void TestAreSynonyms() {
    Synonyms synonyms;

    ASSERT(!synonyms.AreSynonyms("music"s, "melody"s));
    
    synonyms.Add("music"s, "melody"s);
    ASSERT(synonyms.AreSynonyms("music"s, "melody"s));
    ASSERT(synonyms.AreSynonyms("melody"s, "music"s));
    ASSERT(!synonyms.AreSynonyms("music"s, "tune"s));

    synonyms.Add("music"s, "tune"s);
    ASSERT(synonyms.AreSynonyms("music"s, "tune"s));
    ASSERT(!synonyms.AreSynonyms("tune"s, "melody"s));
    
}

void TestSynonyms() {
    TestAddingSynonymsIncreasesTheirCount();
    TestAreSynonyms();
}

int main() {
    TestSynonyms();

    Synonyms synonyms;

    string line;
    while (getline(cin, line)) {
        istringstream command(line);
        string action;
        command >> action;

        if (action == "ADD"s) {
            string first_word, second_word;
            command >> first_word >> second_word;
            synonyms.Add(first_word, second_word);
        } else if (action == "COUNT"s) {
            string word;
            command >> word;
            cout << synonyms.GetSynonymCount(word) << endl;
        } else if (action == "CHECK"s) {
            string first_word, second_word;
            command >> first_word >> second_word;
            if (synonyms.AreSynonyms(first_word, second_word)) {
                cout << "YES"s << endl;
            } else {
                cout << "NO"s << endl;
            }
        } else if (action == "EXIT"s) {
            break;
        }
    }
}