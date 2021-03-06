#include <map>
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <unistd.h>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>

typedef std::vector<std::vector<std::string>> queue;

int main(int argc, char** argv){
    std::map<std::string, long long> registers = {{"a", 0}, {"b", 0}, {"c", 0}, {"d", 0}, {"e", 0}, {"f", 0}, {"g", 0}, {"h", 0}};
    std::string file_name, instruction, reg, reg2;
    getopt(argc, argv, "");
    file_name = std::string(argv[optind]);
    std::ifstream input_file;
    input_file.open(file_name);
    std::vector<std::string> parsed_line;
    queue instructions;
    for (std::string line; std::getline(input_file, line);){
        boost::split(parsed_line, line, boost::is_any_of(" "), boost::token_compress_on);
        instructions.push_back(parsed_line);
    }
    int i = 0, n = instructions.size(), val = 0, offset = 0, muls = 0;
    while (i < n){
        if (muls < 0){
            std::cout << "overflow happened!" << std::endl;
        }
        parsed_line = instructions[i];
        instruction = parsed_line[0];
        // std::cout << "Instruction " << i << " " << instruction << " " << parsed_line[1] << std::endl;
        if (instruction == "jnz"){
            reg = parsed_line[1];
            if (registers.find(reg) == registers.end()){
                val = std::stoi(reg);
            }
            else{
                val = registers[reg];
            }
            reg = parsed_line[2];
            if (registers.find(reg) == registers.end()){
                offset = std::stoi(reg);
            }
            else{
                offset = registers[reg];
            }
            if (val != 0) {
                i += offset;
                continue;
            }
        }
        else{
           if (instruction == "sub"){
               reg = parsed_line[1];
               reg2 = parsed_line[2];
                if (registers.find(reg2) == registers.end()){
                    val = std::stoi(reg2);
                }
                else{
                    val = registers[reg2];
                }
                registers[reg] -= val;
           }
           else if (instruction == "mul"){
               reg = parsed_line[1];
               reg2 = parsed_line[2];
                if (registers.find(reg2) == registers.end()){
                    val = std::stoi(reg2);
                }
                else{
                    val = registers[reg2];
                }
                registers[reg] *= val;
                muls++;
           }
           else if (instruction == "set"){
               reg = parsed_line[1];
               if (registers.find(parsed_line[2]) == registers.end()){
                   registers[reg] = std::stoi(parsed_line[2]);
               }
               else{
                   registers[reg] = registers[parsed_line[2]];
               }
           }
        }
        i++;
    }
    std::cout << muls << std::endl;
}
