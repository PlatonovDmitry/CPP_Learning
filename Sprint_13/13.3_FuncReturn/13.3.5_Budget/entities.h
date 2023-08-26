#pragma once

struct DayBudget
{
    double income;
    double spend;

    DayBudget operator+(const DayBudget& other) const{
        return {income + other.income, spend + other.spend};
    }
};
