#pragma once

#include <iostream>
#include <map>
#include <string>
#include <variant>
#include <vector>

const double RELEVANCE_TOLERANCE = 1e-6;

namespace json {

class Node;
// Сохраните объявления Dict и Array без изменения
using Dict = std::map<std::string, Node>;
using Array = std::vector<Node>;

// Эта ошибка должна выбрасываться при ошибках парсинга JSON
class ParsingError : public std::runtime_error {
public:
    using runtime_error::runtime_error;
};

class Node final : private std::variant<std::nullptr_t, int, double, std::string, bool, Array, Dict>{
public:
    using variant::variant;

    bool IsInt() const;
    bool IsDouble() const;
    bool IsPureDouble() const; 
    bool IsBool() const;
    bool IsString() const;
    bool IsNull() const;
    bool IsArray() const;
    bool IsMap() const;

    int AsInt() const;
    bool AsBool() const;
    double AsDouble() const;
    const Array& AsArray() const;
    const Dict& AsMap() const;
    const std::string& AsString() const;
};

inline bool operator==(const Node &lhs, const Node &rhs) 
{   
    if(lhs.IsNull() && rhs.IsNull()){
        return true;
    } else if(lhs.IsBool() && rhs.IsBool()){
        return lhs.AsBool() == rhs.AsBool();
    } else if(lhs.IsInt() && rhs.IsInt()){
        return lhs.AsInt() == rhs.AsInt();
    } else if(lhs.IsPureDouble() && rhs.IsPureDouble()){
        return lhs.AsDouble() - rhs.AsDouble() < RELEVANCE_TOLERANCE;
    } else if(lhs.IsString() && rhs.IsString()){
        return lhs.AsString() == rhs.AsString();
    } else if(lhs.IsArray() && rhs.IsArray()){
        return lhs.AsArray() == rhs.AsArray();
    } else if(lhs.IsMap() && rhs.IsMap()){
        return lhs.AsMap() == rhs.AsMap();
    } else{
        return false;
    }
}
inline bool operator!=(const Node &lsh, const Node &rsh) {return !(lsh == rsh); }

enum class NodeType {
    NONE,
    INT,
    DOUBLE,
    PURE_DOUBLE,
    BOOL,
    STRING,
    ARRAY,
    MAP
};

class Document {
public:
    explicit Document(Node root);

    const Node& GetRoot() const;

private:
    Node root_;
};
inline bool operator==(const Document &lhs, const Document &rhs){
    return lhs.GetRoot() == rhs.GetRoot();
}
inline bool operator!=(const Document &lhs, const Document &rhs){
    return !(lhs == rhs);
}

Document Load(std::istream& input);

void Print(const Document& doc, std::ostream& output);

void PrintNode(const Node& node, std::ostream& output);

template <typename Value>
void PrintValue(const Value& value, std::ostream& out) {
    out << value;
}

}  // namespace json