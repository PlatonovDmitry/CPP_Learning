#include <algorithm>
#include <iostream>
#include <string>
#include <sstream>
#include <string_view>
#include <vector>

using namespace std;

class Domain {
public:
    Domain(string_view path) :
        path_(string(path))
    {
        reverse(path_.begin(), path_.end());
        path_.insert(path_.end(), '.');
    }

    bool operator==(const Domain& other){
        return lexicographical_compare(
            path_.begin(),
            path_.end(),
            other.path_.begin(),
            other.path_.end()
        );
    }

    bool operator<(const Domain& other) const{
        return path_.compare(other.path_) < 0;
    }

    bool IsSubdomain(const Domain& other) const{
        if(other.path_.size() > path_.size()){
            return false;
        }

        auto answer = std::equal(
            other.path_.begin(),
            other.path_.end(),
            path_.begin()
        );

        return answer;
    }
private:
    string path_;
};

template <typename ForwardIt>
class DomainChecker {
public:
    // конструктор должен принимать список запрещённых доменов через пару итераторов
    DomainChecker(ForwardIt begin, ForwardIt end){
        copy(
            begin,
            end,
            back_inserter(restricts_)
        );
        
        sort(
            restricts_.begin(),
            restricts_.end()
        );

        last_ = unique(
            restricts_.begin(),
            restricts_.end(),
            [](const Domain& lsh, const Domain& rsh){return rsh.IsSubdomain(lsh);}
        );
    }

    // разработайте метод IsForbidden, возвращающий true, если домен запрещён
    bool IsForbidden(const Domain& domain){
        auto lower = upper_bound(
            restricts_.begin(),
            last_,
            domain
        );

        if (lower == restricts_.begin()){
            return false;
        }

        return domain.IsSubdomain(*--lower);
    }
private:
    vector<Domain> restricts_;
    vector<Domain>::iterator last_;
};

// разработайте функцию ReadDomains, читающую заданное количество доменов из стандартного входа
vector<Domain> ReadDomains(istream& input, size_t size){
    vector<Domain> output;
    string line;

    for(size_t i=0; i<size; ++i){
        getline(input, line);
        output.push_back({line});
    }

    return output;
}

template <typename Number>
Number ReadNumberOnLine(istream& input) {
    string line;
    getline(input, line);

    Number num;
    std::istringstream(line) >> num;

    return num;
}

int main() {
    const std::vector<Domain> forbidden_domains = ReadDomains(cin, ReadNumberOnLine<size_t>(cin));
    DomainChecker checker(forbidden_domains.begin(), forbidden_domains.end());

    const std::vector<Domain> test_domains = ReadDomains(cin, ReadNumberOnLine<size_t>(cin));
    for (const Domain& domain : test_domains) {
        cout << (checker.IsForbidden(domain) ? "Bad"sv : "Good"sv) << endl;
    }
}