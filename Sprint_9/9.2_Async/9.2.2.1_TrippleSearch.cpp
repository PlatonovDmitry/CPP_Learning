#include <algorithm>
#include <execution>
#include <future>
#include <iostream>

using namespace std;

template <typename RandomAccessIterator, typename Value>
RandomAccessIterator LowerBound(const execution::sequenced_policy &,
                                RandomAccessIterator range_begin, RandomAccessIterator range_end,
                                const Value &value)
{
    auto left_bound = range_begin;
    auto right_bound = range_end;
    while (left_bound + 1 < right_bound)
    {
        const auto middle = left_bound + (right_bound - left_bound) / 2;
        if (*middle < value)
        {
            left_bound = middle;
        }
        else
        {
            right_bound = middle;
        }
    }
    if (left_bound == range_begin && !(*left_bound < value))
    {
        return left_bound;
    }
    else
    {
        return right_bound;
    }
}

template <typename RandomAccessIterator, typename Value>
RandomAccessIterator LowerBound(RandomAccessIterator range_begin, RandomAccessIterator range_end,
                                const Value &value)
{
    return LowerBound(execution::seq, range_begin, range_end, value);
}

template <typename RandomAccessIterator, typename Value>
RandomAccessIterator LowerBound(const execution::parallel_policy &, RandomAccessIterator range_begin,
                                RandomAccessIterator range_end, const Value &value)
{
    auto left_bound = range_begin;
    auto right_bound = range_end;
    while (left_bound + 1 < right_bound)
    {
        auto step = (right_bound - left_bound) / 3;
        const auto left_middle = left_bound + (step == 0 ? 1 : step);
        const auto right_middle = right_bound - step;

        future<bool> bound_future = async([&right_middle, &value]
                                          { return *right_middle < value; });

        if (*left_middle < value)
        {
            if (bound_future.get())
            {
                left_bound = right_middle;
            }
            else
            {
                left_bound = left_middle;
                right_bound = right_middle;
            }
        }
        else
        {
            right_bound = left_middle;
        }
    }

    if (left_bound == range_begin && !(*left_bound < value))
    {
        return left_bound;
    }
    else
    {
        return right_bound;
    }
}

int main()
{
    /*
    const vector<int> vals = {0, 1, 2, 3, 4, 5};
    int val = 1;
    cout << "Request [" << val << "] → position "
         << LowerBound(execution::par, vals.begin(), vals.end(), val) - vals.begin()
         << endl;
    */


    const vector<string> strings = {"cat", "dog", "dog", "horse"};

    const vector<string> requests = {"bear", "cat", "deer", "dog", "dogs", "horses"};

    // последовательные версии
    cout << "Request [" << requests[0] << "] → position "
         << LowerBound(strings.begin(), strings.end(), requests[0]) - strings.begin() << endl;
    cout << "Request [" << requests[1] << "] → position "
         << LowerBound(execution::seq, strings.begin(), strings.end(), requests[1])
            - strings.begin()
         << endl;
    cout << "Request [" << requests[2] << "] → position "
         << LowerBound(execution::seq, strings.begin(), strings.end(), requests[2])
            - strings.begin()
         << endl;

    // параллельные
    cout << "Request [" << requests[3] << "] → position "
         << LowerBound(execution::par, strings.begin(), strings.end(), requests[3])
            - strings.begin()
         << endl;
    cout << "Request [" << requests[4] << "] → position "
         << LowerBound(execution::par, strings.begin(), strings.end(), requests[4])
            - strings.begin()
         << endl;
    cout << "Request [" << requests[5] << "] → position "
         << LowerBound(execution::par, strings.begin(), strings.end(), requests[5])
            - strings.begin()
         << endl;
    
}