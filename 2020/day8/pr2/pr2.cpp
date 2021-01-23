#include <queue>
#include <vector>
#include <deque>
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

typedef std::vector<std::string> instruction;
typedef std::vector<instruction> program;
bool DEBUG = false;

template<typename T>
bool contains(T& haystack, long long needle){
    return haystack.find(needle) != haystack.end();
}

struct Computer{
    program local_code;
    long long accumulator = 0;
    long long ip_local = 0;
    long long n = local_code.size();
    std::set<int> instructions_run = {};
    long long run(){
        while (ip_local < n){
            long long prev = ip_local;
            instructions_run.insert(ip_local);
            ip_local = execute_program();
            if (contains(instructions_run, ip_local)) return prev;
        }
        return -1;
    }

    long long execute_program(){
        instruction ins = local_code[ip_local];
        std::string opcode = ins[0];
        if (opcode == "acc"){
            // accumulate
            accumulator += std::stoll(ins[1]);
            if (DEBUG){
                std::cout << ip_local << " " << opcode << ins[1] << std::endl;
            }
            return ip_local+1;
        }
        else if (opcode == "jmp"){
            // jump
            if (DEBUG){
                std::cout << ip_local << " " << opcode << ins[1] << std::endl;
            }
            return ip_local+std::stoll(ins[1]);
        }
        else if (opcode == "nop"){
            // nop
            if (DEBUG){
                std::cout << ip_local << " " << opcode << ins[1] << std::endl;
            }
            return ip_local+1;
        }
        return -1;
    }
};

int main(int argc, char** argv){
    std::string file_name;
    getopt(argc, argv, "");
    file_name = std::string(argv[optind]);
    std::ifstream input_file;
    input_file.open(file_name);
    program code;
    std::vector<int> lines_to_change;
    int i = 0;
    for (std::string line; std::getline(input_file, line);){
        instruction before_reg;
        boost::split(before_reg, line, boost::is_any_of(", "), boost::token_compress_on);
        code.push_back(before_reg);
        if (before_reg[0] == "nop" or before_reg[0] == "jmp") lines_to_change.push_back(i);
        i++;
    }
    for (int to_change: lines_to_change){
        program code_cp(code);
        if (code_cp[to_change][0] == "jmp"){
            code_cp[to_change][0] = "nop";
        }
        else{
            code_cp[to_change][0] = "jmp";
        }
        Computer computer{code_cp};
        int return_val = computer.run();
        if (return_val == -1){
            std::cout << computer.accumulator << std::endl;
        }
        
    }
}
