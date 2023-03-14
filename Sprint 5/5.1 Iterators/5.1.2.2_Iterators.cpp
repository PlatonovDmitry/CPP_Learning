#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

struct Lang {
    string name;
    int age;
};

int main() {
    vector<Lang> langs = {{"Python"s, 29}, {"Java"s, 24}, {"C#"s, 20}, {"Ruby"s, 25}, {"C++"s, 37}};
    
    auto found_languages = find_if(langs.begin(), langs.end(), [](const auto& lang){return lang.name[0] == 'J';});

    cout << found_languages->name << ", " << found_languages->age << endl;
}