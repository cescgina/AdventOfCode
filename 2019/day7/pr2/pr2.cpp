#include <deque>
#include <vector>
#include <fstream>
#include <numeric>
#include <iostream>
#include <algorithm>
#include <unistd.h>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>

typedef std::vector<long long> registers;
typedef std::vector<std::string> to_parse;
bool DEBUG = false;

int execute_program(registers& instructions, std::deque<int>& inputs, int& output, int i){
    int n = instructions.size();
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
        if (DEBUG){
            std::cout << i << " " << instructions[i] << " " << opcode << " " << mode_a << " " << mode_b << " " << mode_c << ":";
        }
        if (opcode == 99){
            // halt
            if (DEBUG){
                std::cout << std::endl;
            }
            return -1;
        }
        else if (opcode == 1){
            // add
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
            if (DEBUG){
                std::cout << " " << operand_a << " " << operand_b << " " << operand_c << " " << instructions[operand_c] << std::endl;
            }
            i += 4;
        }
        else if (opcode == 2){
            // mul
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
            if (DEBUG){
                std::cout << " " << operand_a << " " << operand_b << " " << operand_c << " " << instructions[operand_c] << std::endl;
            }
            i += 4;
        }
        else if (opcode == 3){
            // store
            operand_a = instructions[i+1];
            int input = inputs.front();
            inputs.pop_front();
            instructions[operand_a] = input;
            if (DEBUG){
                std::cout << " " << operand_a << " " << instructions[operand_a] << std::endl;
            }
            i += 2;
        }
        else if (opcode == 4){
            // load
            if (mode_a == 1){
                operand_a = instructions[i+1];
            }
            else{
                operand_a = instructions[instructions[i+1]];
            }
            output = operand_a;
            if (DEBUG){
                std::cout << " " << operand_a << std::endl;
            }
            i += 2;
            return i;
        }
        else if (opcode == 5){
            // jump-if-not-zero
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
            if (operand_a != 0) {
                i = operand_b;
            }
            else{
                i += 3;
            }
            if (DEBUG){
                std::cout << " " << operand_a << " " << operand_b << std::endl;
            }
        }
        else if (opcode == 6){
            // jump-if-zero
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
            if (operand_a == 0) {
                i = operand_b;
            }
            else{
                i += 3;
            }
            if (DEBUG){
                std::cout << " " << operand_a << " " << operand_b << std::endl;
            }
        }
        else if (opcode == 7){
            // less-than
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
            instructions[operand_c] = operand_a<operand_b;
            if (DEBUG){
                std::cout << " " << operand_a << " " << operand_b << " " << operand_c << " " << instructions[operand_c] << std::endl;
            }
            i += 4;
        }
        else if (opcode == 8){
            // equals
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
            instructions[operand_c] = operand_a==operand_b;
            if (DEBUG){
                std::cout << " " << operand_a << " " << operand_b << " " << operand_c << " " << instructions[operand_c] << std::endl;
            }
            i += 4;
        }
        else{
            std::cout << "Something wierd found " << i << " " << instructions[i] << " " << opcode << std::endl;
        }
    }
    return -2;
}
int main(int argc, char** argv){
    std::string file_name;
    getopt(argc, argv, "");
    file_name = std::string(argv[optind]);
    std::ifstream input_file;
    input_file.open(file_name);
    std::string line; 
    std::getline(input_file, line);
    to_parse before_reg;
    registers instructions_orig;
    boost::split(before_reg, line, boost::is_any_of(", "), boost::token_compress_on);
    std::transform(before_reg.begin(), before_reg.end(), std::back_inserter(instructions_orig), [](std::string& c) -> int { return std::stoi(c);});
    std::vector<int> phases(5);
    std::iota(phases.begin(), phases.end(), 5);
    int max_output = 0;
    size_t N = phases.size();
    do {
        std::vector<registers> copies(5);
        for (size_t jj = 0; jj < 5; jj++){
            copies[jj] = instructions_orig;
        }
        std::vector<std::deque<int>> memory(5);
        std::vector<int> instruction_pointers(5, 0);
        int output = 0;
        for (size_t jj = 0; jj < 5; jj++){
            memory[jj].clear();
            memory[jj].push_front(phases[jj]);
        }
        memory[0].push_back(0);
        int index = 0;
        while (true){
            int ip = execute_program(copies[index], memory[index], output, instruction_pointers[index]);
            if (ip == -1 and index == 4){
                max_output = std::max(output, max_output);
                break;
            }
            memory[(index+1) % N].push_back(output);
            instruction_pointers[index] = ip;
            index = (index+1)%N;
        }
    } while(std::next_permutation(phases.begin(), phases.end()));
    std::cout << "Max output is " << max_output << std::endl;
}
