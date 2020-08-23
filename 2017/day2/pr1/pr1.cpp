#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <unistd.h>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>

template<typename T>
void print_vector(const std::vector<T> vec){
    for (const auto x: vec){
        std::cout << x << "#";
    }
    std::cout << std::endl;
}

int main(int argc, char** argv){
    std::string file_name;
    getopt(argc, argv, "");
    file_name = std::string(argv[optind]);
    std::ifstream input_file;
    int checksum = 0;
    input_file.open(file_name);
    for (std::string line; std::getline(input_file, line);){
        std::vector<std::string> parsed_line;
        std::vector<int> numbers;
        boost::split(parsed_line, line, boost::is_any_of(" \t"), boost::token_compress_on);
        std::transform(parsed_line.begin(), parsed_line.end(), std::back_inserter(numbers), [](const std::string& s) -> int { return std::stoi(s);});
        const auto res = std::minmax_element(numbers.begin(), numbers.end());
        checksum += (*res.second-*res.first);
    }
    input_file.close();
    std::cout << checksum << std::endl;
}
