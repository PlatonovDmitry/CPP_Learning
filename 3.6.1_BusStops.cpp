#include <cassert>
#include <iostream>
#include <sstream>
#include <map>
#include <string>
#include <vector>

using namespace std;

enum class QueryType {
    NewBus,
    BusesForStop,
    StopsForBus,
    AllBuses,
};

struct Query {
    QueryType type;
    string bus;
    string stop;
    vector<string> stops;
};

istream& operator>>(istream& is, Query& q) {
    string action;
    is >> action;

    if (action == "NEW_BUS"s) {
        q.type = QueryType::NewBus;
        is >> q.bus;
        
        int stop_count;
        is >> stop_count;

        string cur_stop;
        for(int i = 0; i < stop_count; ++i){
            is >> cur_stop;
            q.stops.push_back(cur_stop);
        }
    } else if (action == "BUSES_FOR_STOP"s) {
            q.type = QueryType::BusesForStop;
            is >> q.stop;
    } else if (action == "STOPS_FOR_BUS"s) {
            q.type = QueryType::StopsForBus;
            is >> q.bus;
    } else if (action == "ALL_BUSES"s) {
            q.type = QueryType::AllBuses;
    }

    return is;
}

struct BusesForStopResponse {
    vector<string> buses;
};

ostream& operator<<(ostream& os, const BusesForStopResponse& r) {
    if(r.buses.size() > 0){
        for (const string& cur_bus : r.buses) {
            os << cur_bus << " "s;
        }
    } else{
        os << "No stop"s;
    }

    return os;
}

struct StopsForBusResponse {
    vector<pair<string, vector<string>>> buses_to_stops;
};

ostream& operator<<(ostream& os, const StopsForBusResponse& r) {
    if(r.buses_to_stops.size()>0){
        bool first_line = true;
        for(const auto& [cur_stop, buses] : r.buses_to_stops){
            if(first_line){
                first_line = false;
            }else{
                os << endl;    
            }

            os << "Stop "s << cur_stop << ": "s;
            if(buses.size() > 0){
                for(const string& cur_bus : buses){
                    os << cur_bus << " "s;
                }
            }else{
                os << "no interchange"s;
            }
        }
    } else{
        os << "No bus"s;
    }

    return os;
}

struct AllBusesResponse {
    map<string, vector<string>> stops_to_buses;
};

ostream& operator<<(ostream& os, const AllBusesResponse& r) {
    if(r.stops_to_buses.size()>0){
        bool first_line = true;
        for(const auto& [cur_bus, stops] : r.stops_to_buses){
            if(first_line){
                first_line = false;
            }else{
                os << endl;    
            }
            os << "Bus "s << cur_bus << ": "s;
            for(const string& cur_stop : stops){
                os << cur_stop << " "s;
            }
        }
    } else{
        os << "No buses"s;
    }

    return os;
}

class BusManager {
public:
    void AddBus(const string& bus, const vector<string>& stops) {
        stops_to_buses_[bus] = stops;
        for(const string& cur_stop : stops){
            buses_to_stops_[cur_stop].push_back(bus);
        }
    }

    BusesForStopResponse GetBusesForStop(const string& stop) const {
        BusesForStopResponse output;
        if(buses_to_stops_.count(stop) > 0){
            output.buses = buses_to_stops_.at(stop);
        }

        return output;
    }

    StopsForBusResponse GetStopsForBus(const string& bus) const {
        StopsForBusResponse output;
        if(stops_to_buses_.count(bus) > 0){
            for(const string& cur_stop : stops_to_buses_.at(bus)){
                vector<string> cur_stop_busses;
                for(const string& cur_bus : buses_to_stops_.at(cur_stop)){
                    if(cur_bus!= bus){
                        cur_stop_busses.push_back(cur_bus);
                    }
                }
                output.buses_to_stops.push_back(pair(cur_stop, cur_stop_busses));
            }
            
        }

        return output;
    }

    AllBusesResponse GetAllBuses() const {
        AllBusesResponse output;
        output.stops_to_buses = stops_to_buses_;
        return output;
    }

private:
    map<string, vector<string>> buses_to_stops_;
    map<string, vector<string>> stops_to_buses_;
};

void TestQueryReader(){
    Query q;
    istringstream input;
    
    input.str("ALL_BUSES"s);
    input >> q;
    assert(q.type == QueryType::AllBuses);

    input.clear();
    input.str("BUSES_FOR_STOP Marushkino"s);
    input >> q;
    assert(q.type == QueryType::BusesForStop);
    assert(q.stop == "Marushkino"s);

    input.clear();
    input.str("STOPS_FOR_BUS 32K"s);
    input >> q;
    assert(q.type == QueryType::StopsForBus);
    assert(q.bus == "32K"s);

    input.clear();
    input.str("NEW_BUS 32 3 Tolstopaltsevo Marushkino Vnukovo"s);
    input >> q;
    assert(q.type == QueryType::NewBus);
    assert(q.bus == "32"s);
    assert(q.stops.size() == 3);
    assert(q.stops[0] == "Tolstopaltsevo"s);
    assert(q.stops[1] == "Marushkino"s);
    assert(q.stops[2] == "Vnukovo"s);
    
}

void TestBusesForStopResponseWriter(){
    BusesForStopResponse buses;
    ostringstream output;
    output << buses;
    assert(output.str() == "No stop"s);

    buses.buses.push_back("32");
    buses.buses.push_back("32K");
    
    ostringstream output2;
    output2 << buses;
    string res = output2.str();
    assert(res == "32 32K "s);
} 

void TestBusManager() {
    BusManager bm;
    AllBusesResponse all_buses = bm.GetAllBuses();  
    BusesForStopResponse stop_busses = bm.GetBusesForStop("Marushkino");
    StopsForBusResponse bus_stops = bm.GetStopsForBus("32K");
    assert(all_buses.stops_to_buses.size() == 0);
    assert(stop_busses.buses.size() == 0);
    assert(bus_stops.buses_to_stops.size() == 0);

    bm.AddBus("32", {"Tolstopaltsevo", "Marushkino", "Vnukovo"});
    bm.AddBus("32K", {"Tolstopaltsevo", "Marushkino", "Vnukovo", "Peredelkino", "Solntsevo", "Skolkovo"});   

    stop_busses = bm.GetBusesForStop("Vnukovo");
    assert(stop_busses.buses.size() == 2);
    assert(stop_busses.buses[0] == "32");
    assert(stop_busses.buses[1] == "32K");

    bm.AddBus("950", {"Kokoshkino", "Marushkino", "Vnukovo", "Peredelkino", "Solntsevo", "Troparyovo"});
    bm.AddBus("272", {"Vnukovo", "Moskovsky", "Rumyantsevo", "Troparyovo"});  

    bus_stops = bm.GetStopsForBus("272");
    assert(bus_stops.buses_to_stops.size() == 4);
    assert(bus_stops.buses_to_stops[0].first == "Vnukovo"s);
    assert(bus_stops.buses_to_stops[0].second.size() == 3);
    
    assert(bus_stops.buses_to_stops[1].first == "Moskovsky"s);
    assert(bus_stops.buses_to_stops[1].second.size() == 0);

    assert(bus_stops.buses_to_stops[2].first == "Rumyantsevo"s);
    assert(bus_stops.buses_to_stops[2].second.size() == 0);

    assert(bus_stops.buses_to_stops[3].first == "Troparyovo"s);
    assert(bus_stops.buses_to_stops[3].second.size() == 1);

    all_buses = bm.GetAllBuses();  
    assert(all_buses.stops_to_buses.size() == 4);
    assert(all_buses.stops_to_buses["272"].size() == 4);
    assert(all_buses.stops_to_buses["32"].size() == 3);
    assert(all_buses.stops_to_buses["32K"].size() == 6);
    assert(all_buses.stops_to_buses["950"].size() == 6);
}

int main() {
    TestQueryReader();
    TestBusesForStopResponseWriter();
    TestBusManager();

    int query_count;
    
    cin >> query_count;

    BusManager bm;
    for (int i = 0; i < query_count; ++i) {
        Query q;
        cin >> q;
        switch (q.type) {
            case QueryType::NewBus:
                bm.AddBus(q.bus, q.stops);
                break;
            case QueryType::BusesForStop:
                cout << bm.GetBusesForStop(q.stop) << endl;
                break;
            case QueryType::StopsForBus:
                cout << bm.GetStopsForBus(q.bus) << endl;
                break;
            case QueryType::AllBuses:
                cout << bm.GetAllBuses() << endl;
                break;
        }
    }
}