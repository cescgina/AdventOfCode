#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <unistd.h>

int main(int argc, char** argv){
    std::string file_name, line;
    getopt(argc, argv, "");
    long long total = 0;
    file_name = std::string(argv[optind]);
    std::ifstream input_file(file_name);
    std::vector<int> numbers;
    int number = 0, prev = 10000000;
    for (std::string line; std::getline(input_file, line);){
        number = std::stoi(line);
        numbers.push_back(number);
    }
    prev = numbers[0]+numbers[1]+numbers[2];
    for (size_t i = 3; i< numbers.size();i++){
        number = numbers[i]+prev-numbers[i-3];
        total += (number > prev);
        prev = number;
    }
    input_file.close();
    std::cout << total << std::endl;
}
