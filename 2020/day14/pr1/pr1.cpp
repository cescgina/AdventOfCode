#include <map>
#include <bitset>
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <unistd.h>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>

bool DEBUG = false;

typedef unsigned long long int number;
typedef std::map<int, number> memory;
typedef std::bitset<36> mask;

int main(int argc, char** argv){
    std::string file_name;
    getopt(argc, argv, "");
    file_name = std::string(argv[optind]);
    std::ifstream input_file(file_name);
    mask ones;
    mask zeros;
    memory mem;
    for (std::string line; std::getline(input_file, line);){
        if (line[1] == 'a'){
            ones.reset();
            zeros.set();
            auto pos = line.find("=");
            std::string mask_str = line.substr(pos+2);
            for (size_t i = 0; i<mask_str.size(); i++){
                if (mask_str[i] == '0'){
                    zeros[mask_str.size()-i-1] = 0;
                }
                else if (mask_str[i] == '1'){
                    ones[mask_str.size()-i-1] = 1;
                }
            }
        }
        else{
            auto pos = line.find("[");
            auto pos_end = line.find("]");
            int address = std::stoi(line.substr(pos+1, pos_end-pos-1));
            pos = line.find("=");
            number value = std::stoull(line.substr(pos+2));
            mem[address] = (value & zeros.to_ullong()) | ones.to_ullong(); 
            if (DEBUG) std::cout << "Set address " << address <<" to " << mem[address] << std::endl;
        }
    }
    input_file.close();
    number sum = 0;
    for (auto& memory_location: mem){
        sum += memory_location.second;
    }
    std::cout << sum << std::endl;
}
