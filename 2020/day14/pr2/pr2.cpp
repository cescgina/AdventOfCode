#include <map>
#include <queue>
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
typedef std::map<number, number> memory;
typedef std::bitset<36> mask;
typedef std::queue<int> indices;

void set_addresses(indices to_flip, memory& mem, number address, const number value){
    if (to_flip.empty()){
        if (DEBUG) std::cout << "Set address " << address <<" to " << value << std::endl;
        mem[address] = value;
        return;
    }
    int position = to_flip.front();
    to_flip.pop();
    set_addresses(to_flip, mem, address | 1UL << position, value);
    set_addresses(to_flip, mem, address & ~(1UL << position), value);
}
int main(int argc, char** argv){
    std::string file_name;
    getopt(argc, argv, "");
    file_name = std::string(argv[optind]);
    std::ifstream input_file(file_name);
    mask ones;
    indices to_flip;
    memory mem;
    for (std::string line; std::getline(input_file, line);){
        if (line[1] == 'a'){
            ones.reset();
            while(not to_flip.empty()){
                to_flip.pop();
            }
            auto pos = line.find("=");
            std::string mask_str = line.substr(pos+2);
            for (size_t i = 0; i<mask_str.size(); i++){
                if (mask_str[i] == '1'){
                    ones[mask_str.size()-i-1] = 1;
                }
                else if (mask_str[i] == 'X'){
                    to_flip.push(mask_str.size()-i-1);
                }
            }
        }
        else{
            auto pos = line.find("[");
            auto pos_end = line.find("]");
            number address = std::stoull(line.substr(pos+1, pos_end-pos-1)) | ones.to_ullong();
            pos = line.find("=");
            number value = std::stoull(line.substr(pos+2));
            set_addresses(to_flip, mem, address, value);
        }
    }
    input_file.close();
    number sum = 0;
    for (auto& memory_location: mem){
        sum += memory_location.second;
    }
    std::cout << sum << std::endl;
}
