#include <iostream>
#include <string>
#include <unordered_map>

namespace ini{
    using Section = std::unordered_map<std::string, std::string>;

    class Document {
    public:
        explicit Document(std::unordered_map<std::string, Section> sections);
        // реализация методов должна быть в файле ini.cpp
        Section& AddSection(std::string name);
        const Section& GetSection(const std::string& name) const;
        std::size_t GetSectionCount() const;

    private:
        std::unordered_map<std::string, Section> sections_;
        Section empty_section_;
    };

    // определение этой функции должно быть в файле ini.cpp
    Document Load(std::istream& input);
}