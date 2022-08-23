//
// Created by administrator Guo on 23/11/2021 18:30.
//

#ifndef DSHW2_V1_DECOMPRESS_H
#define DSHW2_V1_DECOMPRESS_H

#include "main.h"
class Decompress {
private:
    std::vector<HTNode> Huffman_tree2;
    std::ifstream file_reader3;
    std::ofstream file_writer2;
    unsigned bit_of_unit2;
    unsigned fork_of_tree2;
    unsigned bit_of_move2;
    unsigned read_complement2;
    unsigned long long size2;
    unsigned long long read_buffer ;
    unsigned char read_buffer_temp ;
    unsigned bit_of_read_buffer ;
    unsigned long long write_buffer ;
    unsigned bit_of_write_buffer ;
    unsigned char write_buffer_temp ;
    std::string decompress_file_name;
    std::string compress_file_name;
public:
    Decompress(const std::string&,const std::string&);
    void open_files();
    void read_tree();
    void write_content();
    void close_files();
};


#endif //DSHW2_V1_DECOMPRESS_H