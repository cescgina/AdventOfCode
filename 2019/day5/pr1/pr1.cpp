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
    int i = 0;
    int n = instructions.size();
    int input = 1, output = 0;
    while (i < n){
        int ins = instructions[i];
        int mode_a, mode_b, mode_c;
        int opcode = ins % 100;
        mode_c = ins / 10000;
        ins %= 10000;
        mode_b = ins / 1000;
        ins %= 1000;
        mode_a = ins / 100;
        int operand_a, operand_b, operand_c;
        if (opcode == 99){
            break;
        }
        else if (opcode == 1){
            if (mode_a == 1){
                operand_a = instructions[i+1];
            }
            else{
                operand_a = instructions[instructions[i+1]];
            }
            if (mode_b == 1){
                operand_b = instructions[i+2];
            }
            else{
                operand_b = instructions[instructions[i+2]];
            }
            operand_c = instructions[i+3];
            instructions[operand_c] = operand_a+operand_b;
            i += 4;
        }
        else if (opcode == 2){
            if (mode_a == 1){
                operand_a = instructions[i+1];
            }
            else{
                operand_a = instructions[instructions[i+1]];
            }
            if (mode_b == 1){
                operand_b = instructions[i+2];
            }
            else{
                operand_b = instructions[instructions[i+2]];
            }
            operand_c = instructions[i+3];
            instructions[operand_c] = operand_a*operand_b;
            i += 4;
        }
        else if (opcode == 3){
            operand_a = instructions[i+1];
            instructions[operand_a] = input;
            i += 2;
        }
        else if (opcode == 4){
            if (mode_a == 1){
                operand_a = instructions[i+1];
            }
            else{
                operand_a = instructions[instructions[i+1]];
            }
            output = operand_a;
            i += 2;
        }
        else{
            std::cout << "Something wierd found " << i << " " << instructions[i] << " " << opcode << std::endl;
        }
    }
    std::cout << "Value is " << output << std::endl;
}
