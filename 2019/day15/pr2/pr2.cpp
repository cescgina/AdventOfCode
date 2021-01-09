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

long long execute_program(program& instructions, long long& output, long long& inputs, long long i, long long& relative_base){
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
            instructions[operand_a] = inputs;
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
            output = operand_a;
            if (DEBUG){
                std::cout << " " << operand_a << " " << i+1 << " " << instructions[i+1] << std::endl;
            }
            i += 2;
            return i;
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

struct state{
    program code;
    long long ip;
    long long relative_base;
    int x;
    int y;
    bool operator<(const state &other) const{
        return ip < other.ip;
    }
    std::pair<int, int> get_coords(){
        return std::make_pair(y, x);
    }
};

typedef std::pair<int, state> moves;

void update_xy(state& current, long long direction){
    switch (direction){
        case 1:
            current.y++;
            break;
        case 2:
            current.y--;
            break;
        case 3:
            current.x--;
            break;
        case 4:
            current.x++;
            break;
    }
}

typedef std::map<std::pair<int, int>, int> map_paths;
typedef std::unordered_set<std::pair<int, int>, boost::hash<std::pair<int, int>>> seen_paths;

bool is_blocked(state& current, long long input, map_paths& path){ 
    auto coords = current.get_coords();
    switch (input){
        case 1:
            coords.first++;
            break;
        case 2:
            coords.first--;
            break;
        case 3:
            coords.second--;
            break;
        case 4:
            coords.second++;
            break;
    }
    if (path.find(coords) == path.end()){
        return false;
    }
    return path[coords] == 0;
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
    long long output;
    long long ip = 0;
    long long relative_base = 0;
    map_paths path;
    path[std::make_pair(0, 0)] = 1;
    seen_paths visited;
    int oxygen_x, oxygen_y;
    std::priority_queue<moves, std::vector<moves>, std::greater<moves>> history;
    history.push(std::make_pair(0, state{code, ip, relative_base, 0, 0}));
    while (!history.empty()){
        int steps = history.top().first;
        state current_program = history.top().second;
        history.pop();
        if (visited.find(current_program.get_coords()) != visited.end()){
            continue;
        }
        visited.insert(current_program.get_coords());
        for (long long inputs = 1; inputs <= 4; inputs++){
            state cp_program = state{current_program};
            if (is_blocked(cp_program, inputs, path)) continue;
            cp_program.ip = execute_program(cp_program.code, output, inputs, cp_program.ip, cp_program.relative_base);
            if (cp_program.ip == -1) continue;
            if (output == 2){
                update_xy(cp_program, inputs);
                path[cp_program.get_coords()] = 2;
                oxygen_y = cp_program.y;
                oxygen_x = cp_program.x;
                continue;
            }
            else if (output == 1){
                update_xy(cp_program, inputs);
                path[cp_program.get_coords()] = 1;
            }
            else{
                int orig_x = cp_program.x;
                int orig_y = cp_program.y;
                update_xy(cp_program, inputs);
                path[cp_program.get_coords()] = 0;
                cp_program.x = orig_x;
                cp_program.y = orig_y;
            }
            history.push(std::make_pair(steps+1, cp_program));
        }
    }
    if (DEBUG){
        int prev_y = -1;
        for (auto& element: path){
            if (element.first.first != prev_y) std::cout << std::endl;
            prev_y = element.first.first;
            if (element.second == 2){
                std::cout << "O";
            }
            else if (element.second == 1){
                std::cout << ".";
            }
            else {
                std::cout << "#";
            }
        }
    }
    std::queue<std::tuple<int, int, int>> to_fill;
    int max_dist = 0;
    to_fill.push(std::make_tuple(0, oxygen_y, oxygen_x));
    while (!to_fill.empty()){
        int dist = std::get<0>(to_fill.front());
        int y = std::get<1>(to_fill.front());
        int x = std::get<2>(to_fill.front());
        to_fill.pop();
        max_dist = std::max(max_dist, dist);
        for (auto& coords: {std::make_pair(y-1, x), std::make_pair(y+1, x), std::make_pair(y, x-1), std::make_pair(y, x+1)}){
            if (path[coords] != 1) continue;
            path[coords] = 2;
            to_fill.push(std::make_tuple(dist+1, coords.first, coords.second));
        }
    }
    std::cout << "Time to fill the room is " << max_dist << " minutes" << std::endl;
}
