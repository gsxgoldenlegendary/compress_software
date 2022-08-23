//
// Created by administrator Guo on 23/11/2021 18:30.
//

#include "Decompress.h"

Decompress::Decompress(const std::string&compress_file_name,const std::string&decompress_file_name) {
    read_buffer=0 ;
    read_buffer_temp=0 ;
    bit_of_read_buffer=0 ;
    read_complement2=0 ;
    write_buffer=0;
    write_buffer_temp=0;
    bit_of_write_buffer=0;
    bit_of_unit2=0;
    fork_of_tree2=0;
    bit_of_move2=0;
    size2=0;
    Decompress::compress_file_name=compress_file_name;
    Decompress::decompress_file_name=decompress_file_name;
    auto zeroth_HTNode2 = new HTNode();
    Huffman_tree2.push_back(*zeroth_HTNode2);
}

void Decompress::close_files() {
    file_reader3.close();
    file_writer2.close();
    std::cout<<"Decompress successfully!\n";
}

void Decompress::write_content() {
    unsigned long long position = Huffman_tree2.size() - 1;
    bool is_new_number = true;
    for (; file_reader3.peek() != EOF || bit_of_read_buffer > 0 || bit_of_write_buffer > read_complement2;) {
        if (bit_of_write_buffer >= read_complement2 + 8) {
            write_buffer_temp = write_buffer >> (bit_of_write_buffer - 8);
            bit_of_write_buffer -= 8;
            write_buffer &= ((unsigned long long)1 << bit_of_write_buffer) - 1;
            file_writer2.write(reinterpret_cast<char *>(&write_buffer_temp), sizeof write_buffer_temp);
        } else if (bit_of_read_buffer == 0) {
            file_reader3.read(reinterpret_cast<char *>(&read_buffer_temp), sizeof read_buffer_temp);
            bit_of_read_buffer += 8;
            read_buffer=(read_buffer<<8)+read_buffer_temp;
        } else {
            if (is_new_number) {
                if (read_buffer >> (bit_of_read_buffer - bit_of_move2) != 0) {
                    position = Huffman_tree2.size() - 1;
                    is_new_number = false;
                }
                bit_of_read_buffer -= bit_of_move2;
                read_buffer &= ((unsigned long long) 1 << bit_of_read_buffer) - 1;
            } else {
                int k = 0;
                for (; Huffman_tree2[position].child[k] == 0 && k < fork_of_tree2; ++k) {}
                if (k == fork_of_tree2) {
                    write_buffer = (write_buffer << bit_of_unit2) + Huffman_tree2[position].number;
                    bit_of_write_buffer += bit_of_unit2;
                    is_new_number = true;
                } else {
                    position = Huffman_tree2[position].child[read_buffer >> (bit_of_read_buffer - bit_of_move2)];
                    bit_of_read_buffer -= bit_of_move2;
                    read_buffer &= ((unsigned long long) 1 << bit_of_read_buffer) - 1;
                }
            }
        }
    }

}

void Decompress::read_tree() {
    file_reader3.read(reinterpret_cast<char *>(&bit_of_unit2), sizeof bit_of_unit2);
    file_reader3.read(reinterpret_cast<char *>(&bit_of_move2), sizeof bit_of_move2);
    file_reader3.read(reinterpret_cast<char *>(&fork_of_tree2), sizeof fork_of_tree2);
    file_reader3.read(reinterpret_cast<char *>(&read_complement2), sizeof read_complement2);
    file_reader3.read(reinterpret_cast<char *>(&size2), sizeof size2);
    for (int i = 0; i < size2; ++i) {
        auto temp_HTNode = new HTNode();
        file_reader3.read(reinterpret_cast<char *>(&temp_HTNode->number), sizeof temp_HTNode->number);
        file_reader3.read(reinterpret_cast<char *>(&temp_HTNode->parent), sizeof temp_HTNode->parent);
        for (int j = 0; j < fork_of_tree2; ++j) {
            file_reader3.read(reinterpret_cast<char *>(&temp_HTNode->child[j]), sizeof temp_HTNode->child[j]);
        }
        Huffman_tree2.push_back(*temp_HTNode);
    }
}

void Decompress::open_files() {
    file_reader3=std::ifstream (compress_file_name, std::ios::binary);
    file_writer2=std::ofstream (decompress_file_name, std::ios::binary);
    if(!file_reader3.is_open()||!file_writer2.is_open()){
        std::cout<<"Fail in opening files!\n";
        exit(0);
    }
}