#pragma once

#include <array>
#include <iostream>
#include <fstream>
#include <string>

// напишите эту функцию
inline bool DecodeRLE(const std::string& src_name, const std::string& dst_name){

    std::ifstream in_file(src_name, std::ios::in | std::ios::binary);
    if (!in_file) {
        return false;
    }

    std::ofstream out_file(dst_name, std::ios::out | std::ios::binary);
    if (!out_file) {
        return false;
    }

    static const int BUFF_SIZE = 1024;
    std::array<char, BUFF_SIZE> buffer;

    do {
        unsigned char header;

        auto block = in_file.get();
        if(block == std::istream::traits_type::eof()){
            break;
        }

        header = static_cast<unsigned char>(block);

        int block_type = (header & 1);
        int data_size = (header >> 1) + 1;

        if(block_type){
            auto zip_char = static_cast<unsigned char>(in_file.get());
            std::string data(data_size, zip_char);
            out_file.write(data.c_str(), data_size);    
        } else{
            in_file.read(buffer.data(), data_size);
            out_file.write(buffer.data(), data_size);
        }
    } while (in_file);

    return true;
}
