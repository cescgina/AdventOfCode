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

void translate_instructions(const std::string& input, std::queue<long long>& inputs){
    for (char c: input){
        inputs.push(c);
    }
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

typedef std::unordered_map<std::pair<int, int>, int, boost::hash<std::pair<int, int>>> map_paths;

bool is_intersection(map_paths& mapp, int x, int y){
    bool intersection = true;
    intersection &= (mapp[std::make_pair(y-1, x)] == 35 or mapp[std::make_pair(y-1, x)] == 43 or mapp[std::make_pair(y-1, x)] == 45);
    intersection &= (mapp[std::make_pair(y, x-1)] == 35 or mapp[std::make_pair(y, x-1)] == 43 or mapp[std::make_pair(y, x-1)] == 45);
    intersection &= (mapp[std::make_pair(y+1, x)] == 35 or mapp[std::make_pair(y+1, x)] == 43 or mapp[std::make_pair(y+1, x)] == 45);
    intersection &= (mapp[std::make_pair(y, x+1)] == 35 or mapp[std::make_pair(y, x+1)] == 43 or mapp[std::make_pair(y, x+1)] == 45);
    return intersection;
}

std::string find_turn(int& direction, map_paths& mapp, int x, int y){
    if (direction == 0){
        // north
        auto coords_right = std::make_pair(y, x+1);
        auto coords_left = std::make_pair(y, x-1);
        if (mapp[coords_right] == 43 or mapp[coords_right] == 35) {
            direction = 1;
            return "R";
        }
        else if (mapp[coords_left] == 43 or mapp[coords_left] == 35){
            direction = 3;
            return "L";
        }
    }
    else if (direction == 1){
        // east
        auto coords_right = std::make_pair(y+1, x);
        auto coords_left = std::make_pair(y-1, x);
        if (mapp[coords_right] == 43 or mapp[coords_right] == 35) {
            direction = 2;
            return "R";
        }
        else if (mapp[coords_left] == 43 or mapp[coords_left] == 35){
            direction = 0;
            return "L";
        }
    }
    else if (direction == 2){
        // south
        auto coords_right = std::make_pair(y, x-1);
        auto coords_left = std::make_pair(y, x+1);
        if (mapp[coords_right] == 43 or mapp[coords_right] == 35) {
            direction = 3;
            return "R";
        }
        else if (mapp[coords_left] == 43 or mapp[coords_left] == 35){
            direction = 1;
            return "L";
        }
    }
    else if (direction == 3){
        // west
        auto coords_right = std::make_pair(y-1, x);
        auto coords_left = std::make_pair(y+1, x);
        if (mapp[coords_right] == 43 or mapp[coords_right] == 35) {
            direction = 0;
            return "R";
        }
        else if (mapp[coords_left] == 43 or mapp[coords_left] == 35){
            direction = 2;
            return "L";
        }
    }
    return "";
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
    registers instructions;
    program code;
    boost::split(before_reg, line, boost::is_any_of(", "), boost::token_compress_on);
    std::transform(before_reg.begin(), before_reg.end(), std::back_inserter(instructions), [](std::string& c) -> long long { return std::stoll(c);});
    for (size_t i = 0; i<instructions.size(); i++){
        code[i] = instructions[i];
    }
    std::queue<long long> output;
    long long ip = 0;
    long long relative_base = 0;
    std::queue<long long> inputs;
    std::string input_instructions = "NOT A J\nNOT B T\nAND T J\nNOT C J\nAND T J\nAND D J\nNOT A T\nOR T J\nNOT C T\nAND D T\nOR T J\nWALK\n";
    translate_instructions(input_instructions, inputs);
    execute_program(code, output, inputs, ip, relative_base);
    
    while (not output.empty()){
        if (output.front() > 255) std::cout << output.front() << std::endl;
        else std::cout << char(output.front());
        output.pop();
    }
}
