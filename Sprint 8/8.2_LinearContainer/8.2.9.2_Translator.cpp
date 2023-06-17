#include <cassert>
#include <list>
#include <map>
#include <string>
#include <string_view>


using namespace std;

class Translator {
public:
    void Add(string_view source, string_view target){
        words_.push_front(string(source));
        auto new_source_it = words_.begin();
        words_.push_front(string(target));
        auto new_target_it = words_.begin();

        sources_[*new_source_it] = *new_target_it;
        targets_[*new_target_it] = *new_source_it;
    }
    string_view TranslateForward(string_view source) const{
        if(sources_.count(source)>0){
            return sources_.at(source);
        }

        return ""s;
    }
    string_view TranslateBackward(string_view target) const{
        if(targets_.count(target)>0){
            return targets_.at(target);
        }

        return ""s;
    }

private:
    list<string> words_;
    map<string_view, string_view> sources_;
    map<string_view, string_view> targets_;
};

void TestSimple() {
    Translator translator;
    translator.Add(string("okno"s), string("window"s));
    translator.Add(string("stol"s), string("table"s));

    assert(translator.TranslateForward("okno"s) == "window"s);
    assert(translator.TranslateBackward("table"s) == "stol"s);
    assert(translator.TranslateForward("table"s) == ""s);
}

int main() {
    TestSimple();
    return 0;
} 