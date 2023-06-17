#include <cassert>
#include <iostream>
#include <vector>

using namespace std;

class MoneyBox {
public:
    explicit MoneyBox(vector<int64_t> nominals)
        : nominals_(move(nominals))
        , counts_(nominals_.size()) {
    }

    const vector<int>& GetCounts() const {
        return counts_;
    }

    void PushCoin(int64_t value) {
        for(int i = 0; i < nominals_.size(); ++i){
            if(nominals_[i]==value){
                ++counts_[i];
                break;
            }
        }
    }

    void PrintCoins(ostream& out) const {
        for (int i = 0; i < nominals_.size(); ++i) {
            if (counts_[i] > 0) {
                out << nominals_[i] << ": "s << counts_[i] << endl;
            }
        }
    }

private:
    const vector<int64_t> nominals_;
    vector<int> counts_;
};

ostream& operator<<(ostream& out, const MoneyBox& cash) {
    cash.PrintCoins(out);
    return out;
}

int main() {
    MoneyBox cash({1, 500, 10000});
    cash.PushCoin(500);
    cash.PushCoin(500);
    cash.PushCoin(10000);
    assert((cash.GetCounts() == vector<int>{0,2,1}));
    cout << cash << endl;
} 