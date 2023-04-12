#include "log_duration.h"

#include <iostream>
#include <numeric>
#include <random>
#include <string>
#include <vector>

using namespace std;

vector<float> ComputeAvgTemp(const vector<vector<float>>& measures) {
    vector<float> output;
    if(measures.size() == 0){
        return output;
    }

    auto day_masure_count = measures[0].size();
    output = vector<float>(day_masure_count);
    auto positive_measure_count = vector<int>(day_masure_count);
    for(int i =0; i < measures.size(); ++i){
        for(int j = 0; j < day_masure_count; ++j){
            output[j] += (measures[i][j] >= 0 ? measures[i][j] : 0);
            positive_measure_count[j] += (measures[i][j] > 0 ? 1 : 0);
        }
    }

    for(int i =0; i < day_masure_count; ++i){
        output[i] /= (positive_measure_count[i] > 0 ? positive_measure_count[i] : 1);
    }

    return output;
}

vector<float> GetRandomVector(int size) {
    static mt19937 engine;
    uniform_real_distribution<float> d(-100, 100);

    vector<float> res(size);
    for (int i = 0; i < size; ++i) {
        res[i] = d(engine);
    }

    return res;
}

vector<float> GetMinusRandomVector(int size) {
    static mt19937 engine;
    uniform_real_distribution<float> d(-100, -1);

    vector<float> res(size);
    for (int i = 0; i < size; ++i) {
        res[i] = d(engine);
    }

    return res;
}

int main() {
    vector<vector<float>> data;
    data.reserve(5000);

    for (int i = 0; i < 5000; ++i) {
        data.push_back(GetRandomVector(5000));
    }

    vector<float> avg;
    {
        LOG_DURATION("ComputeAvgTemp"s);
        avg = ComputeAvgTemp(data);
    }

    cout << "Total mean: "s << accumulate(avg.begin(), avg.end(), 0.f) / avg.size() << endl;
    
    vector<vector<float>> empty_data;
    vector<float> empty_avg;
    {
        LOG_DURATION("ComputeEmptyAvgTemp"s);
        empty_avg = ComputeAvgTemp(empty_data);
    }

    vector<vector<float>> minus_data;
    minus_data.reserve(5000);

    for (int i = 0; i < 5000; ++i) {
        minus_data.push_back(GetMinusRandomVector(5000));
    }

    vector<float> minus_avg;
    {
        LOG_DURATION("ComputeAvgTemp"s);
        minus_avg = ComputeAvgTemp(minus_data);
    }

    cout << "Total mean: "s << accumulate(minus_avg.begin(), minus_avg.end(), 0.f) / minus_avg.size() << endl;

}