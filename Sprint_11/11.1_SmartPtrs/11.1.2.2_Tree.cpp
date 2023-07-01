#include <cassert>
#include <iostream>
#include <memory>

template <typename T>
struct TreeNode;

template <typename T>
using TreeNodePtr = std::unique_ptr<TreeNode<T>>;

template <typename T>
struct TreeNode {
    // Используйте TreeNodePtr<T> вместо сырых указателей
    // Примите умные указатели по rvalue-ссылке
    TreeNode(T val, TreeNodePtr<T>&& left, TreeNodePtr<T>&& right)
        : value(std::move(val))
        , left(std::move(left)) 
        , right(std::move(right)) {
            
    }

    T value;
    TreeNodePtr<T> left;  // Замените TreeNode* на TreeNodePtr<T>
    TreeNodePtr<T> right; // Замените TreeNode* на TreeNodePtr<T>
    TreeNode* parent = nullptr;
};

template <typename T>
bool CheckTreeProperty(const TreeNode<T>* node, const T* min, const T* max){
    bool check_node = min == nullptr ? true : node->value > *min;
    if(!check_node){
        return false;
    }
    check_node = max == nullptr ? true : node->value < *max;
    if(!check_node){
        return false;
    }

    bool check_left = node->left == nullptr ? true : CheckTreeProperty(node->left.get(), min, &node->value);
    if(!check_left){
        return false;
    }

    bool check_right = node->right == nullptr ? true : CheckTreeProperty(node->right.get(), &node->value, max);
    if(!check_right){
        return false;
    }

    return true;
}

template <typename T>
bool CheckTreeProperty(const TreeNode<T>* node) noexcept {
    return CheckTreeProperty<T>(node, nullptr, nullptr);
}

template <typename T>
TreeNode<T>* begin(TreeNode<T>* node) noexcept {
    while (node->left != nullptr){
        node = node->left.get();
    }

    return node;
}

template <typename T>
TreeNode<T>* next(TreeNode<T>* node) noexcept {
    if(node->right != nullptr){
        return begin(node->right.get());
    }

    while (node->parent != nullptr)
    {
        if(node->parent ->left.get() == node){
            return node->parent;
        } 

        node = node->parent;
    }
    
    return nullptr;
}

// функция создаёт новый узел с заданным значением и потомками
TreeNodePtr<int> N(int val, TreeNodePtr<int>&& left = {}, TreeNodePtr<int>&& right = {}) {
    auto result = std::make_unique<TreeNode<int>>(val, std::move(left), std::move(right));

    if(result->left != nullptr){
        result->left->parent = result.get();
    }

    if(result->right != nullptr){
        result->right->parent = result.get();
    }

    return result;
}

int main() {
    using namespace std;
    using T = TreeNode<int>;
    auto root1 = N(6, N(4, N(3), N(5)), N(7));
    assert(CheckTreeProperty(root1.get()));

    T* iter = begin(root1.get());
    while (iter) {
        cout << iter->value << " "s;
        iter = next(iter);
    }
    cout << endl;

    auto root2 = N(6, N(4, N(3), N(5)), N(7, N(8)));
    assert(!CheckTreeProperty(root2.get()));

    // Функция DeleteTree не нужна. Узлы дерева будут рекурсивно удалены
    // благодаря деструкторам unique_ptr
}