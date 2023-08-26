#include <algorithm>
#include <iostream>
#include <list>
#include <string>
#include <unordered_map>
#include <vector>

using namespace std;

class BookManager{
public:
    void Read(int user, int page){
        if(page<=0){
            return;
        }

        auto new_place = lower_bound(
            progres_vector_.begin(),
            progres_vector_.end(),
            page
        );
        progres_vector_.insert(new_place, page);
        
        if(progres_map_.count(user) > 0){
            auto old_place = lower_bound(
                progres_vector_.begin(),
                progres_vector_.end(),
                progres_map_.at(user)
            );

            progres_vector_.erase(old_place);
        }

        progres_map_[user] = page;
    }

    double Cherr(int user){
        if(progres_map_.count(user)==0){
            return 0;
        }

        if(progres_map_.size()==1){
            return 1;
        }

        auto loser_count = distance(
            progres_vector_.begin(),
            lower_bound(
                progres_vector_.begin(),
                progres_vector_.end(),
                progres_map_.at(user))
        );

        return static_cast<double> (loser_count) / (progres_vector_.size() - 1);
    }
private:
    vector<int> progres_vector_;
    unordered_map<int, int> progres_map_;
};

enum class QueryType{
    CHEER,
    READ
};

struct BookQuery{
    QueryType type;
    int user_id;
    int book_page = 0;
};

BookQuery ParseQuery(){
    BookQuery result;

    string query_type;
    cin >> query_type;
    if(query_type == "CHEER"s){
        result.type = QueryType::CHEER;
        cin >> result.user_id;
    } else if (query_type == "READ"s){
        result.type = QueryType::READ;
        cin >> result.user_id;
        cin >> result.book_page;
    }

    return result;
}

int main(){
    BookManager manager;

    int query_count;
    cin >> query_count;

    for(int i=0; i<query_count; ++i){
        auto query = ParseQuery();
        if(query.type == QueryType::CHEER){
            auto part = manager.Cherr(query.user_id);
            cout << part << endl;
        } else if (query.type == QueryType::READ){
            manager.Read(query.user_id, query.book_page);
        }
    }
}
