#include <cmath>
#include <vector>
#include <string>
#include <bitset>
#include <fstream>
#include <iostream>
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
    std::vector<std::string> parsed_line;
    std::getline(input_file, line);
    boost::split(parsed_line, line, boost::is_any_of(" "), boost::token_compress_on);
    long long A_value = std::stoll(parsed_line[4]), A_factor = 16807;
    std::getline(input_file, line);
    boost::split(parsed_line, line, boost::is_any_of(" "), boost::token_compress_on);
    long long B_value = std::stoll(parsed_line[4]), B_factor = 48271, modulo = 2147483647;
    int sum = 0;
    for (int i=0; i<40000000;i++){
        A_value = (A_value*A_factor) % modulo;
        B_value = (B_value*B_factor) % modulo;
        std::bitset<16> A_bits(A_value);
        std::bitset<16> B_bits(B_value);
        if (A_bits == B_bits) sum++;
    }
    std::cout << sum << std::endl;
}
