#include "libstat.h"

#include <iostream>
using namespace std;

int main() {
    using namespace statistics;
    
    tests::AggregSum();
    tests::AggregMax();
    tests::AggregMean();
    tests::AggregStandardDeviation();
    tests::AggregMode();
    tests::AggregPrinter();

    cout << "Test passed!"sv << endl;
}