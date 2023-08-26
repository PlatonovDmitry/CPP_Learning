#pragma once
#include "date.h"

#include <string_view>
#include <vector>

enum QueryType{
    EARN,
    PAY_TAX,
    COMPUTE_INCOME,
    SPEND
};

struct Query
{
    QueryType type;
    Date start_date;
    Date finish_date;
    double earn;
};

std::string_view Trim(std::string_view line);
std::vector<std::string_view> SplitLine(std::string_view line);
Query ParseQuery(std::string_view line);