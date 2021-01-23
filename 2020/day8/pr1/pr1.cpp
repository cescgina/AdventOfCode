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
            if (contains(instructions_run, ip_local)) return accumulator;
            instructions_run.insert(ip_local);
            ip_local = execute_program();
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
    for (std::string line; std::getline(input_file, line);){
        instruction before_reg;
        boost::split(before_reg, line, boost::is_any_of(", "), boost::token_compress_on);
        code.push_back(before_reg);
    }
    Computer computer{code};
    std::cout << computer.run() << std::endl;
}
