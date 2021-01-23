#include <string>
#include <fstream>
#include <iostream>
#include <unordered_set>
#include <unistd.h>

typedef std::unordered_set<int> numbers;

bool contains(numbers& haystack, int needle){
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
    for (int num: all_numbers){
        if (contains(all_numbers, 2020-num)){
            std::cout << num*(2020-num) << std::endl; 
            return 0;
        }
    }
}
