#include "log_duration.h"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <random>
#include <set>
#include <string>
#include <unordered_set>
#include <vector>

using namespace std;

class RandomContainer {
public:
    void Insert(int val) {
        values_pool_.insert(val);
    }
    void Remove(int val) {
        values_pool_.erase(val);
    }
    bool Has(int val) const {
        return values_pool_.count(val) > 0;
    }
    int GetRand() const {
        uniform_int_distribution<int> distr(0, values_pool_.size() - 1);
        int rand_index = distr(engine_);
        auto begin = values_pool_.begin();
        for(int i=0; i< rand_index; ++i){
            ++begin;
        }

        return *begin;
    }

private:
    unordered_set<int> values_pool_;
    mutable mt19937 engine_;
};

int main() {
    RandomContainer container;
    int query_num = 0;

    ifstream source("input.txt");
    source >> query_num;
    {
        LOG_DURATION("Requests handling"s);
        for (int query_id = 0; query_id < query_num; query_id++) {
            string query_type;
            source >> query_type;
            if (query_type == "INSERT"s) {
                int value = 0;
                source >> value;
                container.Insert(value);
            } else if (query_type == "REMOVE"s) {
                int value = 0;
                source >> value;
                container.Remove(value);
            } else if (query_type == "HAS"s) {
                int value = 0;
                source >> value;
                if (container.Has(value)) {
                    cout << "true"s << endl;
                } else {
                    cout << "false"s << endl;
                }
            } else if (query_type == "RAND"s) {
                cout << container.GetRand() << endl;
            }
        }
    }
}