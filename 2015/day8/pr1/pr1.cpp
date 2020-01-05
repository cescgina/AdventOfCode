#include <string>
#include <fstream>
#include <iostream>
#include <unistd.h>

int parse_command(const std::string& order){
    int str_len = 0, code_len = order.size(), iter=1;
    // skip first and last quotes
    while (iter<code_len-1){
        if (order.compare(iter, 1, "\\") == 0){
            if (order.compare(iter+1, 1, "x") == 0){
                iter += 4;
            }
            else{
                iter += 2;
            }
        }
        else{
            iter++;
        }
        str_len += 1;
    }
    return code_len-str_len;
}

int main(int argc, char** argv){
    std::string file_name, command;
    getopt(argc, argv, "");
    file_name = std::string(argv[optind]);
    std::ifstream input_file;
    int total = 0;
    input_file.open(file_name);
    while (std::getline(input_file, command)){
        total += parse_command(command);
    }
    input_file.close();
    std::cout << "Total size " << total << std::endl;
}
