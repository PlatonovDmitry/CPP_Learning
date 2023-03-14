#include <iostream>
#include <stdexcept>
#include <vector>

using namespace std;

class Tower {
public:
    // конструктор и метод SetDisks нужны, чтобы правильно создать башни
    Tower(int disks_num) {
        FillTower(disks_num);
    }

    void Print() const{
        for(const auto& cur_disk : disks_){
            cout << cur_disk << ' ';
        }
        cout << endl;
    }

    int GetDisksNum() const {
        return disks_.size();
    }

    void SetDisks(int disks_num) {
        FillTower(disks_num);
    }

    // добавляем диск на верх собственной башни
    // обратите внимание на исключение, которое выбрасывается этим методом
    void AddToTop(int disk) {
        int top_disk_num = disks_.size() - 1;
        if (0 != disks_.size() && disk >= disks_[top_disk_num]) {
            throw invalid_argument("Невозможно поместить большой диск на маленький");
        } else {
            disks_.push_back(disk);
        }
    }

    // Снять диск
    int GetDisk(){
        if (disks_.empty()) {
            throw invalid_argument("Кончились диски");
        }

        auto last_num = disks_.end() - 1;
        int output = *last_num;
        disks_.erase(last_num);
        return output;
    }

private:
    vector<int> disks_;

    // используем приватный метод FillTower, чтобы избежать дубликации кода
    void FillTower(int disks_num) {
        for (int i = disks_num; i > 0; i--) {
            disks_.push_back(i);
        }
    }
};

void MoveTover(Tower& from, Tower& to, Tower& middle, int size) {
    if(size > 1){
        MoveTover(from, middle, to, size -1);
    }
       
    int bottom_disk = from.GetDisk();
    to.AddToTop(bottom_disk);

    if(size > 1){
        MoveTover(middle, to, from, size -1);
    }
}

void SolveHanoi(vector<Tower>& towers) {
    int disks_num = towers[0].GetDisksNum();
    MoveTover(towers[0], towers[2], towers[1], disks_num);
}

int main() {
    int towers_num = 3;
    int disks_num = 3;
    vector<Tower> towers;
    // добавим в вектор три пустые башни
    for (int i = 0; i < towers_num; ++i) {
        towers.push_back(0);
    }
    // добавим на первую башню три кольца
    towers[0].SetDisks(disks_num);
    SolveHanoi(towers);
}