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
    std::map<std::string, int> registers;
    registers["a"] = 1;
    registers["b"] = 0;
    std::string file_name, instruction, reg;
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
    int i = 0, n = instructions.size();
    while (i < n){
        parsed_line = instructions[i];
        instruction = parsed_line[0];
        std::cout << "Instruction " << i << " " << instruction << " " << parsed_line[1] << std::endl;
        if (instruction.substr(0, 2) == "ji"){
            reg = parsed_line[1].substr(0, parsed_line[1].size()-1);
            if (instruction == "jie"){
                if (registers[reg] % 2 == 0){
                   i += std::stoi(parsed_line[2]);
                   continue;
                }

            }
            else if (instruction == "jio"){
                if (registers[reg] == 1){
                   i += std::stoi(parsed_line[2]);
                   continue;
                }
            }
        }
        else{
           if (instruction == "hlf"){
               registers[parsed_line[1]] /= 2;
           }
           else if (instruction == "tpl"){
               registers[parsed_line[1]] *= 3;
           }
           else if (instruction == "inc"){
               registers[parsed_line[1]]++;
           }
           else if (instruction == "jmp"){
               i += std::stoi(parsed_line[1]);
               continue;
           }
        }
        i++;
    }
    std::cout << "Final value of register b " << registers["b"] << std::endl;
    std::cout << "Final value of register a " << registers["a"] << std::endl;
}
