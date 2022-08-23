//
// Created by administrator Guo on 23/11/2021 15:46.
//

#ifndef DSHW2_V1_COMPRESS_H
#define DSHW2_V1_COMPRESS_H
#include "main.h"

class Compress {
private:
    std::vector<HTNode> Huffman_tree;
    unsigned bit_of_unit;
    unsigned fork_of_tree;
    unsigned bit_of_move;
    unsigned long long read_buffer ;
    unsigned char read_buffer_temp ;
    unsigned bit_of_read_buffer ;
    unsigned read_complement ;
    unsigned long long write_buffer ;
    unsigned bit_of_write_buffer ;
    unsigned char write_buffer_temp ;
    std::string source_file_name;
    std::string compress_file_name;
    std::ifstream file_reader;
    std::ifstream file_reader2;
    std::ofstream file_writer;
    std::ofstream tree_printer;
public:
    Compress(unsigned ,unsigned ,const std::string&,const std::string&);
    void open_files();
    void get_leaves();
    void complete_leaves();
    void struct_tree();
    void encode(unsigned );
    void encode_tree();
    void print(unsigned ,unsigned );
    void print_tree();
    void write_tree();
    void write_content();
    void close_files();
};


#endif //DSHW2_V1_COMPRESS_H