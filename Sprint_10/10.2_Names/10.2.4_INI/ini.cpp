#include "ini.h"

using namespace std;

namespace ini{

    Document::Document(std::unordered_map<std::string, Section> sections)
    : sections_(sections)
    {

    }

    Section& Document::AddSection(std::string name){
        return sections_[name];
    }

    const Section& Document::GetSection(const std::string& name) const{
        if(sections_.count(name)>0){
            return sections_.at(name);
        }
        
        return empty_section_;
    }

    std::size_t Document::GetSectionCount() const{
        return sections_.size();
    }

    Document Load(istream& input) {
        std::unordered_map<std::string, Section> sections;
        string cur_section_name;
        string cur_line;

        while(getline(input, cur_line)){
            size_t section_name_start = cur_line.find('[');   
            if(section_name_start != string::npos) {
                size_t section_name_len = cur_line.find(']') - section_name_start - 1;
                cur_section_name = cur_line.substr(section_name_start + 1, section_name_len);
                sections[cur_section_name];
                continue;
            }

            size_t set_start = cur_line.find('=');   
            if(set_start != string::npos){
                auto key_start = cur_line.find_first_not_of(' ');
                auto key_len = cur_line.substr(0, set_start).find_last_not_of(' ') - key_start + 1;
                auto val_start = cur_line.find_first_not_of(' ', set_start + 1);
                auto val_len = cur_line.find_last_not_of(' ') - val_start + 1;
                sections[cur_section_name][cur_line.substr(key_start, key_len)] = cur_line.substr(val_start, val_len);
            }
        }

        return Document(sections);
    }
}