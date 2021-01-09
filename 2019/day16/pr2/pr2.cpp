#include <map>
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <unistd.h>

bool DEBUG = false;

int main(int argc, char** argv){
    std::string file_name;
    getopt(argc, argv, "");
    file_name = std::string(argv[optind]);
    std::vector<int> numbers;
    std::ifstream input_file;
    input_file.open(file_name);
    std::string line; 
    std::getline(input_file, line);
    input_file.close();
    for (auto c: line){
        numbers.push_back(c-'0');
    }
    size_t offset = 0, base_10 = 1;
    for (int i = 6; i>= 0; i--){
        offset += base_10*numbers[i];
        base_10 *= 10;
    }
    int orig_size = numbers.size();
    std::vector<int> full_msg(orig_size*10000, 0);
    for (int i = 0; i< 10000; i++){
        std::copy(numbers.begin(), numbers.end(), full_msg.begin()+(i*orig_size));
    }
    int phases = 100;
    std::vector<int> base = {0, 1, 0, -1};
    size_t full_size = full_msg.size();

    for (int i=0; i<phases; i++){
        int number = 0;
        for (size_t jj = full_size; jj >= offset; jj--){
            number += full_msg[jj];
            full_msg[jj] = std::abs(number) % 10;
        }
    }
    for (size_t j = 0; j<8; j++){
        std::cout << full_msg[offset+j];
    }
    std::cout << std::endl;
}
