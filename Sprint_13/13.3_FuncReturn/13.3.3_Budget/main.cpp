#include "budget_manager.h"
#include "parser.h"
#include "date.h"

#include <iostream>
#include <iomanip>
#include <fstream>
#include <limits>
#include <numbers>
#include <string_view>

using namespace std;

void ParseAndProcessQuery(BudgetManager& manager, string_view line) {
    auto query = ParseQuery(line);

    switch (query.type)
    {
    case QueryType::PAY_TAX:
        manager.PayTax(query.start_date, query.finish_date, query.earn);
        break;
    case QueryType::EARN:
        manager.Earn(query.start_date, query.finish_date, query.earn);
        break;
    case QueryType::SPEND:
        manager.Spend(query.start_date, query.finish_date, query.earn);
        break;
    case QueryType::COMPUTE_INCOME:
        auto result = manager.ComputeIncome(query.start_date, query.finish_date);
        if(result > 0){
            result += 0.0000001;
        }
        cout << result << endl;
        break;
    }
}

int ReadNumberOnLine(istream& input) {
    std::string line;
    std::getline(input, line);
    return std::stoi(line);
}

int main() {
    BudgetManager manager;
    
    //ifstream source("Tests/Input_2.txt");
    const int query_count = ReadNumberOnLine(cin);

    for (int i = 0; i < query_count; ++i) {
        std::string line;
        std::getline(cin, line);
        ParseAndProcessQuery(manager, line);
    }
}
