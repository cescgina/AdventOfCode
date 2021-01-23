#include <string>
#include <fstream>
#include <iostream>
#include <unordered_set>
#include <unordered_map>
#include <unistd.h>

typedef std::unordered_set<int> numbers;
typedef std::unordered_map<int, std::pair<int, int>> sums;

template<typename T>
bool contains(T& haystack, int needle){
    return haystack.find(needle) != haystack.end();
}

int main(int argc, char** argv){
    std::string file_name;
    getopt(argc, argv, "");
    file_name = std::string(argv[optind]);
    numbers all_numbers;
    std::ifstream input_file(file_name);
    for (std::string line; std::getline(input_file, line);){
        all_numbers.insert(std::stoi(line));
    }
    input_file.close();
    sums sum_pairs;
    for (int num: all_numbers){
        for (int num2: all_numbers){
            if (num == num2) continue;
            sum_pairs[num+num2] = std::make_pair(num, num2);
        }
    }
    for (int num: all_numbers){
        if (contains(sum_pairs, 2020-num)){
            auto pair_nums = sum_pairs[2020-num];
            std::cout << num*pair_nums.first*pair_nums.second << std::endl; 
            return 0;
        }
    }
}
