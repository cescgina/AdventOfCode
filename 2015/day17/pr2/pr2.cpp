#include <vector>
#include <fstream>
#include <iostream>
#include <unistd.h>
#include "cppitertools/combinations.hpp"


typedef std::vector<int> containers;

template <typename T>
void print_vector(const std::vector<T>& elements){
    for (auto it=elements.begin(); it!=elements.end(); it++){
        std::cout << *it << " ";
    }
    std::cout << std::endl;
}

int count_combinations(const int& liters, const containers& bottles){
    int num_comb = 0, sum = 0;
    for (size_t i=2; i< bottles.size(); i++){
        for (auto&& comb : iter::combinations(bottles, i)) {
            sum = 0;
            for (auto&& j : comb ) {
               sum += j; 
            }
            if (sum == liters){
                num_comb++;
            }
        }
        if (num_comb > 0) return num_comb;
    }
    return num_comb;
}

int main(int argc, char** argv){
    std::string file_name, command;
    getopt(argc, argv, "");
    containers sizes;
    file_name = std::string(argv[optind]);
    std::ifstream input_file;
    input_file.open(file_name);
    std::getline(input_file, command);
    int total_size = std::stoi(command);
    while (std::getline(input_file, command)){
        sizes.push_back(std::stoi(command));
    }
    input_file.close();
    std::cout << "Number of ways to distribute is " << count_combinations(total_size, sizes) << std::endl;
}
