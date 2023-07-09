#include <cassert>
#include <fstream>
#include <iostream>
#include <string>
#include <string_view>

using namespace std;

string GetLine(istream& in) {
    string s;
    getline(in, s);
    return s;
}

void CreateFiles(const string& config_file){
    ifstream config{config_file};

    ofstream writer;

    string cur_line = GetLine(config);
    while (cur_line.size()>0)
    {
        if(cur_line[0] == '>'){
            writer << cur_line.substr(1) << endl;
        } else{
            writer.close();
            writer.open(cur_line);
        }

        cur_line = GetLine(config);
    }   
}


int main() {
    ofstream("test_config.txt"s) << "a.txt\n"
                                    ">10\n"
                                    ">abc\n"
                                    "b.txt\n"
                                    ">123"sv;

    CreateFiles("test_config.txt"s);
    ifstream in_a("a.txt"s);
    assert(GetLine(in_a) == "10"s && GetLine(in_a) == "abc"s);

    ifstream in_b("b.txt"s);
    assert(GetLine(in_b) == "123"s);
}
