#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

enum class Gender { FEMALE, MALE };

struct Person {
    int age;           // возраст
    Gender gender;     // пол
    bool is_employed;  // имеет ли работу
};

template <typename InputIt>
int ComputeMedianAge(InputIt range_begin, InputIt range_end) {
    if (range_begin == range_end) {
        return 0;
    }
    vector<typename InputIt::value_type> range_copy(range_begin, range_end);
    auto middle = range_copy.begin() + range_copy.size() / 2;
    nth_element(range_copy.begin(), middle, range_copy.end(), [](const Person& lhs, const Person& rhs) {
        return lhs.age < rhs.age;
    });
    return middle->age;
}

// напишите сигнатуру и реализацию функции PrintStats
void PrintStats(vector<Person> people) {
    cout << "Median age = "s << ComputeMedianAge(people.begin(), people.end()) << endl;

    auto femails_iter_end = partition(
        people.begin(), 
        people.end(), 
        [](const Person& person) {return person.gender == Gender::FEMALE; });

    cout << "Median age for females = "s << ComputeMedianAge(people.begin(), femails_iter_end) << endl;
    cout << "Median age for males = "s << ComputeMedianAge(femails_iter_end, people.end()) << endl;

    auto employed_female_iter_end = partition(
        people.begin(), 
        femails_iter_end, 
        [](const Person& person) {return person.is_employed; });

    cout << "Median age for employed females = "s << ComputeMedianAge(people.begin(), employed_female_iter_end) << endl;
    cout << "Median age for unemployed females = "s << ComputeMedianAge(employed_female_iter_end, femails_iter_end) << endl;

    auto employed_male_iter_end = partition(
        femails_iter_end, 
        people.end(), 
        [](const Person& person) {return person.is_employed; });

    cout << "Median age for employed males = " << ComputeMedianAge(femails_iter_end, employed_male_iter_end) << endl;
    cout << "Median age for unemployed males = "s << ComputeMedianAge(employed_male_iter_end, people.end()) << endl;
}

int main() {
    vector<Person> persons = {
        {31, Gender::MALE, false},   {40, Gender::FEMALE, true},  {24, Gender::MALE, true},
        {20, Gender::FEMALE, true},  {80, Gender::FEMALE, false}, {78, Gender::MALE, false},
        {10, Gender::FEMALE, false}, {55, Gender::MALE, true},
    };
    PrintStats(persons);
}