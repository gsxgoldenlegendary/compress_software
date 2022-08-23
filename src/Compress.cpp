//
// Created by administrator Guo on 23/11/2021 15:46.
//

#include "Compress.h"

Compress::Compress(unsigned bit_of_unit,unsigned fork_of_tree,const std::string& source_file_name,const std::string&  compress_file_name){
    Compress::bit_of_unit=bit_of_unit;
    Compress::fork_of_tree=fork_of_tree;
    bit_of_move = [fork_of_tree]() -> unsigned {
        if (fork_of_tree == 2)return 1;
        else if (3 <= fork_of_tree && fork_of_tree < 8)return 2;
        else if (8 <= fork_of_tree && fork_of_tree < 16)
            return 3;
        else return 4;
    }();
    read_buffer=0 ;
    read_buffer_temp=0 ;
    bit_of_read_buffer=0 ;
    read_complement=0 ;
    write_buffer=0;
    write_buffer_temp=0;
    bit_of_write_buffer=0;
    Compress::source_file_name=source_file_name;
    Compress::compress_file_name=compress_file_name;
    auto zeroth_HTNode = new HTNode();
    Huffman_tree.push_back(*zeroth_HTNode);
    tree_printer=std::ofstream ("tree.txt");
}

void Compress::open_files() {
    file_reader=std::ifstream (source_file_name, std::ios::binary);
    file_reader2=std::ifstream (source_file_name,std::ios::binary);
    file_writer=std::ofstream (compress_file_name,std::ios::binary);
    if(!file_reader.is_open()||!file_reader2.is_open()||!file_writer){
        std::cout<<"Fail in opening files!\n";
        exit(0);
    }
}

void Compress::get_leaves() {
    for (; file_reader.peek() != EOF || bit_of_read_buffer > 0;) {
        if (bit_of_read_buffer < bit_of_unit) {
            if (file_reader.peek() != EOF) {
                file_reader.read(reinterpret_cast<char *>(&read_buffer_temp), sizeof(char));
                read_buffer = (read_buffer << 8) + read_buffer_temp;
                bit_of_read_buffer += 8;
            } else {
                read_complement = bit_of_unit - bit_of_read_buffer;
                read_buffer = read_buffer << read_complement;
                bit_of_read_buffer = bit_of_unit;
            }
        } else {
            auto number = read_buffer >> (bit_of_read_buffer - bit_of_unit);
            bit_of_read_buffer -= bit_of_unit;
            read_buffer &= ((unsigned long long)1 << bit_of_read_buffer) - 1;
            auto iter = std::find(Huffman_tree.begin() + 1, Huffman_tree.end(), number);
            if (iter != Huffman_tree.end()) {
                ++iter->weight;
                if(iter->weight==INT32_MAX){
                    std::cout<<"To big to compress!\n";
                    exit(0);
                }
            } else {
                auto temp_HTNode = new HTNode();
                temp_HTNode->number = number;
                temp_HTNode->weight = 1;
                Huffman_tree.push_back(*temp_HTNode);
            }
        }
    }
}

void Compress::complete_leaves() {
    for (; (Huffman_tree.size() - 2) % (fork_of_tree - 1) != 0;) {
        auto temp_HTNode = new HTNode();
        Huffman_tree.push_back(*temp_HTNode);
    }
}

void Compress::struct_tree() {
    auto cycle_number = (Huffman_tree.size() - 2) / (fork_of_tree - 1);
    for (unsigned i = 0; i < cycle_number; ++i) {
        auto temp_HTNode = new HTNode();
        for (unsigned j = 0; j < fork_of_tree; ++j) {
            unsigned long long temp_weight;
            auto length = Huffman_tree.size();
            unsigned k = 1;
            for (; Huffman_tree[k].is_in_tree; ++k) {}
            temp_weight = Huffman_tree[k].weight;
            auto min_position = k;
            for (; k < length; ++k) {
                if (!Huffman_tree[k].is_in_tree && Huffman_tree[k].weight < temp_weight) {
                    temp_weight = Huffman_tree[k].weight;
                    min_position = k;
                }
            }
            Huffman_tree[min_position].parent = length;
            Huffman_tree[min_position].is_in_tree = true;
            temp_HTNode->child[j] = min_position;
            temp_HTNode->weight += Huffman_tree[min_position].weight;
        }
        Huffman_tree.push_back(*temp_HTNode);
    }
}

void Compress::encode(unsigned position) {
    for (int i = 0; i < fork_of_tree; ++i) {
        if (Huffman_tree[position].child[i]) {
            Huffman_tree[Huffman_tree[position].child[i]].code = (Huffman_tree[position].code << bit_of_move) + i;
            Huffman_tree[Huffman_tree[position].child[i]].bit_of_code = Huffman_tree[position].bit_of_code + bit_of_move;
            encode(Huffman_tree[position].child[i]);
        }
    }
}
void Compress::encode_tree() {
    Huffman_tree[Huffman_tree.size() - 1].code = 1;
    Huffman_tree[Huffman_tree.size() - 1].bit_of_code = bit_of_move;
    encode(Huffman_tree.size() - 1);
}

void Compress::print_tree() {
    print(Huffman_tree.size()-1,0);
    tree_printer.close();
}

void Compress::print(unsigned position, unsigned depth) {
    for (int i = 0; i < depth; ++i) {
        tree_printer << '\t';
    }
    tree_printer << Huffman_tree[position].code;
    int j = 0;
    for (; Huffman_tree[position].child[j] == 0 && j < fork_of_tree; ++j) {}
    if (j == fork_of_tree) {
        tree_printer << '\t' << Huffman_tree[position].number;
    }
    tree_printer << '\n';
    for (int i = 0; i < fork_of_tree; ++i) {
        if (Huffman_tree[position].child[i] != 0)
            print(Huffman_tree[position].child[i], depth + 1);
    }
}

void Compress::write_tree() {
    auto size = Huffman_tree.size() - 1;
    file_writer.write(reinterpret_cast<char *>(&bit_of_unit), sizeof bit_of_unit);
    file_writer.write(reinterpret_cast<char *>(&bit_of_move), sizeof bit_of_move);
    file_writer.write(reinterpret_cast<char *>(&fork_of_tree), sizeof fork_of_tree);
    file_writer.write(reinterpret_cast<char *>(&read_complement), sizeof read_complement);
    file_writer.write(reinterpret_cast<char *>(&size), sizeof size);
    for (auto iter = Huffman_tree.begin() + 1; iter != Huffman_tree.end(); ++iter) {
        file_writer.write(reinterpret_cast<char *>(&iter->number), sizeof iter->number);
        file_writer.write(reinterpret_cast<char *>(&iter->parent), sizeof iter->parent);
        for (int j = 0; j < fork_of_tree; ++j) {
            file_writer.write(reinterpret_cast<char *>(&iter->child[j]), sizeof iter->child[j]);
        }
    }
}

void Compress::write_content() {
    for (; file_reader2.peek() != EOF || bit_of_read_buffer > 0 || bit_of_write_buffer > 0;) {
        if (file_reader2.peek() == EOF && bit_of_read_buffer == 0 && bit_of_write_buffer < 8) {
            write_buffer = (write_buffer << (8 - bit_of_write_buffer));
            bit_of_write_buffer = 8;
        } else if (bit_of_write_buffer >= 8) {
            write_buffer_temp = write_buffer >> (bit_of_write_buffer - 8);
            bit_of_write_buffer -= 8;
            write_buffer &= ((unsigned long long)1<< bit_of_write_buffer) - 1;
            file_writer.write(reinterpret_cast<char *>(&write_buffer_temp), sizeof(char));
        } else if (bit_of_read_buffer < bit_of_unit) {
            if (file_reader2.peek() != EOF) {
                file_reader2.read(reinterpret_cast<char *>(&read_buffer_temp), sizeof(char));
                read_buffer = (read_buffer << 8) + read_buffer_temp;
                bit_of_read_buffer += 8;
            } else {
                read_complement = bit_of_unit - bit_of_read_buffer;
                read_buffer = read_buffer << read_complement;
                bit_of_read_buffer = bit_of_unit;
            }
        } else {
            auto number = read_buffer >> (bit_of_read_buffer - bit_of_unit);
            bit_of_read_buffer -= bit_of_unit;
            read_buffer &= ((unsigned long long) 1 << bit_of_read_buffer) - 1;
            auto iter = std::find(Huffman_tree.begin() + 1, Huffman_tree.end(), number);
            write_buffer = (write_buffer << iter->bit_of_code) + iter->code;
            bit_of_write_buffer += iter->bit_of_code;
        }
    }
}

void Compress::close_files() {
    file_reader.close();
    file_writer.close();
    file_reader2.close();
    std::cout<<"Compress successfully!\n";
}