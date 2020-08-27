#include <map>
#include <queue>
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <unistd.h>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>

typedef std::vector<std::vector<std::string>> queue_inst;

void exec_instructions(const queue_inst& inst, int& index, std::queue<int>& msg_send, std::queue<int>& msg_recv, std::map<std::string, long long>& registers, int& counts){
    int val = 0, offset = 0;
    std::string instruction, reg, reg2;
    std::vector<std::string> parsed_line;
    parsed_line = inst[index];
    instruction = parsed_line[0];
    if (instruction == "jgz"){
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
        if (val > 0) {
            index += offset;
            return;
        }
    }
    else{
       if (instruction == "add"){
           reg = parsed_line[1];
           reg2 = parsed_line[2];
            if (registers.find(reg2) == registers.end()){
                val = std::stoi(reg2);
            }
            else{
                val = registers[reg2];
            }
            registers[reg] += val;
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
       }
       else if (instruction == "snd"){
           reg2 = parsed_line[1];
            if (registers.find(reg2) == registers.end()){
                val = std::stoi(reg2);
            }
            else{
                val = registers[reg2];
            }
            counts++;
            msg_send.push(val);
       }
       else if (instruction == "rcv"){
            reg = parsed_line[1];
            if (msg_recv.empty()){
                registers["WAIT"] = 1;
                return;
            }
            registers[reg] = msg_recv.front();
            msg_recv.pop();
       }
       else if (instruction == "mod"){
           reg = parsed_line[1];
           reg2 = parsed_line[2];
            if (registers.find(reg2) == registers.end()){
                val = std::stoi(reg2);
            }
            else{
                val = registers[reg2];
            }
            registers[reg] %= val;
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
   index++;
}

int main(int argc, char** argv){
    std::map<std::string, long long> registers0;
    std::map<std::string, long long> registers1;
    registers0["p"] = 0;
    registers1["p"] = 1;
    std::queue<int> msg, msg1;
    std::string file_name;
    getopt(argc, argv, "");
    file_name = std::string(argv[optind]);
    std::ifstream input_file;
    input_file.open(file_name);
    std::vector<std::string> parsed_line;
    queue_inst instructions;
    for (std::string line; std::getline(input_file, line);){
        boost::split(parsed_line, line, boost::is_any_of(" "), boost::token_compress_on);
        instructions.push_back(parsed_line);
    }
    int i = 0, n = instructions.size(), i1 = 0, count_send = 0, count_send0 = 0;
    while (i < n and i1 < n){
        if (registers0["WAIT"] == 1 and registers1["WAIT"] == 1){
            break;
        }
        exec_instructions(instructions, i, msg, msg1, registers0, count_send0);
        exec_instructions(instructions, i1, msg1, msg, registers1, count_send);
    }
    std::cout << count_send << std::endl;
}
