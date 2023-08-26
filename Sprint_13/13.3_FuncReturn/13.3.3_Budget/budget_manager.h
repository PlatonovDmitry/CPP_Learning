#pragma once
#include "date.h"

#include <string>
#include <vector>

class BudgetManager {
public:
    inline static const Date START_DATE{2000, 1, 1};
    inline static const Date END_DATE{2100, 1, 1};

    BudgetManager(): 
        income_vector_((Date::ComputeDistance(START_DATE, END_DATE))),
        spend_vector_((Date::ComputeDistance(START_DATE, END_DATE)))
    {
        
    }

    double ComputeIncome(Date start_date, Date end_date);
    void Earn(Date start_date, Date end_date, double income);
    void PayTax(Date start_date, Date end_date, double percent = 0.87);
    void Spend(Date start_date, Date end_date, double spend);

private:
    std::vector<double> income_vector_;
    std::vector<double> spend_vector_;
};
