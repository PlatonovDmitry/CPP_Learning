#include <algorithm>
#include <iostream>
#include <vector>
#include <map>
#include <numeric>

using namespace std;

template <typename It>
void PrintRange(It range_begin, It range_end) {
    for (auto it = range_begin; it != range_end; ++it) {
        cout << *it << " "s;
    }
    cout << endl;
}

template <typename Type>
struct PairWithMin{
    Type Element;
    Type MinElement;
};

template <typename Type>
ostream& operator << (ostream& out, const PairWithMin<Type>& pair_to_print){
    out << pair_to_print.Element;
    return out;
}

template <typename Type>
class Stack {
public:
    void Push(const Type& element) {
        elements_.push_back(element);
    }
    void Pop() {
        elements_.resize(elements_.size()-1);
    }
    const Type& Peek() const {
        return elements_[elements_.size() - 1];
    }
    Type& Peek() {
        return elements_[elements_.size() - 1];
    }
    void Print() const {
        PrintRange(elements_.begin(), elements_.end());
    }
    uint64_t Size() const {
        return elements_.size();
    }
    bool IsEmpty() const {
        return elements_.empty();
    }

private:
    vector<Type> elements_;
};

template <typename Type>
class StackMin {
public:
    void Push(const Type& element) {
        PairWithMin<Type> new_element = {element, element};

        if(elements_.Size() > 0 && element > elements_.Peek().MinElement){
            new_element.MinElement = elements_.Peek().MinElement;
        }

        elements_.Push(new_element);
    }
    void Pop() {
        elements_.Pop();
    }
    const Type& Peek() const {
        return elements_.Peek();
    }
    Type& Peek() {
        return elements_.Peek();
    }
    void Print() const {
        // работу этого метода мы проверять не будем,
        // но если он вам нужен, то можете его реализовать
        elements_.Print();
    }
    uint64_t Size() const {
        return elements_.size();
    }
    bool IsEmpty() const {
        return elements_.IsEmpty();
    }
    const Type& PeekMin() const {
        return elements_.Peek().MinElement;
    }
    Type& PeekMin() {
        return elements_.Peek().MinElement;
    }
private:
    Stack<PairWithMin<Type>> elements_;
};

int main() {
    StackMin<int> stack;
    vector<int> values(5);
    // заполняем вектор для тестирования нашего стека
    iota(values.begin(), values.end(), 1);
    // перемешиваем значения
    random_shuffle(values.begin(), values.end());
    // заполняем стек
    for (int i = 0; i < 5; ++i) {
        stack.Push(values[i]);
    }
    // печатаем стек и его минимум, постепенно убирая из стека элементы
    while (!stack.IsEmpty()) {
        stack.Print();
        cout << "Минимум = "s << stack.PeekMin() << endl;
        stack.Pop();
    }
}