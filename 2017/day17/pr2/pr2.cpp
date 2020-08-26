#include <vector>
#include <string>
#include <fstream>
#include <utility>
#include <iostream>
#include <algorithm>
#include <unistd.h>

void print_values(const std::vector<int>& vals){
    for (auto x: vals){
        std::cout << x << " ";
    }
    std::cout << std::endl;
}

int main(int argc, char** argv){
    std::string file_name, line;
    getopt(argc, argv, "");
    file_name = std::string(argv[optind]);
    std::ifstream input_file;
    input_file.open(file_name);
    std::vector<std::string> parsed_line;
    std::getline(input_file, line);
    input_file.close();
    int curr_pos = 0, steps = std::stoi(line), value = 1, val = 0;
    while(value <= 50000000) {
        curr_pos = ((curr_pos+steps) % (value))+1;
        if (curr_pos == 1){
            val = value;
        }
        value++;
    }
    std::cout << val << std::endl;
}
