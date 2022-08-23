//
// Created by administrator Guo on 23/11/2021 15:46.
//
#include "Compress.h"
#include "Decompress.h"
//It's main function, the main part of the programme.
int main() {
    unsigned bit_of_unit;
    unsigned fork_of_tree;
    std::string operation;
    std::cout<<"Wait for command:\n";
    std::cin>>operation;
    std::string source_file_name;
    std::string compress_file_name;
    std::string decompress_file_name;
    if(operation=="compress"||operation=="c"){
        std::cout<<"Bits of unit:\n";
        std::cin>>bit_of_unit;
        if(!(bit_of_unit%2==0&&4<=bit_of_unit&&bit_of_unit<=32)){
            std::cout<<"Unsupported bit of unit!\n";
            exit(0);
        }
        std::cout<<"Forks of tree:\n";
        std::cin>>fork_of_tree;
        if(!(2<=fork_of_tree&&fork_of_tree<=16)){
            std::cout<<"Unsupported fork of tree!\n";
            exit(0);
        }
        std::cout<<"Source file name:\n";
        std::cin>>source_file_name;
        std::cout<<"Compress file name:\n";
        std::cin>>compress_file_name;
        Compress compress(bit_of_unit,fork_of_tree,source_file_name,compress_file_name);
        compress.open_files();
        compress.get_leaves();
        compress.complete_leaves();
        compress.struct_tree();
        compress.encode_tree();
        compress.print_tree();
        compress.write_tree();
        compress.write_content();
        compress.close_files();
    }else if(operation=="decompress"||operation=="d"){
        std::cout<<"Compress file name:\n";
        std::cin>>compress_file_name;
        std::cout<<"Decompress file name:\n";
        std::cin>>decompress_file_name;
        Decompress decompress(compress_file_name,decompress_file_name);
        decompress.open_files();
        decompress.read_tree();
        decompress.write_content();
        decompress.close_files();
    }else{
        std::cout<<"Illegal operation!\n";
    }
    //It's a good habit.
    return 0;
}