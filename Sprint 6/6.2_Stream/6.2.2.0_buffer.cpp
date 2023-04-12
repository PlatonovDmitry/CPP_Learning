#include <fstream>
#include <string>

#include "../6.1_profile/log_duration.h"

using namespace std;

int main() {
    LOG_DURATION("//n");
    ofstream out_file("ballad.txt"s);
    for (int i = 0; i < 5000; ++i) {
        out_file << "С любимыми не расставайтесь/n"s;
    }
}