#pragma once

#include <algorithm>
#include <cassert>
#include <cstdlib>
#include <memory>
#include <new>
#include <type_traits>
#include <utility>

template <typename T>
class RawMemory {
public:
    RawMemory() = default;

    explicit RawMemory(size_t capacity)
        : buffer_(Allocate(capacity))
        , capacity_(capacity) {
    }

    RawMemory(const RawMemory&) = delete;
    RawMemory& operator=(const RawMemory& rhs) = delete;
    RawMemory(RawMemory&& other) noexcept { 
        Swap(other);
    }
    RawMemory& operator=(RawMemory&& rhs) noexcept { 
        Swap(rhs);
    }

    ~RawMemory() {
        Deallocate(buffer_);
    }

    T* operator+(size_t offset) noexcept {
        // Разрешается получать адрес ячейки памяти, следующей за последним элементом массива
        assert(offset <= capacity_);
        return buffer_ + offset;
    }

    const T* operator+(size_t offset) const noexcept {
        return const_cast<RawMemory&>(*this) + offset;
    }

    const T& operator[](size_t index) const noexcept {
        return const_cast<RawMemory&>(*this)[index];
    }

    T& operator[](size_t index) noexcept {
        assert(index < capacity_);
        return buffer_[index];
    }

    void Swap(RawMemory& other) noexcept {
        std::swap(buffer_, other.buffer_);
        std::swap(capacity_, other.capacity_);
    }

    const T* GetAddress() const noexcept {
        return buffer_;
    }

    T* GetAddress() noexcept {
        return buffer_;
    }

    size_t Capacity() const {
        return capacity_;
    }

private:
    // Выделяет сырую память под n элементов и возвращает указатель на неё
    static T* Allocate(size_t n) {
        return n != 0 ? static_cast<T*>(operator new(n * sizeof(T))) : nullptr;
    }

    // Освобождает сырую память, выделенную ранее по адресу buf при помощи Allocate
    static void Deallocate(T* buf) noexcept {
        operator delete(buf);
    }

    T* buffer_ = nullptr;
    size_t capacity_ = 0;
};

template <typename T>
class Vector {
public:
    using iterator = T*;
    using const_iterator = const T*;

    Vector() = default;
    explicit Vector(size_t size)
        : data_({size})
        , size_(size)  //
    {
        std::uninitialized_value_construct_n(data_.GetAddress(), size);
    }

    Vector(const Vector& other)
        : data_({other.size_})
        , size_(other.size_)  //
    {
        std::uninitialized_move_n(other.data_.GetAddress(), other.size_, data_.GetAddress());
    }
    
    Vector(Vector&& other) noexcept{
        Swap(other);
    }

    Vector& operator=(const Vector& rhs){
        if (this != &rhs) {
            if (rhs.size_ > data_.Capacity()) {
                Vector rhs_copy(rhs);
                Swap(rhs_copy);
            } else {
                if(rhs.size_ > size_){
                    for(size_t i=0; i<size_; ++i){
                        data_[i] = rhs.data_[i];
                    }
                    std::uninitialized_copy_n(rhs.data_ + size_, rhs.size_ - size_, data_ + size_);
                } else{
                    for(size_t i=0; i<rhs.size_; ++i){
                        data_[i] = rhs.data_[i];
                    }
                    std::destroy_n(data_ + rhs.size_, size_ - rhs.size_);
                }

                size_ = rhs.size_;
            }
        }
        return *this;
    }
    Vector& operator=(Vector&& rhs) noexcept{
        Swap(rhs);
        return *this;
    }

    iterator begin() noexcept{
        return data_.GetAddress();
    }
    iterator end() noexcept{
        return data_ + size_;
    }
    const_iterator begin() const noexcept{
        return data_.GetAddress();
    }
    const_iterator end() const noexcept{
        return data_ + size_;
    }
    const_iterator cbegin() const noexcept{
        return data_.GetAddress();
    }
    const_iterator cend() const noexcept{
        return data_ + size_;
    }

    void Swap(Vector& other) noexcept{
        std::swap(size_, other.size_);
        data_.Swap(other.data_);
    }

    void Resize(size_t new_size){
        if(new_size < size_){
            std::destroy_n(data_ + new_size, size_ - new_size);
        } else{
            if(data_.Capacity() < new_size){
                Reserve(new_size);
            }

            std::uninitialized_value_construct_n(data_ + size_, new_size - size_);
        }

        size_ = new_size;
    }
    void PushBack(const T& value){
        EmplaceBack(value);
    }
    void PushBack(T&& value){
        EmplaceBack(std::move(value));
    }
    void PopBack() {/* noexcept */
        --size_;
        std::destroy_at(data_+size_);
    }

    template <typename... Args>
    T& EmplaceBack(Args&&... args){
        if(size_ == data_.Capacity()){
            RawMemory<T> new_data(data_.Capacity() == 0 ? 1 : 2 * data_.Capacity());
            new (new_data + size_) T(std::forward<decltype(args)>(args)...);
            if constexpr (std::is_nothrow_move_constructible_v<T> || !std::is_copy_constructible_v<T>) {
                std::uninitialized_move_n(data_.GetAddress(), size_, new_data.GetAddress());
            } else {
                std::uninitialized_copy_n(data_.GetAddress(), size_, new_data.GetAddress());
            }
            std::destroy_n(data_.GetAddress(), size_);
            data_.Swap(new_data);
        } else{
            new (data_ + size_) T(std::forward<decltype(args)>(args)...);
        }

        ++size_;
        return data_[size_ -1];
    }

    template <typename... Args>
    iterator Emplace(const_iterator pos, Args&&... args){
        size_t new_place = pos - begin();

        if(new_place == size_){
            EmplaceBack(std::forward<Args>(args)...);
            return begin() + new_place;
        }

        if(size_ == data_.Capacity()){
            RawMemory<T> new_data(data_.Capacity() == 0 ? 1 : 2 * data_.Capacity());
            new (new_data + new_place) T(std::forward<Args>(args)...);
            if constexpr (std::is_nothrow_move_constructible_v<T> || !std::is_copy_constructible_v<T>) {
                std::uninitialized_move_n(data_.GetAddress(), new_place, new_data.GetAddress());
                std::uninitialized_move_n(data_ + new_place, size_ - new_place, new_data + new_place + 1);
            } else {
                std::uninitialized_copy_n(data_.GetAddress(), new_place, new_data.GetAddress());
                std::uninitialized_copy_n(data_ + new_place, size_ - new_place, new_data + new_place + 1);
            }

            std::destroy_n(data_.GetAddress(), size_);
            data_.Swap(new_data);
        } else {
            auto temp = T(std::forward<Args>(args)...);
            
            new (data_ + size_) T(std::move(data_[size_ -1]));
            std::move_backward(begin() + new_place, end()-1, end());

            data_[new_place] = std::move(temp);
        }

        ++size_;
        return begin() + new_place;
    }
    iterator Erase(const_iterator pos) /*noexcept(std::is_nothrow_move_assignable_v<T>)*/{
        auto new_place = pos - begin();
        std::move(begin() + new_place + 1, end(), begin() + new_place);
        
        Destroy(end()-1);
        --size_;
        return begin() + new_place;
    }
    iterator Insert(const_iterator pos, const T& value){
        return Emplace(pos, value);
    }
    iterator Insert(const_iterator pos, T&& value){
        return Emplace(pos, std::move(value));
    }

    size_t Size() const noexcept {
        return size_;
    }

    size_t Capacity() const noexcept {
        return data_.Capacity();
    }

    const T& operator[](size_t index) const noexcept {
        return const_cast<Vector&>(*this)[index];
    }

    T& operator[](size_t index) noexcept {
        assert(index < size_);
        return data_[index];
    }

    void Reserve(size_t new_capacity) {
        if (new_capacity <= data_.Capacity()) {
            return;
        }

        RawMemory<T> new_data(new_capacity);
        if constexpr (std::is_nothrow_move_constructible_v<T> || !std::is_copy_constructible_v<T>) {
            std::uninitialized_move_n(data_.GetAddress(), size_, new_data.GetAddress());
        } else {
            std::uninitialized_copy_n(data_.GetAddress(), size_, new_data.GetAddress());
        }
        std::destroy_n(data_.GetAddress(), size_);
        data_.Swap(new_data);
    }

    ~Vector() {
        std::destroy_n(data_.GetAddress(), size_);
    }

private:
    RawMemory<T> data_;
    size_t size_ = 0;

    // Создаёт копию объекта elem в сырой памяти по адресу buf
    static void CopyConstruct(T* buf, const T& elem) {
        new (buf) T(elem);
    }

    // Вызывает деструктор объекта по адресу buf
    static void Destroy(T* buf) noexcept {
        buf->~T();
    }
};