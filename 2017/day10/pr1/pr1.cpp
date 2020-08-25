#include <vector>
#include <string>
#include <fstream>
#include <utility>
#include <numeric>
#include <iostream>
#include <algorithm>
#include <unistd.h>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>

int main(int argc, char** argv){
    std::string file_name, des_reg, cond_reg;
    getopt(argc, argv, "");
    file_name = std::string(argv[optind]);
    std::ifstream input_file;
    input_file.open(file_name);
    std::string line; 
    std::getline(input_file, line);
    int size = std::stoi(line);
    std::getline(input_file, line);
    std::vector<std::string> lengths;
    boost::split(lengths, line, boost::is_any_of(","));
    std::vector<int> numbers = std::vector<int>(size, 0);
    std::iota(numbers.begin(), numbers.end(), 0);
    int current = 0, skip = 0, length;
    for (auto len: lengths){
        length = std::stoi(len);
        if (length > 1){
            std::rotate(numbers.begin(), numbers.begin()+current, numbers.end());
            std::reverse(numbers.begin(), numbers.begin()+length);
            std::rotate(numbers.rbegin(), numbers.rbegin()+current, numbers.rend());
        }
        current += length+skip;
        current %= size;
        skip++;
    }
    std::cout << numbers[0] * numbers[1] << std::endl;
}
