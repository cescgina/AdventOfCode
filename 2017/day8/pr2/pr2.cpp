#include <map>
#include <vector>
#include <string>
#include <fstream>
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
    std::vector<std::string> parsed_line;
    std::map<std::string, int> registers; 
    int max_val = 0;
    input_file.open(file_name);
    for (std::string line; std::getline(input_file, line);){
        boost::split(parsed_line, line, boost::is_any_of(" \t"), boost::token_compress_on);
        int offset = std::stoi(parsed_line[2]), cond_compare = std::stoi(parsed_line[6]);
        cond_reg = parsed_line[4];
        des_reg = parsed_line[0];
        if (parsed_line[1] == "dec") offset *= -1;
        bool result = false;
        if (parsed_line[5] == ">"){
            result = registers[cond_reg] > cond_compare;
        }
        else if (parsed_line[5] == "<"){
            result = registers[cond_reg] < cond_compare;
        }
        else if (parsed_line[5] == "<="){
            result = registers[cond_reg] <= cond_compare;
        }
        else if (parsed_line[5] == ">="){
            result = registers[cond_reg] >= cond_compare;
        }
        else if (parsed_line[5] == "=="){
            result = registers[cond_reg] == cond_compare;
        }
        else if (parsed_line[5] == "!="){
            result = registers[cond_reg] != cond_compare;
        }
        if (result){
            registers[des_reg] += offset;
            max_val = std::max(max_val, registers[des_reg]);
        }
    }
    input_file.close();
    std::cout << max_val << std::endl;
}
