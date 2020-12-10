#include <vector>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <unistd.h>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>

typedef std::vector<long long> registers;
typedef std::vector<std::string> to_parse;

int main(int argc, char** argv){
    std::string file_name;
    getopt(argc, argv, "");
    file_name = std::string(argv[optind]);
    std::ifstream input_file;
    input_file.open(file_name);
    std::string line; 
    std::getline(input_file, line);
    to_parse before_reg;
    registers instructions;
    boost::split(before_reg, line, boost::is_any_of(", "), boost::token_compress_on);
    std::transform(before_reg.begin(), before_reg.end(), std::back_inserter(instructions), [](std::string& c) -> int { return std::stoi(c);});
    instructions[1] = 12;
    instructions[2] = 2;
    int i = 0;
    int n = instructions.size();
    while (i < n){
        if (instructions[i] == 99){
            break;
        }
        else if (instructions[i] == 1){
            instructions[instructions[i+3]] = instructions[instructions[i+1]]+instructions[instructions[i+2]];
        }
        else if (instructions[i] == 2){
            instructions[instructions[i+3]] = instructions[instructions[i+1]]*instructions[instructions[i+2]];
        }
        else{
            std::cout << "Something wierd found " << i << " " << instructions[i]<< std::endl;
        }
        i += 4;
    }
    std::cout << "Value is " << instructions[0] << std::endl;
}
