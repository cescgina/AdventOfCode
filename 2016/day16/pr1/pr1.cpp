#include <vector>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <unistd.h>


void print_bits(const std::vector<bool>& vec){
    for (auto x: vec){
        std::cout << x;
    }
    std::cout << std::endl;
}

int main(int argc, char** argv){
    std::string file_name, line;
    std::vector<std::string> parsed_line;
    getopt(argc, argv, "");
    file_name = std::string(argv[optind]);
    std::ifstream input_file;
    input_file.open(file_name);
    std::getline(input_file, line);
    size_t size = std::stoi(line);
    std::getline(input_file, line);
    input_file.close();
    std::vector<bool> a;
    for (size_t i = 0; i < line.size(); i++){
        a.push_back(line[i] == '1');
    }
    while (a.size() < size){
        std::vector<bool> b = std::vector<bool>(a);
        std::reverse(b.begin(), b.end());
        b.flip();
        a.push_back(false);
        a.insert(a.end(), b.begin(), b.end());
    }
    while (a.size() != size){
        a.pop_back();
    }
    std::vector<bool> checksum;
    while (checksum.size() % 2 == 0){
        checksum.clear();
        for (size_t j = 0; j < a.size(); j += 2){
            checksum.push_back(a[j] == a[j+1]);
        }
        a = checksum;
    }
    print_bits(a);
}
