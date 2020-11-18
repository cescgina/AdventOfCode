#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <unordered_map>
#include <unistd.h>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>

typedef std::vector<int> registers;
typedef std::vector<std::string> instruction_info;
typedef std::vector<std::vector<int>> instructions_queue;

void apply_instruction(registers& reg, registers& instruction){
    int operation = instruction[0];
    int A = instruction[1], B = instruction[2], C = instruction[3];
    //addr
    if (operation == 0){
        reg[C] = reg[A]+reg[B];
    }
    //addi
    if (operation == 1){
        reg[C] = reg[A]+B;
    }

    //mulr
    if (operation == 2){
        reg[C] = reg[A]*reg[B];
    }
    //muli
    if (operation == 3){
        reg[C] = reg[A]*B;
    }

    //banr
    if (operation == 4){
        reg[C] = reg[A] & reg[B];
    }
    //bani
    if (operation == 5){
        reg[C] = reg[A] & B;
    }

    //borr
    if (operation == 6){
        reg[C] = reg[A] | reg[B];
    }
    //bori
    if (operation == 7){
        reg[C] = reg[A] | B;
    }

    //setr
    if (operation == 8){
        reg[C] = reg[A];
    }
    //seti
    if (operation == 9){
        reg[C] = A;
    }

    //gtir
    if (operation == 10){
        reg[C] = (A > reg[B]);
    } 
    //gtri
    if (operation == 11){
        reg[C] = (reg[A] > B);
    } 
    //gtrr
    if (operation == 12){
        reg[C] = (reg[A] > reg[B]);
    } 

    //eqir
    if (operation == 13){
        reg[C] = (A == reg[B]);
    } 
    //eqri
    if (operation == 14){
        reg[C] = (reg[A] == B);
    } 
    //eqrr
    if (operation == 15){
        reg[C] = (reg[A] == reg[B]);
    } 
}

int main(int argc, char** argv){
    std::string file_name, line = "laej";
    getopt(argc, argv, "");
    file_name = std::string(argv[optind]);
    std::ifstream input_file;
    registers register_array(6, 0);
    instructions_queue instructions;
    std::unordered_map<std::string, int> opcodes;
    opcodes["addr"] = 0;
    opcodes["addi"] = 1;
    opcodes["mulr"] = 2;
    opcodes["muli"] = 3;
    opcodes["banr"] = 4;
    opcodes["bani"] = 5;
    opcodes["borr"] = 6;
    opcodes["bori"] = 7;
    opcodes["setr"] = 8;
    opcodes["seti"] = 9;
    opcodes["gtir"] = 10;
    opcodes["gtri"] = 11;
    opcodes["gtrr"] = 12;
    opcodes["eqir"] = 13;
    opcodes["eqri"] = 14;
    opcodes["eqrr"] = 15;
    int ip = 0, ip_index = 0;
    input_file.open(file_name);
    for(std::string line;std::getline(input_file, line);) {
        if (line[0] == '#'){
            auto pos = line.find(" ");
            ip_index = std::stoi(line.substr(pos+1));
            continue;
        }
        instruction_info instruction_to_parse;
        boost::split(instruction_to_parse, line, boost::is_any_of(" "), boost::token_compress_on);
        registers instruction_final = {opcodes[instruction_to_parse[0]]};
        instruction_final.push_back(std::stoi(instruction_to_parse[1]));
        instruction_final.push_back(std::stoi(instruction_to_parse[2]));
        instruction_final.push_back(std::stoi(instruction_to_parse[3]));
        instructions.push_back(instruction_final);
    }
    input_file.close();
    int size_program = instructions.size();
    while (ip < size_program){
        register_array[ip_index] = ip;
        apply_instruction(register_array, instructions[ip]);
        ip = register_array[ip_index]+1;
    }
    std::cout << "Value of register 0 is " << register_array[0] << std::endl;
}
