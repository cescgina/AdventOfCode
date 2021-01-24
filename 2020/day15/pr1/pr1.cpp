#include <map>
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <unistd.h>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>

bool DEBUG = false;

typedef std::map<int, int> memory;

bool contains(memory& haystack, int needle){
    return haystack.find(needle) != haystack.end();
}

int main(int argc, char** argv){
    std::string file_name;
    getopt(argc, argv, "");
    file_name = std::string(argv[optind]);
    std::ifstream input_file(file_name);
    std::string line; 
    std::getline(input_file, line);
    std::vector<std::string> to_parse;
    boost::split(to_parse, line, boost::is_any_of(","), boost::token_compress_on);
    memory mem;
    int turn = 1, last = 0;
    for (auto& n: to_parse){
        last = std::stoi(n);
        mem[last] = turn;
        if (DEBUG) std::cout << turn << " " << last << std::endl;
        turn++;
    }
    last = 0;
    turn++;
    while (turn <2021){
        if (contains(mem, last)){
            int temp = mem[last];
            mem[last] = turn-1;
            last = turn-temp-1;
        }
        else{
            mem[last] = turn-1;
            last = 0;
        }
        if (DEBUG) std::cout << turn << " " << last << std::endl;
        turn++;
    }
    std::cout<< last << std::endl;
}
