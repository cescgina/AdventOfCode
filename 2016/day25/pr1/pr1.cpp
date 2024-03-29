#include <map>
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <unistd.h>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>

typedef std::vector<std::vector<std::string>> queue;

bool is_pattern(const std::vector<int>& arr_val){
    for (size_t i=0; i<arr_val.size();i++){
        if (i % 2 == 0 and arr_val[i] == 1){
            return false;
        }
        if (i % 2 != 0 and arr_val[i] == 0){
            return false;
        }
    }
    return true;
}

int main(int argc, char** argv){
    std::map<std::string, int> registers;
    registers["a"] = 0;
    registers["b"] = 0;
    registers["c"] = 0;
    registers["d"] = 0;
    std::string file_name, instruction, reg;
    getopt(argc, argv, "");
    file_name = std::string(argv[optind]);
    std::ifstream input_file;
    input_file.open(file_name);
    std::vector<std::string> parsed_line;
    std::map<std::string, std::string> changes;
    changes["cpy"] = "jnz";
    changes["inc"] = "dec";
    changes["dec"] = "inc";
    changes["jnz"] = "cpy";
    changes["tgl"] = "inc";
    queue instructions;
    for (std::string line; std::getline(input_file, line);){
        boost::split(parsed_line, line, boost::is_any_of(" "), boost::token_compress_on);
        instructions.push_back(parsed_line);
    }
    int i = 0, n = instructions.size(), val = 0, offset = 0, a_val = 0;
    while (true){
        i = 0;
        std::vector<int> values;
        registers["a"] = a_val;
        while (i < n){
            parsed_line = instructions[i];
            instruction = parsed_line[0];
            if (values.size() == 1000){
                if (is_pattern(values)){
                    std::cout << a_val << std::endl;
                    return 0;
                }
                break;
            }
            if (instruction == "out"){
                reg = parsed_line[1];
                if (registers.find(reg) == registers.end()){
                    val = std::stoi(reg);
                }
                else{
                    val = registers[reg];
                }
                values.push_back(val);

            }
            if (instruction == "tgl"){
               if (registers.find(parsed_line[1]) == registers.end()){
                   val = std::stoi(parsed_line[1]);
               }
               else{
                   val = registers[parsed_line[1]];
               }
               if (i+val >= n) {
                   i++;
                    continue;
               }
               instructions[i+val][0] = changes[instructions[i+val][0]];

            }
            if (instruction == "jnz"){
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

                if (val != 0) {
                    i += offset;
                    continue;
                }
            }
            else{
               if (instruction == "dec"){
                   registers[parsed_line[1]]--;
               }
               else if (instruction == "inc"){
                   registers[parsed_line[1]]++;
               }
               else if (instruction == "cpy"){
                   reg = parsed_line[2];
                   if (registers.find(reg) == registers.end()){
                       // toggling has created an invalid op, skip
                       i++;
                       continue;
                   }
                   if (registers.find(parsed_line[1]) == registers.end()){
                       registers[reg] = std::stoi(parsed_line[1]);
                   }
                   else{
                       registers[reg] = registers[parsed_line[1]];
                   }
               }
            }
            i++;
        }
        a_val++;
    }
}
