#pragma once
#include "flight_provider.h"
#include "hotel_provider.h"

#include <string>
#include <vector>

class Trip {
public:
    std::vector<FlightProvider::BookingId> flights;
    std::vector<HotelProvider::BookingId> hotels;

    Trip(HotelProvider& hp, FlightProvider& fp):
        hp_(hp), fp_(fp)
    {

    }
    Trip(const Trip&) = delete;
    Trip(Trip&& other):
        hp_(other.hp_),
        fp_(other.fp_)
    {
        flights = std::move(other.flights);
        hotels = std::move(other.hotels);
    }
    Trip& operator=(const Trip&) = delete;
    Trip& operator=(Trip&& other){
        if(this != &other){
            flights = std::move(other.flights);
            hotels = std::move(other.hotels);
            std::swap(hp_, other.hp_);
            std::swap(fp_, other.fp_);
        }

        return *this;
    }

    void Cancel(){
        for(auto cur_flight : flights){
            fp_.Cancel(cur_flight);
        }

        for(auto cur_hotel : hotels){
            hp_.Cancel(cur_hotel);
        }
    }

    ~Trip(){
        Cancel();
    }

private:
    HotelProvider& hp_;
    FlightProvider& fp_;
};

class TripManager {
public:
    using BookingId = std::string;
    struct BookingData {
        std::string city_from;
        std::string city_to;
        std::string date_from;
        std::string date_to;
    };

    Trip Book(const BookingData& data) {
        Trip trip(hotel_provider_, flight_provider_);

        {
            FlightProvider::BookingData flight_booking_data{data.city_from, data.city_to, data.date_from};
            trip.flights.push_back(flight_provider_.Book(flight_booking_data));
        }
        {
            HotelProvider::BookingData hotel_booking_data{data.city_to, data.date_from, data.date_to};
            trip.hotels.push_back(hotel_provider_.Book(hotel_booking_data));
        }
        {
            FlightProvider::BookingData flight_booking_data{data.city_to, data.city_from, data.date_to};
            trip.flights.push_back(flight_provider_.Book(flight_booking_data));
        }
        
        
        return trip;
    }

    void Cancel(Trip& trip) {
        trip.Cancel();
    }

private:
    HotelProvider hotel_provider_;
    FlightProvider flight_provider_;
};