#include <cmath>
#include <iostream>

using namespace std;

class Greeter{
public:
    Greeter(const string& gr)
    : gr_(gr)
    {
        cout << "+"s << gr_ << endl;
    }

    ~Greeter(){
        cout << "-"s << gr_ << endl;
    }

    string Gr() const{
        return gr_;
    }

private:
    string gr_;    
};

class TestGreeter{
public:
    TestGreeter(const string& gr1, const string& gr2)
    : gr2_(Greeter{gr2}), gr1_(Greeter{gr1}) 
    {
        cout << "+"s << gr1_.Gr() << "/"s << gr2_.Gr() << endl; 
    }

    ~TestGreeter(){
        cout << "-"s << gr1_.Gr() << "/"s << gr2_.Gr() << endl; 
    }
private:
    Greeter gr2_;
    Greeter gr1_;
};

Greeter glGr{"Global"};
TestGreeter glTestGr{"GlobalTest1", "GlobalTest2"};

int main() {
    Greeter localGr("Local");
    TestGreeter glTestGr{"LocalTest1", "LocalTest2"};
}