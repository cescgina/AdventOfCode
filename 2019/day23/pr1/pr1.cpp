#include <queue>
#include <vector>
#include <fstream>
#include <numeric>
#include <iostream>
#include <algorithm>
#include <unordered_map>
#include <unordered_set>
#include <unistd.h>
#include <boost/functional/hash.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>

typedef std::vector<long long> registers;
typedef std::vector<std::string> to_parse;
typedef std::unordered_map<unsigned long long, long long> program;
bool DEBUG = false;


std::ostream& operator<<(std::ostream& out, std::queue<long long> in){
    while(not in.empty()){
        std::cout << in.front() << ",";
        in.pop();
    }
    return out;
}

std::ostream& operator<<(std::ostream& out, program& code){
    size_t i = 0;
    while(code.find(i) != code.end()){
        std::cout << code[i] << ",";
        i++;
    }
    return out;
}

long long execute_program(program& instructions, std::queue<long long>& output, std::queue<long long>& inputs, long long i, long long& relative_base){
    long long n = instructions.size();
    while (i < n){
        long long ins = instructions[i];
        int mode_a, mode_b, mode_c;
        int opcode = ins % 100;
        mode_c = ins / 10000;
        ins %= 10000;
        mode_b = ins / 1000;
        ins %= 1000;
        mode_a = ins / 100;
        long long operand_a, operand_b, operand_c;
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
            else if (mode_a == 2){
                operand_a = instructions[instructions[i+1]+relative_base];
            }
            else{
                operand_a = instructions[instructions[i+1]];
            }
            if (mode_b == 1){
                operand_b = instructions[i+2];
            }
            else if (mode_b == 2){
                operand_b = instructions[instructions[i+2]+relative_base];
            }
            else{
                operand_b = instructions[instructions[i+2]];
            }
            if (mode_c == 2){
                operand_c = instructions[i+3]+relative_base;
            }
            else{
                operand_c = instructions[i+3];
            }
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
            else if (mode_a == 2){
                operand_a = instructions[instructions[i+1]+relative_base];
            }
            else{
                operand_a = instructions[instructions[i+1]];
            }
            if (mode_b == 1){
                operand_b = instructions[i+2];
            }
            else if (mode_b == 2){
                operand_b = instructions[instructions[i+2]+relative_base];
            }
            else{
                operand_b = instructions[instructions[i+2]];
            }
            if (mode_c == 2){
                operand_c = instructions[i+3]+relative_base;
            }
            else{
                operand_c = instructions[i+3];
            }
            instructions[operand_c] = operand_a*operand_b;
            if (DEBUG){
                std::cout << " " << operand_a << " " << operand_b << " " << operand_c << " " << instructions[operand_c] << std::endl;
            }
            i += 4;
        }
        else if (opcode == 3){
            // store
            if (mode_a == 2){
                operand_a = instructions[i+1]+relative_base;
            }
            else{
                operand_a = instructions[i+1];
            }

            if (inputs.empty()){
                return i;
            }
            instructions[operand_a] = inputs.front();
            inputs.pop();
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
            else if (mode_a == 2){
                operand_a = instructions[instructions[i+1]+relative_base];
            }
            else{
                operand_a = instructions[instructions[i+1]];
            }
            output.push(operand_a);
            if (DEBUG){
                std::cout << " " << operand_a << " " << i+1 << " " << instructions[i+1] << std::endl;
            }
            i += 2;
            if (output.size() == 3) return i;
        }
        else if (opcode == 5){
            // jump-if-not-zero
            if (mode_a == 1){
                operand_a = instructions[i+1];
            }
            else if (mode_a == 2){
                operand_a = instructions[instructions[i+1]+relative_base];
            }
            else{
                operand_a = instructions[instructions[i+1]];
            }
            if (mode_b == 1){
                operand_b = instructions[i+2];
            }
            else if (mode_b == 2){
                operand_b = instructions[instructions[i+2]+relative_base];
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
            else if (mode_a == 2){
                operand_a = instructions[instructions[i+1]+relative_base];
            }
            else{
                operand_a = instructions[instructions[i+1]];
            }
            if (mode_b == 1){
                operand_b = instructions[i+2];
            }
            else if (mode_b == 2){
                operand_b = instructions[instructions[i+2]+relative_base];
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
            else if (mode_a == 2){
                operand_a = instructions[instructions[i+1]+relative_base];
            }
            else{
                operand_a = instructions[instructions[i+1]];
            }
            if (mode_b == 1){
                operand_b = instructions[i+2];
            }
            else if (mode_b == 2){
                operand_b = instructions[instructions[i+2]+relative_base];
            }
            else{
                operand_b = instructions[instructions[i+2]];
            }
            if (mode_c == 2){
                operand_c = instructions[i+3]+relative_base;
            }
            else{
                operand_c = instructions[i+3];
            }
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
            else if (mode_a == 2){
                operand_a = instructions[instructions[i+1]+relative_base];
            }
            else{
                operand_a = instructions[instructions[i+1]];
            }
            if (mode_b == 1){
                operand_b = instructions[i+2];
            }
            else if (mode_b == 2){
                operand_b = instructions[instructions[i+2]+relative_base];
            }
            else{
                operand_b = instructions[instructions[i+2]];
            }
            if (mode_c == 2){
                operand_c = instructions[i+3]+relative_base;
            }
            else{
                operand_c = instructions[i+3];
            }
            instructions[operand_c] = operand_a==operand_b;
            if (DEBUG){
                std::cout << " " << operand_a << " " << operand_b << " " << operand_c << " " << instructions[operand_c] << std::endl;
            }
            i += 4;
        }
        else if (opcode == 9){
            // adjust relative base
            if (mode_a == 1){
                operand_a = instructions[i+1];
            }
            else if (mode_a == 2){
                operand_a = instructions[instructions[i+1]+relative_base];
            }
            else{
                operand_a = instructions[instructions[i+1]];
            }
            relative_base += operand_a;
            if (DEBUG){
                std::cout << " " << operand_a << " " << relative_base << std::endl;
            }
            i += 2;
        }
        else{
            std::cout << "Something wierd found " << i << " " << instructions[i] << " " << opcode << std::endl;
        }
    }
    return -1;
}

struct Computer{
    program local_code;
    long long index;
    std::queue<long long> outputq = {};
    std::queue<long long> inputq = {};
    long long ip_local = 0;
    long long relative_base_local = 0;
    void run(){
        ip_local = execute_program(local_code, outputq, inputq, ip_local, relative_base_local);
    }
    void send_value(long long value){
        inputq.push(value);
    }
    long long get_value(){
        long long value = outputq.front();
        outputq.pop();
        return value;
    }
    long long peek_input(){
        return inputq.front();
    }
    bool is_empty(){
        return inputq.empty();
    }
};

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
    program code;
    boost::split(before_reg, line, boost::is_any_of(", "), boost::token_compress_on);
    std::transform(before_reg.begin(), before_reg.end(), std::back_inserter(instructions), [](std::string& c) -> long long { return std::stoll(c);});
    for (size_t i = 0; i<instructions.size(); i++){
        code[i] = instructions[i];
    }
    std::vector<Computer> network;
    for (long long i=0; i<50; i++){
        network.push_back(Computer{code, i});
        network[i].send_value(i);
        network[i].send_value(-1);
    } 
    while (true){
        for (auto& computer: network){
            if (computer.is_empty()){
                computer.send_value(-1);
            }
            computer.run();
            if (computer.outputq.size() < 3) continue;
            long long address = computer.get_value();
            long long x = computer.get_value();
            long long y = computer.get_value();
            
            if (address == 255){
                std::cout << y << std::endl;
                return 0;
            }
            if (address > 49) continue;
            network[address].send_value(x);
            network[address].send_value(y);
        }
    }
}
