#pragma once

#include <array>
#include <stdexcept>

template <typename T, size_t N>
class StackVector {
public:
    explicit StackVector(size_t a_size = 0)
    : array_(std::array<T, N>{}){
        if(a_size > array_.max_size()){
            throw std::invalid_argument("Превышен максимальный размер массива");
        }

        size_ = a_size;
    }

    T& operator[](size_t index){
        if(index >= size_){
            throw std::invalid_argument("Превышен максимальный размер массива");
        }

        return array_[index];
    }

    const T& operator[](size_t index) const{
        if(index >= size_){
            throw std::invalid_argument("Превышен максимальный размер массива");
        }

        return array_[index];
    }

    T* begin(){
        return array_.begin();
    }
    T* end(){
        auto output = array_.begin();
        for(size_t i = 0; i<size_; ++i){
            ++output;
        }
        return output;
    }
    const T* begin() const{
        return array_.begin();
    }
    const T* end() const{
        auto output = array_.begin();
        for(size_t i = 0; i<size_; ++i){
            ++output;
        }
        return output;
    }

    size_t Size() const{
        return size_;
    }
    size_t Capacity() const{
        return array_.max_size();
    }

    void PushBack(const T& value){
        if(array_.max_size() == size_){
            throw std::overflow_error("Переполнение вектора");
        }
        array_[size_] = value;
        ++size_;
    }
    T PopBack(){
        if(size_ == 0){
            throw std::underflow_error("Вектор пуст");
        }
        --size_;
        return array_[size_];
    }

private:
    size_t size_;
    std::array<T, N> array_;
};
