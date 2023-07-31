#include <iostream>
#include <string>
#include <unordered_map>
#include <queue>

using namespace std;

struct BookOrder{
    string hotel;
    int client_id;
    int room_count;
    int64_t time;
};

class HotelManager {
public:
    void Book(BookOrder order){
        hotel_orders_.push(order);
        hotel_rooms_[order.hotel] += order.room_count;
        hotel_quests_[order.hotel][order.client_id] += order.room_count;

        while (order.time - hotel_orders_.front().time >= 86400)
        {
            auto escape_order = hotel_orders_.front();
            hotel_rooms_[escape_order.hotel] -= escape_order.room_count;
            hotel_quests_[escape_order.hotel][escape_order.client_id] -= escape_order.room_count;
            if(hotel_quests_[escape_order.hotel][escape_order.client_id] == 0){
                hotel_quests_[escape_order.hotel].erase(escape_order.client_id);
            }
            hotel_orders_.pop();
        }
    }
    int ComputeClientCount(string hotel) const{
        if(hotel_quests_.count(hotel)){
            return hotel_quests_.at(hotel).size();
        }
        return 0;
    }
    int ComputeRoomCount(string hotel) const{
        if(hotel_rooms_.count(hotel)){
            return hotel_rooms_.at(hotel);
        }
        return 0;
    }

private:
    queue<BookOrder> hotel_orders_;
    unordered_map<string, int> hotel_rooms_;
    unordered_map<string, unordered_map<int, int>> hotel_quests_;
};

int main() {
    HotelManager manager;

    int query_count;
    cin >> query_count;

    for (int query_id = 0; query_id < query_count; ++query_id) {
        string query_type;
        cin >> query_type;

        if (query_type == "BOOK") {
            int64_t time;
            cin >> time;
            string hotel_name;
            cin >> hotel_name;
            int client_id, room_count;
            cin >> client_id >> room_count;
            manager.Book({hotel_name, client_id, room_count, time});
        } else {
            string hotel_name;
            cin >> hotel_name;
            if (query_type == "CLIENTS") {
                cout << manager.ComputeClientCount(hotel_name) << "\n";
            } else if (query_type == "ROOMS") {
                cout << manager.ComputeRoomCount(hotel_name) << "\n";
            }
        }
    }

    return 0;
}