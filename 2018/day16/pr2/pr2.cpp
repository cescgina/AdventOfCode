#include <map>
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <unordered_set>
#include <unistd.h>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>

typedef std::vector<int> registers;
typedef std::vector<std::string> to_parse;
typedef std::map<int, std::unordered_set<std::string>> possible;
typedef std::map<int, std::string> opcodes;

void remove_occurrences(possible& possibilities, std::string& operation){
    for (auto& matching: possibilities){
        matching.second.erase(operation);    
    }
}

void assign_opcodes(possible& possibilities, opcodes& opcodes_map){
    int opcode;
    std::string operation;
    while(!possibilities.empty()){
        for (auto& matching: possibilities){
            if (matching.second.size() == 1){
                opcode = matching.first;
                operation = *matching.second.begin();
                break;
            }
        }
        possibilities.erase(opcode);
        opcodes_map[opcode] = operation;
        remove_occurrences(possibilities,operation);
    }
}

void count_opcodes(registers& before, registers& after, registers& instruction, possible& possibilities){
    // test all possible opcodes and see if they are consistent
    //addr
    if ((before[instruction[1]]+before[instruction[2]]) == after[instruction[3]]){
        possibilities[instruction[0]].insert("addr");
    }
    //addi
    if ((before[instruction[1]]+instruction[2]) == after[instruction[3]]){
        possibilities[instruction[0]].insert("addi");
    }

    //mulr
    if ((before[instruction[1]]*before[instruction[2]]) == after[instruction[3]]){
        possibilities[instruction[0]].insert("mulr");
    }
    //muli
    if ((before[instruction[1]]*instruction[2]) == after[instruction[3]]){
        possibilities[instruction[0]].insert("muli");
    }

    //banr
    if ((before[instruction[1]] & before[instruction[2]]) == after[instruction[3]]){
        possibilities[instruction[0]].insert("banr");
    }
    //bani
    if ((before[instruction[1]] & instruction[2]) == after[instruction[3]]){
        possibilities[instruction[0]].insert("bani");
    }

    //borr
    if ((before[instruction[1]] | before[instruction[2]]) == after[instruction[3]]){
        possibilities[instruction[0]].insert("borr");
    }
    //bori
    if ((before[instruction[1]] | instruction[2]) == after[instruction[3]]){
        possibilities[instruction[0]].insert("bori");
    }

    //setr
    if (before[instruction[1]] == after[instruction[3]]){
        possibilities[instruction[0]].insert("setr");
    }
    //seti
    if (instruction[1] == after[instruction[3]]){
        possibilities[instruction[0]].insert("seti");
    }

    //gtir
    if ((instruction[1] > before[instruction[2]]) == after[instruction[3]]){
        possibilities[instruction[0]].insert("gtir");
    } 
    //gtri
    if ((before[instruction[1]] > instruction[2]) == after[instruction[3]]){
        possibilities[instruction[0]].insert("gtri");
    } 
    //gtrr
    if ((before[instruction[1]] > before[instruction[2]]) == after[instruction[3]]){
        possibilities[instruction[0]].insert("gtrr");
    } 

    //eqir
    if ((instruction[1] == before[instruction[2]]) == after[instruction[3]]){
        possibilities[instruction[0]].insert("eqir");
    } 
    //eqri
    if ((before[instruction[1]] == instruction[2]) == after[instruction[3]]){
        possibilities[instruction[0]].insert("eqri");
    } 
    //eqrr
    if ((before[instruction[1]] == before[instruction[2]]) == after[instruction[3]]){
        possibilities[instruction[0]].insert("eqrr");
    } 
}

void apply_instruction(registers& reg, registers& instruction, opcodes& opcodes_map){
    std::string operation = opcodes_map[instruction[0]];
    //addr
    if (operation == "addr"){
        reg[instruction[3]] = reg[instruction[1]]+reg[instruction[2]];
    }
    //addi
    if (operation == "addi"){
        reg[instruction[3]] = reg[instruction[1]]+instruction[2];
    }

    //mulr
    if (operation == "mulr"){
        reg[instruction[3]] = reg[instruction[1]]*reg[instruction[2]];
    }
    //muli
    if (operation == "muli"){
        reg[instruction[3]] = reg[instruction[1]]*instruction[2];
    }

    //banr
    if (operation == "banr"){
        reg[instruction[3]] = reg[instruction[1]] & reg[instruction[2]];
    }
    //bani
    if (operation == "bani"){
        reg[instruction[3]] = reg[instruction[1]] & instruction[2];
    }

    //borr
    if (operation == "borr"){
        reg[instruction[3]] = reg[instruction[1]] | reg[instruction[2]];
    }
    //bori
    if (operation == "bori"){
        reg[instruction[3]] = reg[instruction[1]] | instruction[2];
    }

    //setr
    if (operation == "setr"){
        reg[instruction[3]] = reg[instruction[1]];
    }
    //seti
    if (operation == "seti"){
        reg[instruction[3]] = instruction[1];
    }

    //gtir
    if (operation == "gtir"){
        reg[instruction[3]] = (instruction[1] > reg[instruction[2]]);
    } 
    //gtri
    if (operation == "gtri"){
        reg[instruction[3]] = (reg[instruction[1]] > instruction[2]);
    } 
    //gtrr
    if (operation == "gtrr"){
        reg[instruction[3]] = (reg[instruction[1]] > reg[instruction[2]]);
    } 

    //eqir
    if (operation == "eqir"){
        reg[instruction[3]] = (instruction[1] == reg[instruction[2]]);
    } 
    //eqri
    if (operation == "eqri"){
        reg[instruction[3]] = (reg[instruction[1]] == instruction[2]);
    } 
    //eqrr
    if (operation == "eqrr"){
        reg[instruction[3]] = (reg[instruction[1]] == reg[instruction[2]]);
    } 
}
int main(int argc, char** argv){
    std::string file_name, line = "laej";
    getopt(argc, argv, "");
    file_name = std::string(argv[optind]);
    std::ifstream input_file;
    input_file.open(file_name);
    possible possible_matchings;
    for (int i=0; i < 16; i++){
        possible_matchings[i] = std::unordered_set<std::string>();
    }
    while(true){
        std::string before;
        std::getline(input_file, before);
        if (before.empty()) break;
        before = before.substr(9, 10);
        to_parse before_reg;
        registers before_register;
        boost::split(before_reg, before, boost::is_any_of(", "), boost::token_compress_on);
        std::transform(before_reg.begin(), before_reg.end(), std::back_inserter(before_register), [](std::string& c) -> int { return std::stoi(c);});

        std::string instruction;
        std::getline(input_file, instruction);
        to_parse instruction_arr;
        registers instructions;
        boost::split(instruction_arr, instruction, boost::is_any_of(", "), boost::token_compress_on);
        std::transform(instruction_arr.begin(), instruction_arr.end(), std::back_inserter(instructions), [](std::string c) -> int { return std::stoi(c);});

        std::string after;
        std::getline(input_file, after);
        after = after.substr(9, 10);
        to_parse after_reg;
        registers after_register;
        boost::split(after_reg, after, boost::is_any_of(", "), boost::token_compress_on);
        std::transform(after_reg.begin(), after_reg.end(), std::back_inserter(after_register), [](std::string c) -> int { return std::stoi(c);});

        // read empty line
        std::getline(input_file, line);
        count_opcodes(before_register, after_register, instructions, possible_matchings);
    }
    opcodes opcodes_mapping;
    assign_opcodes(possible_matchings, opcodes_mapping);
    registers program_reg = {0, 0, 0, 0};
    std::getline(input_file, line);
    for(std::string inst;std::getline(input_file, inst);){
        if (inst.empty()) break;
        to_parse inst_arr;
        registers instruction_final;
        boost::split(inst_arr, inst, boost::is_any_of(" "), boost::token_compress_on);
        std::transform(inst_arr.begin(), inst_arr.end(), std::back_inserter(instruction_final), [](std::string c) -> int { return std::stoi(c);});
        apply_instruction(program_reg, instruction_final, opcodes_mapping);

    }
    input_file.close();
    std::cout << "Final value for register 0 is " << program_reg[0] << std::endl;
}
