#include "json.h"

using namespace std;

namespace json {

namespace detail{

Node LoadNode(istream& input);

Node LoadArray(istream& input) {
    Array result;

    for (char c; input >> c;) {
        if (c == ']'){
            return Node(move(result));
        }

        if (c != ',') {
            input.putback(c);
        }
        result.push_back(LoadNode(input));
    }

    throw ParsingError("Failed to parse array from stream"s);
}

Node LoadNumber(istream& input) {
    using namespace std::literals;

    std::string parsed_num;

    // Считывает в parsed_num очередной символ из input
    auto read_char = [&parsed_num, &input] {
        parsed_num += static_cast<char>(input.get());
        if (!input) {
            throw ParsingError("Failed to read number from stream"s);
        }
    };

    // Считывает одну или более цифр в parsed_num из input
    auto read_digits = [&input, read_char] {
        if (!std::isdigit(input.peek())) {
            throw ParsingError("A digit is expected"s);
        }
        while (std::isdigit(input.peek())) {
            read_char();
        }
    };

    if (input.peek() == '-') {
        read_char();
    }
    // Парсим целую часть числа
    if (input.peek() == '0') {
        read_char();
        // После 0 в JSON не могут идти другие цифры
    } else {
        read_digits();
    }

    bool is_int = true;
    // Парсим дробную часть числа
    if (input.peek() == '.') {
        read_char();
        read_digits();
        is_int = false;
    }

    // Парсим экспоненциальную часть числа
    if (int ch = input.peek(); ch == 'e' || ch == 'E') {
        read_char();
        if (ch = input.peek(); ch == '+' || ch == '-') {
            read_char();
        }
        read_digits();
        is_int = false;
    }

    try {
        if (is_int) {
            // Сначала пробуем преобразовать строку в int
            try {
                return Node(std::stoi(parsed_num));
            } catch (...) {
                // В случае неудачи, например, при переполнении,
                // код ниже попробует преобразовать строку в double
            }
        }
        return Node(std::stod(parsed_num));
    } catch (...) {
        throw ParsingError("Failed to convert "s + parsed_num + " to number"s);
    }
}

Node LoadString(istream& input) {
    using namespace std::literals;
    
    auto it = std::istreambuf_iterator<char>(input);
    auto end = std::istreambuf_iterator<char>();
    std::string s;
    while (true) {
        if (it == end) {
            // Поток закончился до того, как встретили закрывающую кавычку?
            throw ParsingError("String parsing error");
        }
        const char ch = *it;
        if (ch == '"') {
            // Встретили закрывающую кавычку
            ++it;
            break;
        } else if (ch == '\\') {
            // Встретили начало escape-последовательности
            ++it;
            if (it == end) {
                // Поток завершился сразу после символа обратной косой черты
                throw ParsingError("String parsing error");
            }
            const char escaped_char = *(it);
            // Обрабатываем одну из последовательностей: \\, \n, \t, \r, \"
            switch (escaped_char) {
                case 'n':
                    s.push_back('\n');
                    break;
                case 't':
                    s.push_back('\t');
                    break;
                case 'r':
                    s.push_back('\r');
                    break;
                case '"':
                    s.push_back('"');
                    break;
                case '\\':
                    s.push_back('\\');
                    break;
                default:
                    // Встретили неизвестную escape-последовательность
                    throw ParsingError("Unrecognized escape sequence \\"s + escaped_char);
            }
        } else if (ch == '\n' || ch == '\r') {
            // Строковый литерал внутри- JSON не может прерываться символами \r или \n
            throw ParsingError("Unexpected end of line"s);
        } else {
            // Просто считываем очередной символ и помещаем его в результирующую строку
            s.push_back(ch);
        }
        ++it;
    }

    return Node(move(s));
}

Node LoadDict(istream& input) {
    Dict result;

    for (char c; input >> c;) {

        if(c=='}'){
            return Node(move(result));
        }

        if (c == ',') {
            input >> c;
        }

        string key = LoadString(input).AsString();
        input >> c;
        result.insert({move(key), LoadNode(input)});
    }

    throw ParsingError("Failed to parse map from stream"s);    
}

Node LoadBool(istream& input){
    std::string s;

    int i = 0;
    while(i < 4){
        s += tolower(static_cast<char>(input.get()));
        ++i;
    }

    if(s == "true"s){
        return Node(true);
    } else if (s == "fals"s){
        s += tolower(static_cast<char>(input.get()));
        if(s == "false"s){
            return Node(false);
        }
    }

    throw ParsingError("Cannot parse bool value"s);
}

Node LoadNull(istream& input){
    std::string s;

    int i = 0;
    while(i < 4){
        s += tolower(static_cast<char>(input.get()));
        ++i;
    }

    if(s == "null"s){
        return Node();
    }

    throw ParsingError("Cannot parse null value"s);
}

Node LoadNode(istream& input) {
    char c;
    input >> c;

    if (c == '[') {
        return LoadArray(input);
    } else if (c == '{') {
        return LoadDict(input);
    } else if (c == '"') {
        return LoadString(input);
    } else if (c == 't' || c == 'T' || c == 'f' || c == 'F') {
        input.putback(c);
        return LoadBool(input);
    } else if (c == 'n' || c == 'N') {
        input.putback(c);
        return LoadNull(input);
    } else {
        input.putback(c);
        return LoadNumber(input);
    }
}

}  // namespace

bool Node::IsInt() const{
    return std::holds_alternative<int>(*this);
}
bool Node::IsDouble() const{
    return 
        std::holds_alternative<int>(*this) || 
        std::holds_alternative<double>(*this);
}
bool Node::IsPureDouble() const{
    return std::holds_alternative<double>(*this);
}
bool Node::IsBool() const{
    return std::holds_alternative<bool>(*this);
}
bool Node::IsString() const{
    return std::holds_alternative<std::string>(*this);
}
bool Node::IsNull() const{
    return std::holds_alternative<std::nullptr_t>(*this);
}
bool Node::IsArray() const{
    return std::holds_alternative<Array>(*this);
}
bool Node::IsMap() const{
    return std::holds_alternative<Dict>(*this);
}


const Array& Node::AsArray() const {
    return IsArray() ? get<Array>(*this) : throw std::logic_error("Not an array");
}
const Dict& Node::AsMap() const {
    return IsMap() ? get<Dict>(*this) : throw std::logic_error("Not an map");
}
int Node::AsInt() const {
    return IsInt() ? get<int>(*this) : throw std::logic_error("Not an int");
}
bool Node::AsBool() const {
    return IsBool() ? get<bool>(*this) : throw std::logic_error("Not an bool");
}
double Node::AsDouble() const {
    if(IsPureDouble()){
        return get<double>(*this);
    } else if (IsInt()){
        return static_cast<double>(get<int>(*this));
    }

    throw std::logic_error("Not an double");
}
const string& Node::AsString() const {
    return IsString() ? get<string>(*this) : throw std::logic_error("Not an string");
}

Document::Document(Node root)
    : root_(move(root)) {
}

const Node& Document::GetRoot() const {
    return root_;
}

Document Load(istream& input) {
    return Document{detail::LoadNode(input)};
}

void PrintValue(std::nullptr_t, std::ostream& out) {
    out << "null"sv;
}

void PrintValue(bool bool_val, std::ostream& out) {
    out << (bool_val ? "true"sv : "false"sv);
}

void PrintValue(string string_val, std::ostream& out) {
    out << "\""s;
    for(char cur_char: string_val){
        switch (cur_char)
        {
        case '\\':
            out << "\\"s << "\\"s ;
            break;
        case '\r':
            out << "\\"s << "r"s ;
            break;
        case '\n':
            out << "\\"s << "n"s ;
            break;
        case '\"':
            out << "\\"s << "\""s ;
            break;
        default:
            out << cur_char;
            break;
        }
    }
    out << "\""s;
}

void PrintValue(Array array_val, std::ostream& out) {
    out << "["sv;
    bool is_first = true;
    for(Node cur_node: array_val){
        if (is_first){
            is_first = false;
        } else{
            out << ", "sv;
        }
        PrintNode(cur_node, out);
    }
    out << "]"sv;
}

void PrintValue(Dict dict_val, std::ostream& out) {
    out << "{"sv;
    bool is_first = true;
    for(auto& [key, val]: dict_val){
        if (is_first){
            is_first = false;
        } else{
            out << ", "sv;
        }

        out << "\""sv << key << "\": ";

        PrintNode(val, out);
    }
    out << "}"sv;
}

void PrintNode(const Node& node, std::ostream& output){
    if(node.IsNull()){
        PrintValue(std::nullptr_t{}, output);
    }else if(node.IsBool()){
        PrintValue(node.AsBool(), output);
    }else if(node.IsInt()){
        PrintValue(node.AsInt(), output);
    }else if(node.IsDouble()){
        PrintValue(node.AsDouble(), output);
    }else if(node.IsString()){
        PrintValue(node.AsString(), output);
    }else if(node.IsArray()){
        PrintValue(node.AsArray(), output);
    }else if(node.IsMap()){
        PrintValue(node.AsMap(), output);
    }
}

void Print(const Document& doc, std::ostream& output) {
    PrintNode(doc.GetRoot(), output);
}

}  // namespace json