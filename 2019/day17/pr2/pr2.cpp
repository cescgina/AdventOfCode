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
    program orig_code(code);
    execute_program(code, output, inputs, ip, relative_base);
    map_paths paths;
    int x = 0, y = 0;
    int max_x = x, max_y = y;
    int start_x = x, start_y = y;
    int direction = 0;
    while (not output.empty()){
        int value = output.front();
        output.pop();
        if (value == 10){
            y++;
            x = 0;
            continue;
        }
        if (value == 60){
            start_x = x;
            start_y = y;
            direction = 3;
        }
        else if(value == 62){
            start_x = x;
            start_y = y;
            direction = 1;
        }
        else if (value == 94){
            start_x = x;
            start_y = y;
            direction = 0;
        }
        else if (value == 76){
            start_x = x;
            start_y = y;
            direction = 2;
        }
        max_x = std::max(max_x, x);
        max_y = std::max(max_y, y);
        paths[std::make_pair(y, x)] = value;
        x++;
    }
    x = start_x;
    y = start_y;
    int steps = 0;
    // intersections as +, ascii 43
    // visited locations as -, ascii 45
    std::vector<std::string> pathway;
    while (true){
        if (direction == 0){
            // north
            auto coords = std::make_pair(y-1, x);
            if (paths[coords] == 43 or paths[coords] == 35) {
                paths[std::make_pair(y, x)] = 45;
                if (is_intersection(paths, x, y)) paths[std::make_pair(y, x)] = 43;
                steps++;
                y--;
                continue;
            }
        }
        else if (direction == 1){
            // east
            auto coords = std::make_pair(y, x+1);
            if (paths[coords] == 43 or paths[coords] == 35) {
                paths[std::make_pair(y, x)] = 45;
                if (is_intersection(paths, x, y)) paths[std::make_pair(y, x)] = 43;
                x++;
                steps++;
                continue;
            }
        }
        else if (direction == 2){
            // south
            auto coords = std::make_pair(y+1, x);
            if (paths[coords] == 43 or paths[coords] == 35) {
                paths[std::make_pair(y, x)] = 45;
                if (is_intersection(paths, x, y)) paths[std::make_pair(y, x)] = 43;
                y++;
                steps++;
                continue;
            }
        }
        else if (direction == 3){
            // west
            auto coords = std::make_pair(y, x-1);
            if (paths[coords] == 43 or paths[coords] == 35) {
                paths[std::make_pair(y, x)] = 45;
                if (is_intersection(paths, x, y)) paths[std::make_pair(y, x)] = 43;
                x--;
                steps++;
                continue;
            }
        }
        if (steps != 0){
            pathway.push_back(std::to_string(steps));
            steps = 0;
        }
        std::string turn = find_turn(direction, paths, x, y);
        if (turn == "") break;
        pathway.push_back(turn);
    }
    for (auto c: pathway){
        std::cout << c <<",";
    }
    std::cout << std::endl;
    // found the funcitons manually, see path.txt file 
    std::vector<char> A = {'R',',','4',',','L',',','1','2',',','L',',','8',',','R',',','4'};
    std::vector<char> B = {'L',',','8',',','R',',','1','0',',','R',',','1','0',',','R',',','6'};
    std::vector<char> C = {'R',',','4',',','R',',','1','0',',','L',',','1','2'};
    std::vector<char> routine = {'A',',','B',',','A',',','C',',','A',',','B',',','A',',','C',',','B',',','C'};
    output = {};
    ip = 0;
    relative_base = 0;
    inputs = {};
    orig_code[0] = 2;
    for (size_t i = 0; i < routine.size(); i++){
        inputs.push((int)routine[i]);
    }
    inputs.push(10);

    for (size_t i = 0; i < A.size(); i++){
        inputs.push((int)A[i]);
    }
    inputs.push(10);


    for (size_t i = 0; i < B.size(); i++){
        inputs.push((int)B[i]);
    }
    inputs.push(10);


    for (size_t i = 0; i < C.size(); i++){
        inputs.push((int)C[i]);
    }
    inputs.push(10);

    // no video feed
    inputs.push(110);
    inputs.push(10);

    std::cout << "Calling program" << std::endl;
    execute_program(orig_code, output, inputs, ip, relative_base);
    
    while (not output.empty()){
        if (output.front() > 255) std::cout << output.front() << std::endl;
        else std::cout << char(output.front());
        output.pop();
    }
}
