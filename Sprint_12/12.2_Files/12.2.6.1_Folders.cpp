#include <algorithm>
#include <cassert>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <string_view>

using namespace std;
using filesystem::path;

path operator""_p(const char* data, std::size_t sz) {
    return path(data, data + sz);
}

void PrintFolder(ostream& dst, const path& p, int level){
    map<string, bool> dir;
    for (const auto& dir_entry: filesystem::directory_iterator(p)) {
        dir[dir_entry.path().filename()] = dir_entry.status().type() == filesystem::file_type::directory;
    }

    for_each(
        dir.rbegin(),
        dir.rend(),
        [level, &dst, &p](const pair<string, bool> item){
            dst << string(2*level, ' ') << item.first << endl;
            if(item.second){
                PrintFolder(dst, p / item.first, level + 1);
            }    
        }
    );
}

// напишите эту функцию
void PrintTree(ostream& dst, const path& p){
    auto stat = filesystem::status(p);
    dst << p.filename().string() << endl;

    if(stat.type() == filesystem::file_type::directory){
        PrintFolder(dst, p, 1);
    }
}



int main() {
    error_code err;
    filesystem::remove_all("test_dir", err);
    filesystem::create_directories("test_dir"_p / "a"_p, err);
    filesystem::create_directories("test_dir"_p / "b"_p, err);

    ofstream("test_dir"_p / "b"_p / "f1.txt"_p);
    ofstream("test_dir"_p / "b"_p / "f2.txt"_p);
    ofstream("test_dir"_p / "a"_p / "f3.txt"_p);

    ostringstream out;
    PrintTree(out, "test_dir"_p);
    auto res = out.str();
    auto tst = "test_dir\n"
        "  b\n"
        "    f2.txt\n"
        "    f1.txt\n"
        "  a\n"
        "    f3.txt\n"sv;
    assert(out.str() == tst);
}