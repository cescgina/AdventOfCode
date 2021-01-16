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
            //return i;
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
            return -1;
        }
    }
    return -1;
}

std::pair<int, int> calculate_extremes(int row, program& to_run){
    std::queue<long long> output;
    long long ip = 0;
    long long relative_base = 0;
    std::queue<long long> inputs;
    int min_pos = row;
    int max_pos = 0;
    bool beam = false;
    for (int x=row/2; x < row*2; x++){
        inputs.push(x);
        inputs.push(row);
        program cp_code(to_run);
        execute_program(cp_code, output, inputs, ip, relative_base);
        int out = output.front();
        output.pop();
        if (out == 1){
            min_pos = std::min(min_pos, x);
            max_pos = std::max(max_pos, x);
            beam = true;
        }
        else{
            if (beam) break;
        }
    }
    return std::make_pair(min_pos, max_pos);
}

std::pair<int, int> get_extremes(int row, std::unordered_map<int, std::pair<int, int>>& cache_points, program& code){
    if (cache_points.find(row) == cache_points.end()){
        cache_points[row] = calculate_extremes(row, code);
    }
    return cache_points[row];
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
    std::vector<char> chars = {'.', '#'};
    std::unordered_map<int, std::pair<int, int>> cache_points;
    int limit = 2000;
    int start = 855;
    for (int i=start; i<limit; i++){
        std::pair<int, int> init_limits = get_extremes(i, cache_points, code);
        int end = init_limits.second;
        int st = end-100+1;
        if (st < init_limits.first) continue;
        int row = i+1, count = 1;
        while (count < 100){
            std::pair<int, int> beam_limits = get_extremes(row, cache_points, code);
            if (st < beam_limits.first) break;
            count++;
            row++;
        }
        if (count == 100){
            std::cout << "Value is " << st*10000+i << " coords " << i << "," << st << std::endl;
            return 0;
        }
    }
}
