#include <algorithm>
#include <cstdlib>
#include <deque>
#include <vector>

#include "../../Sprint 6/6.1_profile/log_duration.h"

using namespace std;

template <class container>
void AppendRandom(container& v, int n) {
    for (int i = 0; i < n; ++i) {
        v.push_back(rand() % 2);
    }
}

int main(){
    static const int N = 1000000;
    vector<int> vec;
    deque<int> deq;

    {
        LOG_DURATION("vector");    
        AppendRandom(vec, N);
    }
    {
        LOG_DURATION("deque");
        AppendRandom(deq, N);
    }
    {
        LOG_DURATION("vector sort");    
        sort(vec.begin(), vec.end());
    }
    {
        LOG_DURATION("deque sort");
        sort(deq.begin(), deq.end());
    }
}