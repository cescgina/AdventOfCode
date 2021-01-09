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
    int phases = 100;
    if (file_name == "data/test.txt") phases = 4;
    std::vector<int> base = {0, 1, 0, -1};

    for (int i=0; i<phases; i++){
        std::vector<int> new_numbers = numbers;
        for (size_t ii = 0; ii < numbers.size(); ii++){
            int number = 0;
            int size_pattern = 4*(ii+1);
            for (size_t jj = 0; jj < numbers.size(); jj++){
                number += (numbers[jj]*base[((jj+1)%size_pattern)/(ii+1)]);
            }
            new_numbers[ii] = std::abs(number) % 10;
        }
        numbers.swap(new_numbers);
    }
    for (size_t j = 0; j<8; j++){
        std::cout << numbers[j];
    }
    std::cout << std::endl;
}
