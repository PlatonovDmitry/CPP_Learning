#include <cassert>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <regex>
#include <sstream>
#include <string>
#include <string_view>
#include <vector>

using namespace std;
using filesystem::path;

path operator""_p(const char* data, std::size_t sz) {
    return path(data, data + sz);
}

bool Preprocess(const path& in_file, std::ofstream& out, const vector<path>& include_directories);

bool Preprocess(string file_name, std::ofstream& out, const vector<path>& include_directories, string parent_file, int line_num){
    for(auto cur_dir : include_directories){
        if(filesystem::exists(cur_dir / path(file_name))){
            return Preprocess(cur_dir / path(file_name), out, include_directories);
        }
    }

    cout << "unknown include file " << file_name;//<имя файла в директиве>
    cout << " at file " << parent_file; //<имя файла, где директива>
    cout << " at line " << line_num; //<номер строки, где директива>";
    cout << endl;
    return false;
}

bool Preprocess(const path& in_file, std::ofstream& out, const vector<path>& include_directories){
    std::ifstream source(in_file, ios::in);
    if (!source) {
        return false;
    }

    static regex file_reg(R"/(\s*#\s*include\s*"([^"]*)"\s*)/"); 
    static regex lib_reg(R"/(\s*#\s*include\s*<([^>]*)>\s*)/"); 
    smatch m;

    auto root_folder = in_file.lexically_normal().parent_path();

    int line_num = 0;
    string cur_line;
    while (getline(source, cur_line))
    {
        ++line_num;
        if (regex_match(cur_line, m, file_reg)) {
            if(filesystem::exists(root_folder / path(m[1]))){
                if(!Preprocess(root_folder / path(m[1]), out, include_directories)){
                    return false;
                }
            } else if(!Preprocess(path(m[1]), out, include_directories, in_file.string(), line_num)){
                return false;
            }
        } else if(regex_match(cur_line, m, lib_reg)){
            if(!Preprocess(path(m[1]), out, include_directories, in_file.string(), line_num)){
                return false;
            }
        } else{
            out << cur_line << endl;
        }
    }

    return true;
}

// напишите эту функцию
bool Preprocess(const path& in_file, const path& out_file, const vector<path>& include_directories){
    std::ifstream source(in_file, ios::in);
    if (!source) {
        return false;
    }

    std::ofstream dest(out_file, ios::out);
    if (!dest) {
        return false;
    }

    return Preprocess(in_file, dest, include_directories);
}

string GetFileContents(string file) {
    ifstream stream(file);

    // конструируем string по двум итераторам
    return {(istreambuf_iterator<char>(stream)), istreambuf_iterator<char>()};
}

void Test() {
    error_code err;
    filesystem::remove_all("sources"_p, err);
    filesystem::create_directories("sources"_p / "include2"_p / "lib"_p, err);
    filesystem::create_directories("sources"_p / "include1"_p, err);
    filesystem::create_directories("sources"_p / "dir1"_p / "subdir"_p, err);

    {
        ofstream file("sources/a.cpp");
        file << "// this comment before include\n"
                "#include \"dir1/b.h\"\n"
                "// text between b.h and c.h\n"
                "#include \"dir1/d.h\"\n"
                "\n"
                "int SayHello() {\n"
                "    cout << \"hello, world!\" << endl;\n"
                "#   include<dummy.txt>\n"
                "}\n"sv;
    }
    {
        ofstream file("sources/dir1/b.h");
        file << "// text from b.h before include\n"
                "#include \"subdir/c.h\"\n"
                "// text from b.h after include"sv;
    }
    {
        ofstream file("sources/dir1/subdir/c.h");
        file << "// text from c.h before include\n"
                "#include <std1.h>\n"
                "// text from c.h after include\n"sv;
    }
    {
        ofstream file("sources/dir1/d.h");
        file << "// text from d.h before include\n"
                "#include \"lib/std2.h\"\n"
                "// text from d.h after include\n"sv;
    }
    {
        ofstream file("sources/include1/std1.h");
        file << "// std1\n"sv;
    }
    {
        ofstream file("sources/include2/lib/std2.h");
        file << "// std2\n"sv;
    }

    assert((!Preprocess("sources"_p / "a.cpp"_p, "sources"_p / "a.in"_p,
                                  {"sources"_p / "include1"_p,"sources"_p / "include2"_p})));

    ostringstream test_out;
    test_out << "// this comment before include\n"
                "// text from b.h before include\n"
                "// text from c.h before include\n"
                "// std1\n"
                "// text from c.h after include\n"
                "// text from b.h after include\n"
                "// text between b.h and c.h\n"
                "// text from d.h before include\n"
                "// std2\n"
                "// text from d.h after include\n"
                "\n"
                "int SayHello() {\n"
                "    cout << \"hello, world!\" << endl;\n"sv;

    auto res_str = GetFileContents("sources/a.in"s);
    auto comp_str =  test_out.str();
    assert(res_str == comp_str);
}

int main() {
    Test();
}