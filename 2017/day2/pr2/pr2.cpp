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

int find_division(const std::vector<int> vec){
    int n = vec.size();
    for (int i=0; i<n; i++){
        for (int j=0; j<n; j++){
            if (i == j) continue;
            if (vec[i] % vec[j] == 0){
                return vec[i]/vec[j];
            }
        }
    }
    return 0;
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
        checksum += find_division(numbers);
    }
    input_file.close();
    std::cout << checksum << std::endl;
}
