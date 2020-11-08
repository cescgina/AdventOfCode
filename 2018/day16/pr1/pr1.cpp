#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <unistd.h>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>

typedef std::vector<int> registers;
typedef std::vector<std::string> to_parse;

int count_opcodes(registers& before, registers& after, registers& instruction){
    int count = 0;
    // test all possible opcodes and see if they are consistent

    //addr
    if ((before[instruction[1]]+before[instruction[2]]) == after[instruction[3]]) count++;
    //addi
    if ((before[instruction[1]]+instruction[2]) == after[instruction[3]]) count++;

    //mulr
    if ((before[instruction[1]]*before[instruction[2]]) == after[instruction[3]]) count++;
    //muli
    if ((before[instruction[1]]*instruction[2]) == after[instruction[3]]) count++;

    //banr
    if ((before[instruction[1]] & before[instruction[2]]) == after[instruction[3]]) count++;
    //bani
    if ((before[instruction[1]] & instruction[2]) == after[instruction[3]]) count++;

    //borr
    if ((before[instruction[1]] | before[instruction[2]]) == after[instruction[3]]) count++;
    //bori
    if ((before[instruction[1]] | instruction[2]) == after[instruction[3]]) count++;

    //setr
    if (before[instruction[1]] == after[instruction[3]]) count++;
    //seti
    if (instruction[1] == after[instruction[3]]) count++;

    //gtir
    if ((instruction[1] > before[instruction[2]]) == after[instruction[3]]) count++; 
    //gtri
    if ((before[instruction[1]] > instruction[2]) == after[instruction[3]]) count++; 
    //gtrr
    if ((before[instruction[1]] > before[instruction[2]]) == after[instruction[3]]) count++; 

    //eqir
    if ((instruction[1] == before[instruction[2]]) == after[instruction[3]]) count++; 
    //eqri
    if ((before[instruction[1]] == instruction[2]) == after[instruction[3]]) count++; 
    //eqrr
    if ((before[instruction[1]] == before[instruction[2]]) == after[instruction[3]]) count++; 
    return count;
}

int main(int argc, char** argv){
    std::string file_name, line = "laej";
    getopt(argc, argv, "");
    file_name = std::string(argv[optind]);
    std::ifstream input_file;
    input_file.open(file_name);
    int multiple_op = 0;
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
        if (count_opcodes(before_register, after_register, instructions) > 2) multiple_op++;
    }
    input_file.close();
    std::cout << "There are " << multiple_op << " instructions that behave like 3 or more opcodes" << std::endl;
}
