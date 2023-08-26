#include "parser.h"

using namespace std;

string_view Trim(string_view line){
    string ignore_symbols = "\\\t\r\n "s;
    string_view output = line;
    while (ignore_symbols.find_first_of(output[0]) != string::npos){
        output = output.substr(1);
    }

    while (ignore_symbols.find_first_of(output[output.size() - 1]) != string::npos){
        output = output.substr(0, output.size() - 1);
    }

    return output;   
}

vector<string_view> SplitLine(string_view line){
    vector<string_view> output;

    ssize_t start_pos = 0;
    while (start_pos < line.size())
    {
        auto last_pos = line.find_first_of(' ', start_pos);
        if(last_pos == string::npos){
            last_pos = line.size();
        }

        output.push_back(Trim(line.substr(start_pos, last_pos - start_pos)));
        start_pos = last_pos + 1;
    }
    
    return output;
}

Query ParseQuery(string_view line){
    Query output;

    auto params = SplitLine(line);

    output.start_date = Date::FromString(params[1]);
    output.finish_date = Date::FromString(params[2]);

    if(params[0] == "ComputeIncome"s){
        output.type = QueryType::COMPUTE_INCOME;
    } else if(params[0] == "Earn"s){
        output.type = QueryType::EARN;
        output.earn = stod(string(params[3]));
    } else if(params[0] == "PayTax"s){
        output.type = QueryType::PAY_TAX;
        output.earn = params.size()== 4 ?
            (100.0 - stoi(string(params[3]))) / 100:
            0.87;
    }else if(params[0] == "Spend"s){
        output.type = QueryType::SPEND;
        output.earn = stod(string(params[3]));
    }

    return output;
}