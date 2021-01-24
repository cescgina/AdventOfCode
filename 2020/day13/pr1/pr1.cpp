#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <unistd.h>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>

bool DEBUG = false;

int main(int argc, char** argv){
    std::string file_name;
    getopt(argc, argv, "");
    file_name = std::string(argv[optind]);
    std::ifstream input_file(file_name);
    std::string line; 
    std::getline(input_file, line);
    int start = std::stoi(line);
    std::getline(input_file, line);
    std::vector<std::string> to_parse;
    std::vector<int> buses;
    boost::split(to_parse, line, boost::is_any_of(","), boost::token_compress_on);
    for (auto& bus: to_parse){
        if (bus[0] == 'x') continue;
        buses.push_back(std::stoi(bus));
    }
    int time = start;
    while (true){
        for (int bus: buses){
            if (time % bus == 0){
                std::cout << bus*(time-start) << std::endl;
                return 0;
            }
        }
        time++;
    }
}
