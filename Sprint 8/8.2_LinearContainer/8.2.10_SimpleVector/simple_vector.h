#pragma once

#include <algorithm>
#include <cassert>
#include <initializer_list>
#include <stdexcept>

#include "array_ptr.h"

struct ReserveProxyObj
{
    explicit ReserveProxyObj(size_t size)
        : Capacity(size)
    {
    }

    size_t Capacity = 0;
};

ReserveProxyObj Reserve(size_t capacity_to_reserve) {
    return ReserveProxyObj(capacity_to_reserve);
}

template <typename Type>
class SimpleVector
{
public:
    using Iterator = Type *;
    using ConstIterator = const Type *;

    SimpleVector() noexcept = default;

    // Создаёт вектор из size элементов, инициализированных значением по умолчанию
    explicit SimpleVector(size_t size)
    :internal_array_({size}), 
    capacity_(size),
    size_(size){
        std::generate(internal_array_.Get(), &internal_array_[size], [](){return Type{};});
    }

    explicit SimpleVector(ReserveProxyObj size)
    :internal_array_({size.Capacity}),
    capacity_(size.Capacity),
    size_(0){
        
    }

    // Создаёт вектор из size элементов, инициализированных значением value
    SimpleVector(size_t size, const Type &value)
    {
        ArrayPtr<Type> temp(size);
        std::fill(temp.Get(), &temp[size], value);

        internal_array_.swap(temp);
        size_ = size;
        capacity_ = size_;
    }

    // Создаёт вектор из std::initializer_list
    SimpleVector(std::initializer_list<Type> init)
    {
        ArrayPtr<Type> temp(init.size());
        std::copy(init.begin(), init.end(), temp.Get());

        internal_array_.swap(temp);
        size_ = init.size();
        capacity_ = size_;
    }

    SimpleVector(const SimpleVector &other)
    {
        SimpleVector temp(other.size_);
        std::copy(other.begin(), other.end(), temp.begin());
        swap(temp);
    }

    SimpleVector &operator=(const SimpleVector &rhs)
    {
        auto temp = SimpleVector(rhs);

        swap(temp);
        return *this;
    }

    void Reserve(size_t new_capacity)
    {
        if (new_capacity > capacity_)
        {
            ArrayPtr<Type> temp(new_capacity);
            std::copy(internal_array_.Get(), &internal_array_[size_], temp.Get());

            internal_array_.swap(temp);
            capacity_ = new_capacity;
        }
    }

    // Возвращает количество элементов в массиве
    size_t GetSize() const noexcept
    {
        return size_;
    }

    // Возвращает вместимость массива
    size_t GetCapacity() const noexcept
    {
        return capacity_;
    }

    // Сообщает, пустой ли массив
    bool IsEmpty() const noexcept
    {
        return size_ == 0;
    }

    // Возвращает ссылку на элемент с индексом index
    Type &operator[](size_t index) noexcept
    {
        return internal_array_[index];
    }

    // Возвращает константную ссылку на элемент с индексом index
    const Type &operator[](size_t index) const noexcept
    {
        return internal_array_[index];
    }

    // Возвращает константную ссылку на элемент с индексом index
    // Выбрасывает исключение std::out_of_range, если index >= size
    Type &At(size_t index)
    {
        if (index >= size_)
        {
            throw std::out_of_range("Индекс за пределами диапазона");
        }

        return internal_array_[index];
    }

    // Возвращает константную ссылку на элемент с индексом index
    // Выбрасывает исключение std::out_of_range, если index >= size
    const Type &At(size_t index) const
    {
        if (index >= size_)
        {
            throw std::out_of_range("Индекс за пределами диапазона");
        }

        return internal_array_[index];
    }

    // Обнуляет размер массива, не изменяя его вместимость
    void Clear() noexcept
    {
        size_ = 0;
    }

    // Изменяет размер массива.
    // При увеличении размера новые элементы получают значение по умолчанию для типа Type
    void Resize(size_t new_size)
    {
        if (new_size > size_)
        {
            auto max_size = std::max(new_size, 2 * size_);
            ArrayPtr<Type> temp(max_size);
            std::copy(internal_array_.Get(), &internal_array_[size_], temp.Get());
            std::fill(&temp[size_], &temp[new_size], Type{});

            internal_array_.swap(temp);
            capacity_ = max_size;
        }

        size_ = new_size;
    }

    // Добавляет элемент в конец вектора
    // При нехватке места увеличивает вдвое вместимость вектора
    void PushBack(const Type &item)
    {
        if (size_ == capacity_) {
            size_t new_size = size_ == 0 ? 1 : 2 * size_;
            ArrayPtr<Type> temp(new_size);
            std::copy(
                std::make_move_iterator(internal_array_.Get()), 
                std::make_move_iterator(&internal_array_[size_]), 
                temp.Get());
                
            internal_array_.swap(temp);
            capacity_ = new_size;
        }
        
        internal_array_[size_] = item;
        ++size_;
    }

    // Вставляет значение value в позицию pos.
    // Возвращает итератор на вставленное значение
    // Если перед вставкой значения вектор был заполнен полностью,
    // вместимость вектора должна увеличиться вдвое, а для вектора вместимостью 0 стать равной 1
    Iterator Insert(ConstIterator pos, const Type &value)
    {
        Iterator output;
        if (size_ == capacity_)
        {
            size_t new_size;
            if (size_ == 0)
            {
                new_size = 1;
            }
            else
            {
                new_size = 2 * size_;
            }

            ArrayPtr<Type> temp(new_size);
            std::copy(internal_array_.Get(), Iterator(pos), temp.Get());
            output = std::copy_backward(Iterator(pos), &internal_array_[size_], &temp[size_ + 1]);
            internal_array_.swap(temp);
            capacity_ = new_size;
        }
        else
        {
            output = std::copy_backward(Iterator(pos), &internal_array_[size_], &internal_array_[size_ + 1]);
        }

        --output;
        *output = value;

        ++size_;
        return output;
    }

    // "Удаляет" последний элемент вектора. Вектор не должен быть пустым
    void PopBack() noexcept
    {
        assert(size_ > 0);
        --size_;
    }

    // Удаляет элемент вектора в указанной позиции
    Iterator Erase(ConstIterator pos)
    {
        assert(size_ > 0);

        auto next_it = Iterator(pos);
        auto output_it = Iterator(pos);
        --output_it;
        std::rotate(Iterator(pos), ++next_it, &internal_array_[size_]);
        --size_;
        return ++output_it;
    }

    // Обменивает значение с другим вектором
    void swap(SimpleVector &other) noexcept
    {
        other.internal_array_.swap(internal_array_);
        std::swap(size_, other.size_);
        std::swap(capacity_, other.capacity_);
    }

    // Возвращает итератор на начало массива
    // Для пустого массива может быть равен (или не равен) nullptr
    Iterator begin() noexcept
    {
        return internal_array_.Get();
    }

    // Возвращает итератор на элемент, следующий за последним
    // Для пустого массива может быть равен (или не равен) nullptr
    Iterator end() noexcept
    {
        return &internal_array_[size_];
    }

    // Возвращает константный итератор на начало массива
    // Для пустого массива может быть равен (или не равен) nullptr
    ConstIterator begin() const noexcept
    {
        return internal_array_.Get();
    }

    // Возвращает итератор на элемент, следующий за последним
    // Для пустого массива может быть равен (или не равен) nullptr
    ConstIterator end() const noexcept
    {
        return &internal_array_[size_];
    }

    // Возвращает константный итератор на начало массива
    // Для пустого массива может быть равен (или не равен) nullptr
    ConstIterator cbegin() const noexcept
    {
        return internal_array_.Get();
    }

    // Возвращает итератор на элемент, следующий за последним
    // Для пустого массива может быть равен (или не равен) nullptr
    ConstIterator cend() const noexcept
    {
        return &internal_array_[size_];
    }

private:
    ArrayPtr<Type> internal_array_;

    size_t size_ = 0;
    size_t capacity_ = 0;
};

template <typename Type>
inline bool operator==(const SimpleVector<Type> &lhs, const SimpleVector<Type> &rhs)
{
    return std::equal(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
}

template <typename Type>
inline bool operator!=(const SimpleVector<Type> &lhs, const SimpleVector<Type> &rhs)
{
    return !(lhs == rhs);
}

template <typename Type>
inline bool operator<(const SimpleVector<Type> &lhs, const SimpleVector<Type> &rhs)
{
    return std::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
}

template <typename Type>
inline bool operator<=(const SimpleVector<Type> &lhs, const SimpleVector<Type> &rhs)
{
    return !(rhs < lhs);
}

template <typename Type>
inline bool operator>(const SimpleVector<Type> &lhs, const SimpleVector<Type> &rhs)
{
    return !(lhs <= rhs);
}

template <typename Type>
inline bool operator>=(const SimpleVector<Type> &lhs, const SimpleVector<Type> &rhs)
{
    return !(lhs < rhs);
}