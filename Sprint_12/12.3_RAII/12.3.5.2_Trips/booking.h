#pragma once

namespace raii{

    template <typename Provider>
    class Booking {
    private:
        using BookingId = typename Provider::BookingId;

        Provider* provider_;
        BookingId id_;

    public:
        Booking(Provider* p, BookingId id):
            provider_(p),
            id_(id)
        {

        }
        Booking(const Booking&) = default;
        Booking(Booking&& other) = default;
        Booking& operator=(const Booking&) = default;
        Booking& operator=(Booking&& other) = default;
        ~Booking(){
            provider_->CancelOrComplete(*this);
        }

        // Эта функция не требуется в тестах, но в реальной программе она может быть нужна
        BookingId GetId() const{
            return id_;
        }
    };

}