#include "budget_manager.h"

#include <algorithm>
#include <numeric>

using namespace std;

double BudgetManager::ComputeIncome(Date start_date, Date end_date){
    auto begin = Date::ComputeDistance(START_DATE, start_date);
    auto finish = Date::ComputeDistance(START_DATE, end_date) + 1;

    return transform_reduce(
        income_vector_.begin() + begin,
        income_vector_.begin() + finish,
        spend_vector_.begin() + begin,
        0.0,
        plus<>(),
        minus<>()
    );
}

void BudgetManager::Earn(Date start_date, Date end_date, double income){
    auto days_count = Date::ComputeDistance(start_date, end_date) + 1;
    auto income_per_day = income / days_count;

    auto begin = Date::ComputeDistance(START_DATE, start_date);
    for (size_t i = 0; i < days_count; i++) {
        income_vector_[i + begin]+= income_per_day;
    }
}

void BudgetManager::PayTax(Date start_date, Date end_date, double percent){
    auto days_count = Date::ComputeDistance(start_date, end_date) + 1;
    auto begin = Date::ComputeDistance(START_DATE, start_date);
    for (size_t i = 0; i < days_count; i++) {
        income_vector_[i + begin]*= percent;
    }
}

void BudgetManager::Spend(Date start_date, Date end_date, double spend){
    auto days_count = Date::ComputeDistance(start_date, end_date) + 1;
    auto spend_per_day = spend / days_count;

    auto begin = Date::ComputeDistance(START_DATE, start_date);
    for (size_t i = 0; i < days_count; i++) {
        spend_vector_[i + begin]+= spend_per_day;
    }
}